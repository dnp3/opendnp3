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
#ifndef ASIODNP3_DNP3CHANNEL_H
#define ASIODNP3_DNP3CHANNEL_H

#include <openpal/logging/Logger.h>

#include <opendnp3/link/LinkChannelStatistics.h>
#include <opendnp3/link/ChannelRetry.h>
#include <opendnp3/outstation/ICommandHandler.h>

#include <asiopal/ASIOExecutor.h>
#include <asiopal/Synchronized.h>
#include <asiopal/PhysicalLayerASIO.h>

#include "asiodnp3/IChannel.h"
#include "asiodnp3/StackLifecycle.h"
#include "asiodnp3/LinkLayerRouter.h"
#include "asiodnp3/IChannelListener.h"
#include "asiodnp3/OutstationStackConfig.h"

#include <memory>

namespace asiodnp3
{

class IStack;
class IOutstation;

class DNP3Channel : public IChannel
{

public:

	DNP3Channel(
		const openpal::Logger& logger,
	    const opendnp3::ChannelRetry& retry,
	    std::shared_ptr<IChannelListener> listener,
	    std::unique_ptr<asiopal::PhysicalLayerASIO> phys
	);

	// ----------------------- Implement IChannel -----------------------

	virtual opendnp3::LinkChannelStatistics GetChannelStatistics() override final;

	void Shutdown() override final;

	virtual openpal::LogFilters GetLogFilters() const override final;

	virtual void SetLogFilters(const openpal::LogFilters& filters) override final;

	virtual IMaster* AddMaster(char const* id,
	                           std::shared_ptr<opendnp3::ISOEHandler> SOEHandler,
	                           std::shared_ptr<opendnp3::IMasterApplication> application,
	                           const MasterStackConfig& config) override final;



	virtual IOutstation* AddOutstation(char const* id,
	                                   std::shared_ptr<opendnp3::ICommandHandler> commandHandler,
	                                   std::shared_ptr<opendnp3::IOutstationApplication> application,
	                                   const OutstationStackConfig& config) override final;

	// -----------------------------------------------------------------------

	// Helper functions only available inside DNP3Manager
	void SetShutdownHandler(const openpal::action_t& action);


private:

	// ----- generic method for adding a stack ------
	template <class T>
	T* AddStack(const opendnp3::LinkConfig& link, const std::function<T* ()>& factory);

	void InitiateShutdown(asiopal::Synchronized<bool>& handler);

	void CheckForFinalShutdown();

	openpal::action_t shutdownHandler;
	opendnp3::LinkChannelStatistics statistics;

	std::unique_ptr<asiopal::PhysicalLayerASIO> phys;
	openpal::Logger logger;

	asiopal::Synchronized<bool>* pShutdownHandler;

	LinkLayerRouter router;
	StackLifecycle stacks;

};

}

#endif
