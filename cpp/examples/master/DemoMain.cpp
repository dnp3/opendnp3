/**
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */
#include <asiodnp3/DNP3Manager.h>
#include <asiodnp3/PrintingSOEHandler.h>
#include <asiodnp3/ConsoleLogger.h>

#include <asiopal/UTCTimeSource.h>

#include <opendnp3/LogLevels.h>
#include <opendnp3/app/ControlRelayOutputBlock.h>

#include "BlockingCommandCallback.h"

#include <thread>

using namespace std;
using namespace openpal;
using namespace asiopal;
using namespace asiodnp3;
using namespace opendnp3;

int main(int argc, char* argv[])
{

	// Specify what log levels to use. NORMAL is warning and above
	// You can add all the comms logging by uncommenting below
	const uint32_t FILTERS = levels::NORMAL; // | levels::ALL_COMMS;

	// This is the main point of interaction with the stack
	DNP3Manager manager(std::thread::hardware_concurrency());

	// send log messages to the console
	manager.AddLogSubscriber(&ConsoleLogger::Instance());	

	// Connect via a TCPClient socket to a outstation	
	auto pClient = manager.AddTCPClient("tcpclient", FILTERS, TimeDuration::Seconds(2), TimeDuration::Minutes(1), "127.0.0.1", 20000);

	// Optionally, you can bind listeners to the channel to get state change notifications
	// This listener just prints the changes to the console
	pClient->AddStateListener([](ChannelState state) 
	{
		std::cout << "channel state: " << ChannelStateToString(state) << std::endl;
	});

	// The master config object for a master. The default are
	// useable, but understanding the options are important.
	MasterStackConfig stackConfig;

	// you can override application layer settings for the master here
	// in this example, we've change the application layer timeout to 2 seconds
	stackConfig.master.responseTimeout = TimeDuration::Seconds(2);
	
	// You can override the default link layer settings here
	// in this example we've changed the default link layer addressing
	stackConfig.link.LocalAddr = 1;
	stackConfig.link.RemoteAddr = 10;

	// Create a new master on a previously declared port, with a
	// name, log level, command acceptor, and config info. This
	// returns a thread-safe interface used for sending commands.
	auto pMaster = pClient->AddMaster(
	                   "master",						// id for logging
	                   PrintingSOEHandler::Inst(),		// callback for data processing
	                   asiopal::UTCTimeSource::Inst(),	// system clock for time syncing
	                   stackConfig						// stack configuration
	               );
	
	
	// do an integrity poll (Class 3/2/1/0) once per minute
	auto integrityScan = pMaster->AddClassScan(ClassField::AllClasses(), TimeDuration::Minutes(1));

	// do a Class 1 exception poll every 5 seconds
	auto exceptionScan = pMaster->AddClassScan(ClassField(ClassField::CLASS_1), TimeDuration::Seconds(5));

	// Enable the master. This will start communications.
	pMaster->Enable();

	auto pCommandProcessor = pMaster->GetCommandProcessor();

	do
	{

		std::cout << "Enter a command" << std::endl;
		std::cout << "x - exits program" << std::endl;
		std::cout << "i - integrity demand scan" << std::endl;
		std::cout << "e - exception demand scan" << std::endl;
		std::cout << "c - send crob" << std::endl;
	
		char cmd;
		std::cin >> cmd;
		switch(cmd)
		{
			case('x'):
				// C++ destructor on DNP3Manager cleans everything up for you
				return 0;
			case('i'):
				integrityScan.Demand();
				break;
			case('e'):
				exceptionScan.Demand();
				break;
			case('c'):
				{
					// This is an example of synchronously doing a control operation
					ControlRelayOutputBlock crob(ControlCode::LATCH_ON);
					BlockingCommandCallback handler;
					pCommandProcessor->SelectAndOperate(crob, 0, handler);
					auto response = handler.WaitForResult();
					std::cout << "Result: " << CommandResultToString(response.GetResult()) <<
								 " Status: " << CommandStatusToString(response.GetStatus()) << std::endl;
					break;
				}
			default:
				std::cout << "Unknown action: " << cmd << std::endl;
				break;
		}
	}
	while(true);

	return 0;
}

