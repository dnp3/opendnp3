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
#ifndef __DNP3_MANAGER_H_
#define __DNP3_MANAGER_H_

#include <string>
#include <set>
#include <stdint.h>
#include <memory>
#include <functional>

#include <openpal/Logger.h>
#include <openpal/IPhysicalLayerAsync.h>
#include <openpal/TimeDuration.h>
#include <openpal/IMutex.h>
#include <openpal/IShutdownHandler.h>

#include "opendnp3/link/IOpenDelayStrategy.h"

namespace opendnp3
{

class IChannel;
class DNP3Channel;

class DNP3Manager : private openpal::ITypedShutdownHandler<DNP3Channel*>
{
public:

	DNP3Manager(openpal::IMutex* pMutex = nullptr);	

	IChannel* CreateChannel(	openpal::Logger aLogger,
	                            openpal::TimeDuration minOpenRetry,
	                            openpal::TimeDuration maxOpenRetry,
	                            openpal::IPhysicalLayerAsync* apPhys,
	                            IOpenDelayStrategy* pOpenStrategy = ExponentialBackoffStrategy::Inst());

	/// Asynchronously shutdown all channels
	void BeginShutdown(openpal::IShutdownHandler* pHandler = nullptr);

private:

	bool isShuttingDown;
	openpal::IMutex* pMutex;
	
	openpal::IShutdownHandler* pShutdownHandler;

	std::set<DNP3Channel*> channels;

	void OnShutdown(DNP3Channel* apChannel) override final;

	// returns true if there are no more channels
	bool InitiateShutdown(openpal::IShutdownHandler* pHandler);

	// returns true if there are no more channels and we're shutting down
	bool OnChannelShutdown(DNP3Channel* apChannel);
};

}

#endif
