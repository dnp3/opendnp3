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

#include <opendnp3/outstation/OutstationStackConfig.h>
#include <opendnp3/outstation/SimpleCommandHandler.h>
#include <opendnp3/outstation/TimeTransaction.h>
#include <opendnp3/outstation/ITimeWriteHandler.h>
#include <opendnp3/LogLevels.h>

#include <asiopal/Log.h>
#include <asiopal/LogToStdio.h>
#include <asiopal/IOServiceThreadPool.h>
#include <asiopal/UTCTimeSource.h>
#include <asiopal/PhysicalLayerAsyncTCPServer.h>

#include <asiodnp3/IChannel.h>
#include <asiodnp3/IOutstation.h>

#include <string>
#include <iostream>

using namespace std;
using namespace opendnp3;
using namespace openpal;
using namespace asiopal;
using namespace asiodnp3;

int main(int argc, char* argv[])
{

	// Specify a LogLevel for the stack/physical layer to use.
	// Log statements with a lower priority will not be logged.
	const uint32_t FILTERS = levels::NORMAL;

	//A default logging backend that can proxy to multiple other backends
	EventLog log;
	LogToStdio iologger;
	log.AddLogSubscriber(&iologger); // This singleton logger just prints messages to the console

	IOServiceThreadPool pool(&log, FILTERS,  1); // only 1 thread is needed for a single stack

	// This is the main point of interaction with the stack
	DNP3Manager mgr;

	// you can optionally pass a function into the client constructor to configure your socket
	// using platform specific options
	auto configure = [](asio::ip::tcp::socket & socket)
	{
		// platfrom specific socket configuration here
	};

	// Create the raw physical layer
	auto pServerRoot = new LogRoot(&log, "server", FILTERS);
	auto pServerPhys = new PhysicalLayerAsyncTCPServer(*pServerRoot, pool.GetIOService(), "0.0.0.0", 20000, configure);
	// Wrap the physical layer in a DNP channel
	auto pServer = mgr.CreateChannel(pServerRoot, TimeDuration::Seconds(5), TimeDuration::Seconds(5), pServerPhys);

	// The master config object for a outstation. The default are
	// useable, but understanding the options are important.
	OutstationStackConfig stackConfig;
	stackConfig.dbTemplate = DatabaseTemplate::AllTypes(10);
	stackConfig.outstation.mDisableUnsol = true;

	// Create a new outstation with a log level, command handler, and
	// config info this	returns a thread-safe interface used for
	// updating the outstation's database.
	auto pOutstation = pServer->AddOutstation("outstation", SuccessCommandHandler::Inst(), NullTimeWriteHandler::Inst(), stackConfig);

	// TODO - reach in and configure the database here before it is enabled

	// Enable the outstation and start communications
	pOutstation->Enable();

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
