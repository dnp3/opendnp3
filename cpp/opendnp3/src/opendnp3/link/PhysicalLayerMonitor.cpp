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
    Logger aLogger,
    IPhysicalLayerAsync* pPhys_,
    TimeDuration minOpenRetry_,
    TimeDuration maxOpenRetry_,
    IOpenDelayStrategy* pOpenStrategy_) :
	Loggable(aLogger),
	IHandlerAsync(aLogger),
	mpPhys(pPhys_),
	mpOpenTimer(nullptr),
	mpState(MonitorStateInit::Inst()),
	mFinalShutdown(false),
	minOpenRetry(minOpenRetry_),
	maxOpenRetry(maxOpenRetry_),
	currentRetry(minOpenRetry_),
	pOpenStrategy(pOpenStrategy_)
{
	assert(mpPhys != nullptr);
	mpPhys->SetHandler(this);
}

PhysicalLayerMonitor::~PhysicalLayerMonitor()
{}

ChannelState PhysicalLayerMonitor::GetState()
{
	return mpState->GetState();
}

void PhysicalLayerMonitor::ChangeState(IMonitorState* apState)
{
	LOG_BLOCK(LogLevel::Debug, mpState->ConvertToString() << " -> " << apState->ConvertToString() << " : " << mpPhys->ConvertStateToString());
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

void PhysicalLayerMonitor::_OnOpenFailure()
{
	LOG_BLOCK(LogLevel::Debug, "_OnOpenFailure()");
	mpState->OnOpenFailure(this);
	this->OnPhysicalLayerOpenFailureCallback();
	this->currentRetry = pOpenStrategy->GetNextDelay(currentRetry, maxOpenRetry);
}

void PhysicalLayerMonitor::_OnLowerLayerUp()
{
	LOG_BLOCK(LogLevel::Debug, "_OnLowerLayerUp");
	this->currentRetry = minOpenRetry;
	mpState->OnLayerOpen(this);
	this->OnPhysicalLayerOpenSuccessCallback();
}

void PhysicalLayerMonitor::_OnLowerLayerDown()
{
	LOG_BLOCK(LogLevel::Debug, "_OnLowerLayerDown");
	mpState->OnLayerClose(this);
	this->OnPhysicalLayerCloseCallback();
}

/* ------- Actions for the states ------- */

void PhysicalLayerMonitor::StartOpenTimer()
{
	assert(mpOpenTimer == nullptr);
	mpOpenTimer = mpPhys->GetExecutor()->Start(currentRetry, std::bind(&PhysicalLayerMonitor::OnOpenTimerExpiration, this));
}

void PhysicalLayerMonitor::CancelOpenTimer()
{
	assert(mpOpenTimer != nullptr);
	mpOpenTimer->Cancel();
	mpOpenTimer = nullptr;
}

/* ------- Internal helper functions ------- */



}
