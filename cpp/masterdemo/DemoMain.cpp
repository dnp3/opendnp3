
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the GNU Affero General Public License Version 3.0
// (the "Additional License"). You may not use these modifications except in
// compliance with the additional license. You may obtain a copy of the Additional
// License at
//
// http://www.gnu.org/licenses/agpl.html
//
// Contact Automatak, LLC for a commercial license to these modifications
//

#include <opendnp3/DNP3Manager.h>
#include <opendnp3/IChannel.h>
#include <opendnp3/IMaster.h>
#include <opendnp3/MasterStackConfig.h>
#include <opendnp3/ICommandProcessor.h>
#include <opendnp3/IMeasurementHandler.h>

#include <asiopal/Log.h>
#include <asiopal/LogToStdio.h>
#include <asiopal/UTCTimeSource.h>
#include <asiopal/PhysicalLayerAsyncTCPClient.h>
#include <asiopal/IOServiceThreadPool.h>

#include <iostream>
#include <future>

using namespace std;
using namespace asiopal;
using namespace opendnp3;

int main(int argc, char* argv[])
{

	// Specify a FilterLevel for the stack/physical layer to use.
	// Log statements with a lower priority will not be logged.
	const FilterLevel LOG_LEVEL = LEV_INFO;

	EventLog log;
	// You can optionally subcribe to log messages
	// This singleton logger just prints messages to the console
	log.AddLogSubscriber(LogToStdio::Inst());

	// asio thread pool that drives the stack
	IOServiceThreadPool pool(Logger(&log, LOG_LEVEL, "pool"), 1); // 1 stack only needs 1 thread

	// This is the main point of interaction with the stack
	DNP3Manager mgr;

	// Connect via a TCPClient socket to a slave
	auto pClientPhys = new PhysicalLayerAsyncTCPClient(Logger(&log, LOG_LEVEL, "tcpclient"), pool.GetIOService(), "127.0.0.1", 20000);
	// wait 3000 ms in between failed connect calls.
	auto pClient = mgr.CreateChannel(Logger(&log, LOG_LEVEL, "tcpclient"), TimeDuration::Seconds(3), pClientPhys);

	// You can optionally add a listener to the channel. You can do this anytime and
	// you will receive a stream of all state changes
	pClient->AddStateListener([](ChannelState state) {
		std::cout << "Client state: " << ConvertChannelStateToString(state) << std::endl;
	});

	// The master config object for a master. The default are
	// useable, but understanding the options are important.
	MasterStackConfig stackConfig;

	// Create a new master on a previously declared port, with a
	// name, log level, command acceptor, and config info. This
	// returns a thread-safe interface used for sending commands.
	auto pMaster = pClient->AddMaster(
	                       "master",						// stack name
	                       LOG_LEVEL,						// log filter level
						   PrintingMeasurementHandler::Inst(),	// callback for data processing
	                       asiopal::UTCTimeSource::Inst(),	// system clock for time syncing
	                       stackConfig						// stack configuration
	               );

	// You can optionally add a listener to the stack to observer communicate health. You
	// can do this anytime and you will receive a stream of all state changes.
	pMaster->AddStateListener([](StackState state) {
		std::cout << "master state: " << ConvertStackStateToString(state) << std::endl;
	});

	// Enable the master. This will start communications.
	pMaster->Enable();

	auto pCmdProcessor = pMaster->GetCommandProcessor();

	std::string cmd;
	do {
		std::cout << "Enter something to perform a SELECT/OPERATE sequence, or type exit" << std::endl;
		std::cin >> cmd;
		if(cmd == "exit") break;
		else {
			ControlRelayOutputBlock crob(CC_LATCH_ON);

			promise<CommandResponse> selectResult;
			pCmdProcessor->SelectAndOperate(crob, 0, [&](CommandResponse cr) {
				selectResult.set_value(cr);
			});
			CommandResponse rsp = selectResult.get_future().get();
			std::cout << "Select/Operate result: " << rsp.mResult << std::endl;
		}
	}
	while(true);

	return 0;
}

