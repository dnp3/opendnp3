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

#include <opendnp3/outstation/OutstationStackConfig.h>
#include <opendnp3/outstation/SimpleCommandHandler.h>
#include <opendnp3/outstation/TimeTransaction.h>
#include <opendnp3/outstation/ITimeWriteHandler.h>
#include <opendnp3/link/LinkLayerRouter.h>
#include <opendnp3/outstation/Outstation.h>
#include <opendnp3/app/ApplicationStack.h>
#include <opendnp3/outstation/DynamicallyAllocatedDatabase.h>
#include <opendnp3/outstation/DynamicallyAllocatedEventBuffer.h>
#include <opendnp3/LogLevels.h>

#include <asiopal/Log.h>
#include <asiopal/LogToStdio.h>
#include <asiopal/IOServiceThreadPool.h>
#include <asiopal/UTCTimeSource.h>
#include <asiopal/PhysicalLayerAsyncTCPServer.h>

#include <openpal/Logger.h>

#include <string>
#include <iostream>

using namespace std;
using namespace opendnp3;
using namespace openpal;
using namespace asiopal;

int main(int argc, char* argv[])
{
	std::cout << sizeof(ApplicationStack) << std::endl;
	std::cout << sizeof(LinkLayer) << std::endl;
	std::cout << sizeof(AppLayer) << std::endl;
	std::cout << sizeof(TransportLayer) << std::endl;

	// Specify a LogLevel for the stack/physical layer to use.
	// Log statements with a lower priority will not be logged.
	const uint32_t LOG_LEVEL = levels::ALL;

	//A default logging backend that can proxy to multiple other backends
	EventLog el;	
	el.AddLogSubscriber(LogToStdio::Inst()); // This singleton logger just prints messages to the console

	asio::io_service service;
	asio::io_service::strand strand(service);
	asiopal::ASIOExecutor executor(&strand);

	LinkRoute route(1, 1024);

	PhysicalLayerAsyncTCPServer server(LogConfig(&el, LOG_LEVEL, "tcpserver"), &service, "127.0.0.1", 20000);
	LinkLayerRouter router(server.GetLogger().GetSubLogger("router"), &server, TimeDuration::Seconds(1), TimeDuration::Seconds(60));
	ApplicationStack stack(server.GetLogger().GetSubLogger("root"), &executor, AppConfig(false), LinkConfig(false, false));
	stack.link.SetRouter(&router);
	router.AddContext(&stack.link, route);

	DynamicallyAllocatedDatabase dadb(DatabaseTemplate::AllTypes(5));
	DynamicallyAllocatedEventBuffer eb(EventBufferConfig::AllTypes(100));

	Database database(dadb.GetFacade());

	Outstation outstation(
		server.GetLogger().GetSubLogger("outstation"), 
		&stack.application, 
		&executor, 
		NullTimeWriteHandler::Inst(), 
		&database,
		eb.GetFacade(), 
		SuccessCommandHandler::Inst(), 
		OutstationConfig());
	
	
	stack.application.SetUser(&outstation);

	router.Enable(&stack.link);

	//LogEntry le()

	el.Log(LogEntry(LogFilters(~0), "hello", "", "test", -1));

	// Start dispatching events
	service.run();

	return 0;
}
