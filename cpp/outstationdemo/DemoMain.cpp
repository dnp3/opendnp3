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
#include <opendnp3/DNP3Manager.h>

#include <opendnp3/outstation/SlaveStackConfig.h>
#include <opendnp3/outstation/SimpleCommandHandler.h>
#include <opendnp3/outstation/TimeTransaction.h>
#include <opendnp3/outstation/ITimeWriteHandler.h>
#include <opendnp3/outstation/IOutstation.h>

#include <opendnp3/IChannel.h>

#include <asiopal/Log.h>
#include <asiopal/LogToStdio.h>
#include <asiopal/IOServiceThreadPool.h>
#include <asiopal/UTCTimeSource.h>
#include <asiopal/PhysicalLayerAsyncTCPServer.h>

#include <string>
#include <iostream>

using namespace std;
using namespace opendnp3;
using namespace asiopal;

int main(int argc, char* argv[])
{

	// Specify a LogLevel for the stack/physical layer to use.
	// Log statements with a lower priority will not be logged.
	const LogLevel LOG_LEVEL = LogLevel::Info;

	//A default logging backend that can proxy to multiple other backends
	EventLog log;
	log.AddLogSubscriber(LogToStdio::Inst()); // This singleton logger just prints messages to the console

	IOServiceThreadPool pool(Logger(&log, LOG_LEVEL, "pool"), 1); // only 1 thread is needed for a single stack

	// This is the main point of interaction with the stack
	DNP3Manager mgr;

	// you can optionally pass a function into the client constructor to configure your socket
	// using platform specific options
	auto configure = [](boost::asio::ip::tcp::socket& socket){
		// platfrom specific socket configuration here
	};

	// Create the raw physical layer
	auto pServerPhys = new PhysicalLayerAsyncTCPServer(Logger(&log, LOG_LEVEL, "tcpserver"), pool.GetIOService(), "127.0.0.1", 20000, configure);
	// Wrap the physical layer in a DNP channel
	auto pServer = mgr.CreateChannel(Logger(&log, LOG_LEVEL, "tcpserver"), TimeDuration::Seconds(5), TimeDuration::Seconds(5), pServerPhys);

	// You can optionally add a listener to the channel. You can do this anytime and
	// you will receive a stream of all state changes
	pServer->AddStateListener([](ChannelState state) {
		std::cout << "Server state: " << ChannelStateToString(state) << std::endl;
	});

	

	// The master config object for a slave. The default are
	// useable, but understanding the options are important.
	SlaveStackConfig stackConfig;	
	stackConfig.database = DatabaseConfiguration(DatabaseTemplate(5, 5, 5, 5, 5, 5));	
	stackConfig.slave.mDisableUnsol = true;

	// Create a new slave with a log level, command handler, and
	// config info this	returns a thread-safe interface used for
	// updating the slave's database.
	auto pOutstation = pServer->AddOutstation("outstation", LOG_LEVEL, SuccessCommandHandler::Inst(), NullTimeWriteHandler::Inst(), stackConfig);

	// You can optionally add a listener to the stack to observer communicate health. You
	// can do this anytime and you will receive a stream of all state changes.
	pOutstation->AddStateListener([](StackState state) {
		std::cout << "outstation state: " << StackStateToString(state) << std::endl;
	});

	// Enable the outstation and start communications
	pOutstation->Enable();

	auto pDataObserver = pOutstation->GetDataObserver();

	std::string input;
	uint32_t count = 0;
	do {
		std::cout << "Enter something to increment a counter or type exit" << std::endl;
		std::cin >> input;
		if(input == "exit") break;
		else {
			TimeTransaction tx(pDataObserver, UTCTimeSource::Inst()->Now()); //automatically calls Start()/End() and sets time for each measurement
			tx.Update(Counter(count, CQ_ONLINE), 0);
			++count;
		}
	}
	while(true);

	return 0;
}
