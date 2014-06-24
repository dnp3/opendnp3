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
#ifndef __DNP3_CHANNEL_H_
#define __DNP3_CHANNEL_H_

#include <openpal/logging/LogRoot.h>

#include <opendnp3/outstation/OutstationStackConfig.h>
#include <opendnp3/link/LinkLayerRouter.h>
#include <opendnp3/link/LinkChannelStatistics.h>

#include "IChannel.h"
#include "IShutdownHandler.h"

#include <asiopal/ASIOExecutor.h>

#include <memory>
#include <set>

namespace openpal
{
	class IPhysicalLayer;
}

namespace opendnp3
{
	class ICommandHandler;
	class ITimeWriteHandler;
}


namespace asiodnp3
{

class IStack;
class IOutstation;

class DNP3Channel : public IChannel, private ITypedShutdownHandler<IStack*>, private opendnp3::IChannelStateListener
{
	enum class State
	{
	    READY,
	    SHUTTING_DOWN,
	    SHUTDOWN
	};

public:

	DNP3Channel(
		openpal::LogRoot* pLogRoot_,
		asiopal::ASIOExecutor& executor,
	    openpal::TimeDuration minOpenRetry,
	    openpal::TimeDuration maxOpenRetry,
	    opendnp3::IOpenDelayStrategy* pStrategy,
		openpal::IPhysicalLayer* pPhys_,
	    ITypedShutdownHandler<DNP3Channel*>* pShutdownHandler_		
	);

	virtual opendnp3::LinkChannelStatistics GetChannelStatistics() override final;

	// public interface, callable only from outside
	void BeginShutdown() override final;

	openpal::IExecutor* GetExecutor();

	virtual openpal::LogFilters GetLogFilters() const override final;

	virtual void SetLogFilters(const openpal::LogFilters& filters) override final;

	virtual void AddStateChangeCallback(const StateChangeCallback& callback) override final;

	virtual IMaster* AddMaster(	char const* id,
								opendnp3::ISOEHandler* pPublisher,
								openpal::IUTCTimeSource* pTimeSource,
								const opendnp3::MasterStackConfig& config) override final;

	virtual IOutstation* AddOutstation(char const* id,
								opendnp3::ICommandHandler* pCmdHandler,
								opendnp3::ITimeWriteHandler* pTimeWriteHandler,
								const opendnp3::OutstationStackConfig& config) override final;

	// Helper functions only available inside DNP3Manager

private:

	IMaster* _AddMaster(char const* id,
		opendnp3::ISOEHandler* pPublisher,
		openpal::IUTCTimeSource* pTimeSource,
		const opendnp3::MasterStackConfig& config);

	IOutstation* _AddOutstation(char const* id,
		opendnp3::ICommandHandler* pCmdHandler,
		opendnp3::ITimeWriteHandler* pTimeWriteHandler,
		const opendnp3::OutstationStackConfig& config);

	void InitiateShutdown();

	virtual void OnStateChange(opendnp3::ChannelState state) override final;	

	// shutdown from the stack
	void OnShutdown(IStack* apStack) override final;

	void CheckForFinalShutdown();

	opendnp3::LinkChannelStatistics statistics;
	std::unique_ptr<openpal::IPhysicalLayer> pPhys;
	std::unique_ptr<openpal::LogRoot> pLogRoot;
	asiopal::ASIOExecutor* pExecutor;
	openpal::Logger logger;

	State state;
	ITypedShutdownHandler<DNP3Channel*>* pShutdownHandler;
		
	std::set<IStack*> stacks;

	opendnp3::ChannelState channelState;
	std::vector<StateChangeCallback> callbacks;
	
	opendnp3::LinkLayerRouter router;

	
};

}

#endif

