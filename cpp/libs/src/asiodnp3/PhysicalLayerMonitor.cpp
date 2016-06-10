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
#include "PhysicalLayerMonitor.h"

#include "PhysicalLayerMonitorStates.h"

#include "opendnp3/LogLevels.h"

#include <openpal/channel/IPhysicalLayer.h>
#include <openpal/logging/LogMacros.h>

#include <assert.h>

using namespace openpal;
using namespace opendnp3;

namespace asiodnp3
{

PhysicalLayerMonitor::PhysicalLayerMonitor(
    openpal::Logger logger_,
    openpal::IExecutor& executor,
    IPhysicalLayer* pPhys_,
    const opendnp3::ChannelRetry& retry_
) :
	logger(logger_),
	pPhys(pPhys_),
	pExecutor(&executor),
	isOnline(false),
	mpOpenTimer(nullptr),
	mpState(&MonitorStateInit::Instance()),
	mFinalShutdown(false),
	retry(retry_),
	currentRetry(retry_.minOpenRetry)
{
	assert(pPhys != nullptr);
	pPhys->SetHandler(this);
}

ChannelState PhysicalLayerMonitor::GetState()
{
	return mpState->GetState();
}

void PhysicalLayerMonitor::ChangeState(IMonitorState& state)
{
	FORMAT_LOG_BLOCK(logger, flags::DBG, "%s -> %s", mpState->Name(), state.Name());
	IMonitorState* pLast = mpState;
	mpState = &state;

	if(pLast->GetState() != state.GetState())
	{
		this->OnStateChange(mpState->GetState());
	}

	if (mpState->GetState() == ChannelState::SHUTDOWN)
	{
		this->OnShutdown();
	}
}


/// ------- IHandler -------

void PhysicalLayerMonitor::OnOpenFailure()
{
	if (mpState->OnOpenFailure(*this))
	{
		this->OnPhysicalLayerOpenFailureCallback();
		this->currentRetry = retry.strategy.GetNextDelay(currentRetry, retry.maxOpenRetry);
	}
}

void PhysicalLayerMonitor::OnLowerLayerUp()
{
	if (mpState->OnLayerOpen(*this))
	{
		isOnline = true;
		this->currentRetry = retry.minOpenRetry;
		this->OnPhysicalLayerOpenSuccessCallback();
	}
}

void PhysicalLayerMonitor::OnLowerLayerDown()
{
	if (mpState->OnLayerClose(*this))
	{
		isOnline = false;
		this->OnPhysicalLayerCloseCallback();
	}
}

/* ------- User facing events that occurs ------- */

void PhysicalLayerMonitor::Start()
{
	mpState->OnStartRequest(*this);
}

void PhysicalLayerMonitor::StartOne()
{
	mpState->OnStartOneRequest(*this);
}

void PhysicalLayerMonitor::Close()
{
	mpState->OnCloseRequest(*this);
}

void PhysicalLayerMonitor::Suspend()
{
	mpState->OnSuspendRequest(*this);
}

void PhysicalLayerMonitor::Shutdown()
{
	mpState->OnShutdownRequest(*this);
}

/* ------- External events that occurs ------- */

void PhysicalLayerMonitor::OnOpenTimerExpiration()
{
	if (mpState->OnOpenTimeout(*this))
	{
		assert(mpOpenTimer != nullptr);
		mpOpenTimer = nullptr;
	}
}

/* ------- Actions for the states ------- */

void PhysicalLayerMonitor::StartOpenTimer()
{
	assert(mpOpenTimer == nullptr);
	auto lambda = [this]()
	{
		this->OnOpenTimerExpiration();
	};
	mpOpenTimer = pExecutor->Start(currentRetry, Action0::Bind(lambda));
}

void PhysicalLayerMonitor::CancelOpenTimer()
{
	assert(mpOpenTimer != nullptr);
	mpOpenTimer->Cancel();
	mpOpenTimer = nullptr;
}

}
