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
#include <asiodnp3/IChannel.h>
#include <asiodnp3/IMaster.h>
#include <asiodnp3/PrintingSOEHandler.h>

#include <opendnp3/master/MasterStackConfig.h>
#include <opendnp3/master/ICommandProcessor.h>
#include <opendnp3/master/ISOEHandler.h>
#include <opendnp3/app/ClassMask.h>
#include <opendnp3/LogLevels.h>

#include <asiopal/Log.h>
#include <asiopal/LogToStdio.h>
#include <asiopal/UTCTimeSource.h>
#include <asiopal/PhysicalLayerAsyncTCPClient.h>
#include <asiopal/IOServiceThreadPool.h>

#include <iostream>
#include <future>

#include "PrintingCommandCallback.h"

using namespace std;
using namespace openpal;
using namespace asiopal;
using namespace asiodnp3;
using namespace opendnp3;

int main(int argc, char* argv[])
{

	// Specify a LogLevel for the stack/physical layer to use.
	// Log statements with a lower priority will not be logged.
	const uint32_t FILTERS = levels::NORMAL;
	
	LogToStdio iologger;

	// asio thread pool that drives the stack
	IOServiceThreadPool pool(&iologger, FILTERS, 1); // 1 stack only needs 1 thread

	// This is the main point of interaction with the stack
	DNP3Manager mgr;

	// you can optionally pass a function into the client constructor to configure your socket
	// using platform specific options
	auto configure = [](asio::ip::tcp::socket & socket)
	{
		// platfrom specific socket configuration here
	};

	// Connect via a TCPClient socket to a outstation
	auto pClientRoot = new LogRoot(&iologger, "client", FILTERS);
	auto pClientPhys = new PhysicalLayerAsyncTCPClient(*pClientRoot, pool.GetIOService(), "127.0.0.1", 20000, configure);
	// wait 3000 ms in between failed connect calls
	auto pClient = mgr.CreateChannel(pClientRoot, TimeDuration::Seconds(2), TimeDuration::Minutes(1), pClientPhys);

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

	auto integrityScan = pMaster->GetIntegrityScan();
	auto exceptionScan = pMaster->AddClassScan(CLASS_1 | CLASS_2 | CLASS_3, TimeDuration::Seconds(5), TimeDuration::Seconds(5));

	// Enable the master. This will start communications.
	pMaster->Enable();

	auto pCmdProcessor = pMaster->GetCommandProcessor();

	do
	{

		std::cout << "Enter a command {x == exit, d == demand scan, c == control}" << std::endl;
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
				pCmdProcessor->SelectAndOperate(crob, 0, &PrintingCommandCallback::Inst());
				break;
			}
		default:
			break;
		}
	}
	while(true);

	return 0;
}

