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
#include <opendnp3/link/LinkLayerRouter.h>
#include <opendnp3/outstation/Slave.h>
#include <opendnp3/app/ApplicationStack.h>
#include <opendnp3/outstation/DynamicallyAllocatedDatabase.h>

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

	// Specify a LogLevel for the stack/physical layer to use.
	// Log statements with a lower priority will not be logged.
	const LogLevel LOG_LEVEL = levels::INTERPRET;

	//A default logging backend that can proxy to multiple other backends
	EventLog log;
	log.AddLogSubscriber(LogToStdio::Inst()); // This singleton logger just prints messages to the console

	asio::io_service service;
	asio::io_service::strand strand(service);
	asiopal::ASIOExecutor executor(&strand);

	LinkRoute route(1, 1024);

	PhysicalLayerAsyncTCPServer server(Logger(&log, LOG_LEVEL, "tcpserver"), &service, "127.0.0.1", 20000);
	LinkLayerRouter router(Logger(&log, LOG_LEVEL, "router"), &server, TimeDuration::Seconds(1), TimeDuration::Seconds(60));
	ApplicationStack stack(Logger(&log, LOG_LEVEL, "root"), &executor, AppConfig(false), LinkConfig(false, false));
	stack.link.SetRouter(&router);
	router.AddContext(&stack.link, route);
	
	DynamicallyAllocatedDatabase dadb(DatabaseTemplate::AllTypes(5));
	Database database(dadb.GetFacade());
	Slave slave(Logger(&log, LOG_LEVEL, "slave"), &stack.application, &executor, NullTimeWriteHandler::Inst(), &database, SuccessCommandHandler::Inst(), SlaveConfig());
	stack.application.SetUser(&slave);

	router.Enable(&stack.link);

	// Start dispatching events
	service.run();

	return 0;
}
