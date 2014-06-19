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
#include <opendnp3/app/PointClass.h>
#include <opendnp3/app/ControlRelayOutputBlock.h>

#include "PrintingCommandCallback.h"

#include <thread>

using namespace std;
using namespace openpal;
using namespace asiopal;
using namespace asiodnp3;
using namespace opendnp3;

int main(int argc, char* argv[])
{

	// Specify what log levels to use. NORMAL is warning and above
	const uint32_t FILTERS = levels::NORMAL | levels::ALL_COMMS;

	// This is the main point of interaction with the stack
	DNP3Manager manager(std::thread::hardware_concurrency());

	// send log messages to the console
	manager.AddLogSubscriber(&ConsoleLogger::Instance());	

	// Connect via a TCPClient socket to a outstation	
	auto pClient = manager.AddTCPClient("tcpclient", FILTERS, TimeDuration::Seconds(2), TimeDuration::Minutes(1), "127.0.0.1", 20000);

	// The master config object for a master. The default are
	// useable, but understanding the options are important.
	MasterStackConfig stackConfig;
	stackConfig.master.integrityPeriod = TimeDuration::Minutes(1);

	// Create a new master on a previously declared port, with a
	// name, log level, command acceptor, and config info. This
	// returns a thread-safe interface used for sending commands.
	auto pMaster = pClient->AddMaster(
	                   "master",						// id for logging
	                   PrintingSOEHandler::Inst(),		// callback for data processing
	                   asiopal::UTCTimeSource::Inst(),	// system clock for time syncing
	                   stackConfig						// stack configuration
	               );
	
	
	auto integrityScan = pMaster->AddClassScan(ALL_CLASSES, TimeDuration::Minutes(1));
	auto exceptionScan = pMaster->AddClassScan(ALL_EVENT_CLASSES, TimeDuration::Seconds(5));


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
				return 0;
			case('i'):
				integrityScan.Demand();
				break;
			case('e'):
				exceptionScan.Demand();
				break;
			case('c'):
				{
					ControlRelayOutputBlock crob(ControlCode::LATCH_ON);								
					pCommandProcessor->SelectAndOperate(crob, 0, &PrintingCommandCallback::Inst());
					break;
				}
			default:
				break;
		}
	}
	while(true);

	return 0;
}

