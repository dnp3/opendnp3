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


#include <opendnp3/link/LinkLayerRouter.h>
#include <opendnp3/outstation/Outstation.h>
#include <opendnp3/transport/TransportStack.h>
#include <opendnp3/outstation/StaticallyAllocatedDatabase.h>
#include <opendnp3/outstation/DynamicallyAllocatedDatabase.h>
#include <opendnp3/outstation/StaticallyAllocatedEventBuffer.h>
#include <opendnp3/outstation/NewOutstation.h>
#include <opendnp3/LogLevels.h>
#include <opendnp3/outstation/SimpleCommandHandler.h>

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

void ToggleBinaryEvery(uint16_t milliseconds, IExecutor* pExecutor, Database* pDatabase, bool value, bool update);

int main(int argc, char* argv[])
{
	cout << sizeof(TransportLayer) << " - TL " << endl;
	cout << sizeof(LinkLayer) << " - LL " << endl;
	cout << sizeof(NewOutstation) << " - NO " << endl;

	// Specify a LogLevel for the stack/physical layer to use.
	// Log statements with a lower priority will not be logged.
	const uint32_t FILTERS = levels::ALL;
	
	LogToStdio iologger;	

	openpal::LogRoot root(&iologger, "server", FILTERS);

	asio::io_service service;
	asio::io_service::strand strand(service);
	asiopal::ASIOExecutor executor(&strand);

	PhysicalLayerAsyncTCPServer server(root, &service, "0.0.0.0", 20000);
	LinkLayerRouter router(root, &server, TimeDuration::Seconds(1), TimeDuration::Seconds(60));
		
	TransportStack stack(root, &executor, LinkConfig(false, false));

	//StaticallyAllocatedDatabase<5, 5, 5, 5, 5, 5, 5> staticBuffers;
	DynamicallyAllocatedDatabase staticBuffers(DatabaseTemplate::BinaryOnly(5));
	StaticallyAllocatedEventBuffer<10, 10, 10, 10, 10, 10, 10> eventBuffers;

	Database database(staticBuffers.GetFacade());

	SimpleCommandHandler commandHandler(CommandStatus::SUCCESS);

	NewOutstationConfig config;	
	config.params.maxTxFragSize = 10;
	config.defaultEventResponses.binary = EventBinaryResponse::Group2Var2;
	NewOutstation outstation(config, executor, root, stack.transport, commandHandler, NullTimeWriteHandler::Inst(), database, eventBuffers.GetFacade());

	stack.transport.SetAppLayer(&outstation);

	stack.link.SetRouter(&router);
	router.AddContext(&stack.link, LinkRoute(1, 1024));

	router.Enable(&stack.link);

	ToggleBinaryEvery(3000, &executor, &database, true, false);

	// Start dispatching events
	service.run();

	return 0;
}

void ToggleBinaryEvery(uint16_t milliseconds, IExecutor* pExecutor, Database* pDatabase, bool value, bool update)
{
	if (update)
	{
		pDatabase->Update(Binary(value), 0);
	}

	auto lambda = [pExecutor, pDatabase, value, milliseconds]() { ToggleBinaryEvery(milliseconds, pExecutor, pDatabase, !value, true); };
	pExecutor->Start(TimeDuration::Milliseconds(milliseconds), Bind(lambda));
}
