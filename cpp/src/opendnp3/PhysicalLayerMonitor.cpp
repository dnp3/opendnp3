
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the terms of the License.
//

#include "PhysicalLayerMonitor.h"

#include "IPhysicalLayerAsync.h"
#include "PhysicalLayerMonitorStates.h"

#include <openpal/LoggableMacros.h>

#include <functional>

#include <assert.h>

using namespace openpal;
using namespace std::chrono;

namespace opendnp3
{

PhysicalLayerMonitor::PhysicalLayerMonitor(Logger& arLogger, IPhysicalLayerAsync* apPhys, timer_clock::duration aMinOpenRetry, timer_clock::duration aMaxOpenRetry) :
	Loggable(arLogger),
	IHandlerAsync(arLogger),
	mpPhys(apPhys),
	mpOpenTimer(NULL),
	mpState(MonitorStateInit::Inst()),
	mFinalShutdown(false),
	mMinOpenRetry(aMinOpenRetry),
	mMaxOpenRetry(aMaxOpenRetry),
	mCurrentRetry(aMinOpenRetry)
{
	assert(apPhys != NULL);
	mpPhys->SetHandler(this);
}

PhysicalLayerMonitor::~PhysicalLayerMonitor()
{}

ChannelState PhysicalLayerMonitor::GetState()
{
	return mpState->GetState();
}

bool PhysicalLayerMonitor::WaitForShutdown(millis_t aTimeout)
{
	std::unique_lock<std::mutex> lock(mMutex);
	while(!mFinalShutdown) {
		if(aTimeout >= 0) {
			mCondition.wait_for(lock, milliseconds(aTimeout));
			break;
		}
		else mCondition.wait(lock);
	}
	return mFinalShutdown;
}

void PhysicalLayerMonitor::ChangeState(IMonitorState* apState)
{
	LOG_BLOCK(LEV_DEBUG, mpState->ConvertToString() << " -> " << apState->ConvertToString() << " : " << mpPhys->ConvertStateToString());
	IMonitorState* pLast = mpState;

	std::unique_lock<std::mutex> lock(mMutex);
	mpState = apState;
	if(pLast->GetState() != apState->GetState()) {

		this->OnStateChange(mpState->GetState());

		// signaling this way makes sure we're free and clear of the event that causes this
		// before someone else and deletes
		if(mpState->GetState() == CS_SHUTDOWN) mpPhys->GetExecutor()->Post(std::bind(&PhysicalLayerMonitor::DoFinalShutdown, this));
	}
}

void PhysicalLayerMonitor::DoFinalShutdown()
{
	std::unique_lock<std::mutex> lock(mMutex);
	mFinalShutdown = true;
	mCondition.notify_all();
}

/* ------- User facing events that occurs ------- */

void PhysicalLayerMonitor::Start()
{
	LOG_BLOCK(LEV_DEBUG, "Start()");
	mpState->OnStartRequest(this);
}

void PhysicalLayerMonitor::StartOne()
{
	LOG_BLOCK(LEV_DEBUG, "StartOne()");
	mpState->OnStartOneRequest(this);
}

void PhysicalLayerMonitor::Close()
{
	LOG_BLOCK(LEV_DEBUG, "Close()");
	mpState->OnCloseRequest(this);
}

void PhysicalLayerMonitor::Suspend()
{
	LOG_BLOCK(LEV_DEBUG, "Suspend()");
	mpState->OnSuspendRequest(this);
}

void PhysicalLayerMonitor::Shutdown()
{
	LOG_BLOCK(LEV_DEBUG, "Shutdown()");
	mpState->OnShutdownRequest(this);
}

/* ------- External events that occurs ------- */

void PhysicalLayerMonitor::OnOpenTimerExpiration()
{
	LOG_BLOCK(LEV_DEBUG, "OnOpenTimerExpiration()");
	assert(mpOpenTimer != NULL);
	mpOpenTimer = NULL;
	mpState->OnOpenTimeout(this);
}

void PhysicalLayerMonitor::_OnOpenFailure()
{
	LOG_BLOCK(LEV_DEBUG, "_OnOpenFailure()");
	mpState->OnOpenFailure(this);
	this->OnPhysicalLayerOpenFailureCallback();
	this->mCurrentRetry = std::min(2 * mCurrentRetry, mMaxOpenRetry);
}

void PhysicalLayerMonitor::_OnLowerLayerUp()
{
	LOG_BLOCK(LEV_DEBUG, "_OnLowerLayerUp");
	this->mCurrentRetry = mMinOpenRetry;
	mpState->OnLayerOpen(this);
	this->OnPhysicalLayerOpenSuccessCallback();
}

void PhysicalLayerMonitor::_OnLowerLayerDown()
{
	LOG_BLOCK(LEV_DEBUG, "_OnLowerLayerDown");
	mpState->OnLayerClose(this);
	this->OnPhysicalLayerCloseCallback();
}

/* ------- Actions for the states ------- */

void PhysicalLayerMonitor::StartOpenTimer()
{
	assert(mpOpenTimer == NULL);
	mpOpenTimer = mpPhys->GetExecutor()->Start(mCurrentRetry, std::bind(&PhysicalLayerMonitor::OnOpenTimerExpiration, this));
}

void PhysicalLayerMonitor::CancelOpenTimer()
{
	assert(mpOpenTimer != NULL);
	mpOpenTimer->Cancel();
	mpOpenTimer = NULL;
}

/* ------- Internal helper functions ------- */



}
