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
#include "PhysicalLayerMonitor.h"

#include "PhysicalLayerMonitorStates.h"

#include <openpal/IPhysicalLayerAsync.h>
#include <openpal/LoggableMacros.h>

#include <functional>

#include <assert.h>

using namespace openpal;

namespace opendnp3
{

PhysicalLayerMonitor::PhysicalLayerMonitor(
    const Logger& logger,
    IPhysicalLayerAsync* pPhys_,
    TimeDuration minOpenRetry_,
    TimeDuration maxOpenRetry_,
    IOpenDelayStrategy* pOpenStrategy_) :
		Loggable(logger),	
		pPhys(pPhys_),
		isOnline(false),
		mpOpenTimer(nullptr),
		mpState(MonitorStateInit::Inst()),
		mFinalShutdown(false),
		minOpenRetry(minOpenRetry_),
		maxOpenRetry(maxOpenRetry_),
		currentRetry(minOpenRetry_),
		pOpenStrategy(pOpenStrategy_)
{
	assert(pPhys != nullptr);
	pPhys->SetHandler(this);
}

PhysicalLayerMonitor::~PhysicalLayerMonitor()
{}

ChannelState PhysicalLayerMonitor::GetState()
{
	return mpState->GetState();
}

void PhysicalLayerMonitor::ChangeState(IMonitorState* apState)
{
	LOG_BLOCK(LogLevel::Debug, mpState->ConvertToString() << " -> " << apState->ConvertToString() << " : " << pPhys->ConvertStateToString());
	IMonitorState* pLast = mpState;	
	mpState = apState;

	if(pLast->GetState() != apState->GetState())
	{
		this->OnStateChange(mpState->GetState());		
	}

	if (mpState->GetState() == ChannelState::SHUTDOWN)
	{
		this->OnShutdown();
	}
}


/// ------- IHandlerAsync -------

void PhysicalLayerMonitor::OnOpenFailure()
{
	LOG_BLOCK(LogLevel::Debug, "OnOpenFailure()");
	mpState->OnOpenFailure(this);
	this->OnPhysicalLayerOpenFailureCallback();
	this->currentRetry = pOpenStrategy->GetNextDelay(currentRetry, maxOpenRetry);
}

void PhysicalLayerMonitor::OnLowerLayerUp()
{
	if (!isOnline)
	{
		isOnline = true;
		LOG_BLOCK(LogLevel::Debug, "OnLowerLayerUp");
		this->currentRetry = minOpenRetry;
		mpState->OnLayerOpen(this);
		this->OnPhysicalLayerOpenSuccessCallback();
	}
	else
	{
		LOG_BLOCK(LogLevel::Error, "Monitor is already online");
	}
}

void PhysicalLayerMonitor::OnLowerLayerDown()
{
	if (isOnline)
	{
		isOnline = false;
		LOG_BLOCK(LogLevel::Debug, "OnLowerLayerDown");
		mpState->OnLayerClose(this);
		this->OnPhysicalLayerCloseCallback();
	}
	else
	{
		LOG_BLOCK(LogLevel::Error, "Monitor is not online");
	}
}

/* ------- User facing events that occurs ------- */

void PhysicalLayerMonitor::Start()
{
	LOG_BLOCK(LogLevel::Debug, "Start()");
	mpState->OnStartRequest(this);
}

void PhysicalLayerMonitor::StartOne()
{
	LOG_BLOCK(LogLevel::Debug, "StartOne()");
	mpState->OnStartOneRequest(this);
}

void PhysicalLayerMonitor::Close()
{
	LOG_BLOCK(LogLevel::Debug, "Close()");
	mpState->OnCloseRequest(this);
}

void PhysicalLayerMonitor::Suspend()
{
	LOG_BLOCK(LogLevel::Debug, "Suspend()");
	mpState->OnSuspendRequest(this);
}

void PhysicalLayerMonitor::Shutdown()
{
	LOG_BLOCK(LogLevel::Debug, "Shutdown()");
	mpState->OnShutdownRequest(this);
}

/* ------- External events that occurs ------- */

void PhysicalLayerMonitor::OnOpenTimerExpiration()
{
	LOG_BLOCK(LogLevel::Debug, "OnOpenTimerExpiration()");
	assert(mpOpenTimer != nullptr);
	mpOpenTimer = nullptr;
	mpState->OnOpenTimeout(this);
}

/* ------- Actions for the states ------- */

void PhysicalLayerMonitor::StartOpenTimer()
{
	assert(mpOpenTimer == nullptr);
	mpOpenTimer = pPhys->GetExecutor()->Start(currentRetry, std::bind(&PhysicalLayerMonitor::OnOpenTimerExpiration, this));
}

void PhysicalLayerMonitor::CancelOpenTimer()
{
	assert(mpOpenTimer != nullptr);
	mpOpenTimer->Cancel();
	mpOpenTimer = nullptr;
}

/* ------- Internal helper functions ------- */



}
