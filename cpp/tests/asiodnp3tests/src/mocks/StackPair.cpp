/*
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

#include "StackPair.h"

#include "asiodnp3/DefaultMasterApplication.h"

#include "opendnp3/outstation/SimpleCommandHandler.h"

#include <sstream>

namespace asiodnp3 {

	StackPair::StackPair(DNP3Manager& manager, uint16_t port, uint16_t numPointsPerType, uint32_t eventsPerIteration) :
		NUM_POINTS_PER_TYPE(numPointsPerType),
		EVENTS_PER_ITERATION(eventsPerIteration),
		soeHandler(std::make_shared<opendnp3::QueuingSOEHandler>()),
		clientListener(std::make_shared<QueuedChannelListener>()),
		serverListener(std::make_shared<QueuedChannelListener>()),
		master(CreateMaster(manager, port, this->soeHandler, this->clientListener)),
		outstation(CreateOutstation(manager, port, numPointsPerType, eventsPerIteration, this->serverListener))
	{
		this->outstation->Enable();
		this->master->Enable();		
	}
	
	bool StackPair::WaitForChannelsOnline(std::chrono::steady_clock::duration timeout)
	{
		return this->clientListener->WaitForState(opendnp3::ChannelState::OPEN, timeout) &&
			this->serverListener->WaitForState(opendnp3::ChannelState::OPEN, timeout);
	}

	OutstationStackConfig StackPair::GetOutstationStackConfig(uint16_t numPointsPerType, uint16_t eventBufferSize)
	{
		OutstationStackConfig config(opendnp3::DatabaseSizes::AllTypes(numPointsPerType));

		config.outstation.eventBufferConfig = opendnp3::EventBufferConfig::AllTypes(eventBufferSize);
		config.outstation.params.allowUnsolicited = true;

		return config;
	}

	MasterStackConfig StackPair::GetMasterStackConfig()
	{
		MasterStackConfig config;

		config.master.disableUnsolOnStartup = false;
		config.master.startupIntegrityClassMask = opendnp3::ClassField::None();
		config.master.unsolClassMask = opendnp3::ClassField::AllEventClasses();

		return config;
	}

	IMaster* StackPair::CreateMaster(DNP3Manager& manager, uint16_t port, std::shared_ptr<opendnp3::ISOEHandler> soehandler, std::shared_ptr<IChannelListener> listener)
	{
		auto channel = manager.AddTCPClient(
			GetId("client", port).c_str(),
			LEVELS,
			opendnp3::ChannelRetry::Default(),
			"127.0.0.1",
			"127.0.0.1",
			port,
			listener
		);

		return channel->AddMaster(
			GetId("master", port).c_str(),
			soehandler,
			DefaultMasterApplication::Create(),
			GetMasterStackConfig()
		);
	}
	
	IOutstation* StackPair::CreateOutstation(DNP3Manager& manager, uint16_t port, uint16_t numPointsPerType, uint16_t eventBufferSize, std::shared_ptr<IChannelListener> listener)
	{
		auto channel = manager.AddTCPServer(
			GetId("server", port).c_str(),
			LEVELS,
			opendnp3::ChannelRetry::Default(),
			"127.0.0.1",			
			port,
			listener
		);

		return channel->AddOutstation(
			GetId("outstation", port).c_str(),
			opendnp3::SuccessCommandHandler::Create(),
			opendnp3::DefaultOutstationApplication::Create(),
			GetOutstationStackConfig(numPointsPerType, eventBufferSize)
		);
	}

	std::string StackPair::GetId(const char* name, uint16_t port)
	{
		std::ostringstream oss;
		oss << name << ":" << port;
		return oss.str();
	}
}



