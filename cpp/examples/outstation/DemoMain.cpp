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

#include <opendnp3/outstation/ICommandHandler.h>
#include <opendnp3/outstation/TimeTransaction.h>
#include <opendnp3/outstation/SimpleCommandHandler.h>
#include <opendnp3/outstation/ITimeWriteHandler.h>

#include <opendnp3/LogLevels.h>

#include <string>
#include <thread>
#include <iostream>

using namespace std;
using namespace opendnp3;
using namespace openpal;
using namespace asiopal;
using namespace asiodnp3;

int main(int argc, char* argv[])
{

	// Specify what log levels to use. NORMAL is warning and above
	// You can add all the comms logging by uncommenting below.
	const uint32_t FILTERS = levels::NORMAL; // | levels::ALL_COMMS;

	// This is the main point of interaction with the stack
	DNP3Manager manager(std::thread::hardware_concurrency());

	// send log messages to the console
	manager.AddLogSubscriber(&ConsoleLogger::Instance());

	// Create a TCP server (listener)	
	auto pServer = manager.AddTCPServer("server", FILTERS, TimeDuration::Seconds(5), TimeDuration::Seconds(5), "0.0.0.0", 20000);

	// The main object for a outstation. The defaults are useable, 
	// but understanding the options are important.
	OutstationStackConfig stackConfig;	
	
	// You must specify the shape of your database and the size of the event buffers
	stackConfig.dbTemplate = DatabaseTemplate::AllTypes(10);
	stackConfig.eventBuffer = EventBufferConfig::AllTypes(10);
	
	// you can override an default outstation parameters here
	// in this example, we've enabled the oustation to use unsolicted reporting
	// if the master enables it
	stackConfig.outstation.params.allowUnsolicited = true;	

	// You can override the default link layer settings here
	// in this example we've changed the default link layer addressing
	stackConfig.link.LocalAddr = 10;
	stackConfig.link.RemoteAddr = 1;

	// You can optionally change the default reporting variations
	stackConfig.outstation.defaultEventResponses.binary = EventBinaryResponse::Group2Var2;
	stackConfig.outstation.defaultEventResponses.analog = EventAnalogResponse::Group32Var3;
	
	// Create a new outstation with a log level, command handler, and
	// config info this	returns a thread-safe interface used for
	// updating the outstation's database.
	auto pOutstation = pServer->AddOutstation("outstation", SuccessCommandHandler::Inst(), &NullTimeWriteHandler::Inst(), stackConfig);	

	// Enable the outstation and start communications
	pOutstation->Enable();

	// Get the interface used to load measurements into the outstation
	auto pLoader = pOutstation->GetLoader();

	// variables used in example loop
	char input;
	uint32_t count = 0;
	double value = 0;
	bool binary = false;
	DoubleBit dbit = DoubleBit::DETERMINED_OFF;
	bool run = true;

	while(run)
	{
		std::cout << "c = counter, b = binary, d = doublebit, a = analog, x = exit" << std::endl;
		std::cin >> input;
		switch (input)
		{
		case('c'):
			{
				TimeTransaction tx(pLoader, UTCTimeSource::Inst()->Now());
				tx.Update(Counter(count, CQ_ONLINE), 0);
				++count;
				break;
			}
		case('a') :
			{
				TimeTransaction tx(pLoader, UTCTimeSource::Inst()->Now());
				tx.Update(Analog(value, AQ_ONLINE), 0);
				value += 1.75;
				break;
			}
		case('b') :
			{
				TimeTransaction tx(pLoader, UTCTimeSource::Inst()->Now());
				tx.Update(Binary(binary), 0);
				binary = !binary;
				break;
			}
		case('d') :
			{
				TimeTransaction tx(pLoader, UTCTimeSource::Inst()->Now());
				tx.Update(DoubleBitBinary(dbit), 0);
				dbit = (dbit == DoubleBit::DETERMINED_OFF) ? DoubleBit::DETERMINED_ON : DoubleBit::DETERMINED_OFF;
				break;
			}
		case('x') :
			{
				run = false;
				break;
			}
		default:
			std::cout << "No action registered for: " << input << std::endl;
			break;
		}
	}

	return 0;
}
