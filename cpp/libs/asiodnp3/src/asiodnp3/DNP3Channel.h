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
#ifndef ASIODNP3_DNP3CHANNEL_H
#define ASIODNP3_DNP3CHANNEL_H

#include <openpal/logging/LogRoot.h>
#include <openpal/crypto/ICryptoProvider.h>

#include <opendnp3/outstation/OutstationStackConfig.h>
#include <opendnp3/link/LinkChannelStatistics.h>

#include <asiopal/ASIOExecutor.h>
#include <asiopal/Synchronized.h>

#include "IChannel.h"
#include "MultidropTaskLock.h"
#include "StackLifecycle.h"

#include "asiodnp3/impl/LinkLayerRouter.h"
#include <memory>

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

class DNP3Channel : public IChannel, private opendnp3::IChannelStateListener
{
	
public:

	DNP3Channel(
		openpal::LogRoot* pLogRoot_,
		asiopal::ASIOExecutor& executor,		
	    openpal::TimeDuration minOpenRetry,
	    openpal::TimeDuration maxOpenRetry,
		opendnp3::IOpenDelayStrategy& strategy,
		openpal::IPhysicalLayer* pPhys,
		openpal::ICryptoProvider* pCrypto
	);

	// ----------------------- Implement IChannel -----------------------

	virtual opendnp3::LinkChannelStatistics GetChannelStatistics() override final;
	
	void Shutdown() override final;	

	virtual openpal::LogFilters GetLogFilters() const override final;

	virtual void SetLogFilters(const openpal::LogFilters& filters) override final;

	virtual void AddStateListener(const std::function<void(opendnp3::ChannelState)>& listener) override final;

	virtual IMaster* AddMaster(	char const* id,
								opendnp3::ISOEHandler& SOEHandler,								
								opendnp3::IMasterApplication& application,
								const opendnp3::MasterStackConfig& config) override final;

	virtual IMasterSA* AddMaster(	char const* id,
									opendnp3::ISOEHandler& SOEHandler,
									opendnp3::IMasterApplication& application,
									const secauth::MasterAuthStackConfig& config) override final;

	virtual IOutstation* AddOutstation(char const* id,
								opendnp3::ICommandHandler& commandHandler,
								opendnp3::IOutstationApplication& application,
								const opendnp3::OutstationStackConfig& config) override final;

	virtual IOutstationSA* AddOutstation(char const* id,
								opendnp3::ICommandHandler& commandHandler,
								secauth::IOutstationApplicationSA& application,
								const secauth::OutstationAuthStackConfig& config,								
								openpal::IUTCTimeSource& timeSource) override final;

	// -----------------------------------------------------------------------

	// Helper functions only available inside DNP3Manager
	void SetShutdownHandler(const openpal::Action0& action);


private:
	
	// ----- generic method for adding a stack ------
	template <class T>
	T* AddStack(const opendnp3::LinkConfig& link, const std::function<T* ()>& factory);

	void InitiateShutdown(asiopal::Synchronized<bool>& handler);

	virtual void OnStateChange(opendnp3::ChannelState state) override final;		

	void CheckForFinalShutdown();

	MultidropTaskLock taskLock;

	openpal::Action0 shutdownHandler;
	opendnp3::LinkChannelStatistics statistics;
	std::unique_ptr<openpal::IPhysicalLayer> pPhys;
	openpal::ICryptoProvider* pCrypto;
	std::unique_ptr<openpal::LogRoot> pLogRoot;
	asiopal::ASIOExecutor* pExecutor;	
	openpal::Logger logger;
	
	asiopal::Synchronized<bool>* pShutdownHandler;		

	opendnp3::ChannelState channelState;
	std::vector<std::function<void(opendnp3::ChannelState)>> callbacks;
	
	LinkLayerRouter router;
	StackLifecycle stacks;
	
};

}

#endif

