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
#include "PhysicalLayerMonitorStates.h"

#include <openpal/channel/IPhysicalLayer.h>
#include <openpal/logging/LogMacros.h>

#include "opendnp3/LogLevels.h"

using namespace openpal;
using namespace opendnp3;

namespace asiodnp3
{

/* --- MonitorStateActions --- */

void MonitorStateActions::ChangeState(PhysicalLayerMonitor* apContext, IMonitorState& state)
{
	apContext->ChangeState(state);
}

void MonitorStateActions::StartOpenTimer(PhysicalLayerMonitor* apContext)
{
	apContext->StartOpenTimer();
}

void MonitorStateActions::CancelOpenTimer(PhysicalLayerMonitor* apContext)
{
	apContext->CancelOpenTimer();
}

void MonitorStateActions::Close(PhysicalLayerMonitor* apContext)
{
	apContext->pPhys->BeginClose();
}

void MonitorStateActions::Open(PhysicalLayerMonitor* apContext)
{
	apContext->pPhys->BeginOpen();
}

/* --- ExceptsOnLayerOpen --- */

bool CannotOpen::OnLayerOpen(PhysicalLayerMonitor* apContext)
{
	FORMAT_LOG_BLOCK(apContext->GetLogger(), flags::ERR, "Invalid action for state: %s", this->Name());
	return false;
}

/* --- NotOpening --- */

bool NotOpening::OnOpenFailure(PhysicalLayerMonitor* apContext)
{
	FORMAT_LOG_BLOCK(apContext->GetLogger(), flags::ERR, "Invalid action for state: %s", this->Name());
	return false;
}

/* --- NotOpen --- */

bool NotOpen::OnLayerClose(PhysicalLayerMonitor* apContext)
{
	FORMAT_LOG_BLOCK(apContext->GetLogger(), flags::ERR, "Invalid action for state: %s", this->Name());
	return false;
}

/* --- NotWaitingForTimer --- */

bool NotWaitingForTimer::OnOpenTimeout(PhysicalLayerMonitor* apContext)
{
	FORMAT_LOG_BLOCK(apContext->GetLogger(), flags::ERR, "Invalid action for state: %s", this->Name());
	return false;
}

/* --- IgnoresClose --- */

void IgnoresClose::OnCloseRequest(PhysicalLayerMonitor* apContext)
{
	
}

/* --- IgnoresSuspend --- */

void IgnoresSuspend::OnSuspendRequest(PhysicalLayerMonitor* apContext)
{
	
}

/* --- StartsOnClose --- */

bool StartsOnClose::OnLayerClose(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateWaiting::Instance());
	MonitorStateActions::StartOpenTimer(apContext);
	return true;
}

/* --- IgnoresShutdown --- */

void IgnoresShutdown::OnShutdownRequest(PhysicalLayerMonitor* apContext)
{
	
}

/* --- IgnoresStart --- */

void IgnoresStart::OnStartRequest(PhysicalLayerMonitor* apContext)
{
	
}

/* --- IgnoresStartOne --- */

void IgnoresStartOne::OnStartOneRequest(PhysicalLayerMonitor* apContext)
{
	
}

/* --- OpenFailureCausesWait --- */

bool OpenFailureCausesWait::OnOpenFailure(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateWaiting::Instance());
	MonitorStateActions::StartOpenTimer(apContext);
	return true;
}

/* --- WaitingBase --- */

void MonitorStateWaitingBase::OnSuspendRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::CancelOpenTimer(apContext);
	MonitorStateActions::ChangeState(apContext, MonitorStateSuspended::Instance());
}

void MonitorStateWaitingBase::OnShutdownRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::CancelOpenTimer(apContext);
	MonitorStateActions::ChangeState(apContext, MonitorStateShutdown::Instance());
}

/* ----------------- Concrete ----------------- */


/* --- Shutdown --- */

MonitorStateShutdown MonitorStateShutdown::instance;

/* --- Suspended --- */

MonitorStateSuspended MonitorStateSuspended::instance;

/* --- Init --- */

MonitorStateInit MonitorStateInit::instance;

/* ---- SuspendedBase --- */
void MonitorStateSuspendedBase::OnStartRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateOpening::Instance());
	MonitorStateActions::Open(apContext);
}

void MonitorStateSuspendedBase::OnStartOneRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateOpeningOne::Instance());
	MonitorStateActions::Open(apContext);
}

void MonitorStateSuspendedBase::OnShutdownRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateShutdown::Instance());
}

/* --- OpeningBase --- */

void MonitorStateOpeningBase::OnShutdownRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateOpeningStopping::Instance());
	MonitorStateActions::Close(apContext);
}

void MonitorStateOpeningBase::OnSuspendRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateOpeningSuspending::Instance());
	MonitorStateActions::Close(apContext);
}

/* --- Opening --- */

MonitorStateOpening MonitorStateOpening::instance;

void MonitorStateOpening::OnStartOneRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateOpeningOne::Instance());
}

void MonitorStateOpening::OnCloseRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateOpeningClosing::Instance());
	MonitorStateActions::Close(apContext);
}

bool MonitorStateOpening::OnLayerOpen(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateOpen::Instance());
	return true;
}

/* --- OpeningOne --- */

MonitorStateOpeningOne MonitorStateOpeningOne::instance;

bool MonitorStateOpeningOne::OnOpenFailure(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateSuspended::Instance());
	return true;
}

void MonitorStateOpeningOne::OnStartRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateOpening::Instance());
}

void MonitorStateOpeningOne::OnCloseRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateOpeningSuspending::Instance());
	MonitorStateActions::Close(apContext);
}

bool MonitorStateOpeningOne::OnLayerOpen(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateOpenOne::Instance());
	return true;
}

/* --- OpeningClosing --- */

MonitorStateOpeningClosing MonitorStateOpeningClosing::instance;

void MonitorStateOpeningClosing::OnStartOneRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateOpeningSuspending::Instance());
}

void MonitorStateOpeningClosing::OnShutdownRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateOpeningStopping::Instance());
}

void MonitorStateOpeningClosing::OnSuspendRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateOpeningSuspending::Instance());
}

/* --- OpeningStopping --- */

MonitorStateOpeningStopping MonitorStateOpeningStopping::instance;

/* --- OpeningSuspending --- */

MonitorStateOpeningSuspending MonitorStateOpeningSuspending::instance;

void MonitorStateOpeningSuspending::OnStartRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateOpeningClosing::Instance());
}

void MonitorStateOpeningSuspending::OnShutdownRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateOpeningStopping::Instance());
}

/* --- Open --- */

MonitorStateOpen MonitorStateOpen::instance;

void MonitorStateOpen::OnStartOneRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateOpenOne::Instance());
}

void MonitorStateOpen::OnCloseRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateClosing::Instance());
	MonitorStateActions::Close(apContext);
}

void MonitorStateOpen::OnSuspendRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateSuspending::Instance());
	MonitorStateActions::Close(apContext);
}

void MonitorStateOpen::OnShutdownRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateShutingDown::Instance());
	MonitorStateActions::Close(apContext);
}

/* --- OpenOne --- */

MonitorStateOpenOne MonitorStateOpenOne::instance;

void MonitorStateOpenOne::OnStartRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateOpen::Instance());
}

void MonitorStateOpenOne::OnCloseRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateSuspending::Instance());
	MonitorStateActions::Close(apContext);
}

void MonitorStateOpenOne::OnSuspendRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateSuspending::Instance());
	MonitorStateActions::Close(apContext);
}

void MonitorStateOpenOne::OnShutdownRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateShutingDown::Instance());
	MonitorStateActions::Close(apContext);
}

bool MonitorStateOpenOne::OnLayerClose(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateSuspended::Instance());
	return true;
}

/* --- Waiting --- */

MonitorStateWaiting MonitorStateWaiting::instance;

void MonitorStateWaiting::OnStartOneRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateWaitingOne::Instance());
}

bool MonitorStateWaiting::OnOpenTimeout(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::Open(apContext);
	MonitorStateActions::ChangeState(apContext, MonitorStateOpening::Instance());
	return true;
}

/* --- WaitingOne --- */

MonitorStateWaitingOne MonitorStateWaitingOne::instance;

void MonitorStateWaitingOne::OnStartRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateWaiting::Instance());
}

bool MonitorStateWaitingOne::OnOpenTimeout(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::Open(apContext);
	MonitorStateActions::ChangeState(apContext, MonitorStateOpeningOne::Instance());
	return true;
}

/* --- Closing --- */

MonitorStateClosing MonitorStateClosing::instance;

void MonitorStateClosing::OnStartOneRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateSuspending::Instance());
}

void MonitorStateClosing::OnShutdownRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateShutingDown::Instance());
}

void MonitorStateClosing::OnSuspendRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateSuspending::Instance());
}

/* --- Suspending --- */

MonitorStateSuspending MonitorStateSuspending::instance;

bool MonitorStateSuspending::OnLayerClose(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateSuspended::Instance());
	return true;
}

void MonitorStateSuspending::OnStartRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateClosing::Instance());
}

void MonitorStateSuspending::OnShutdownRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateShutingDown::Instance());
}

/* --- Stopping --- */

MonitorStateShutingDown MonitorStateShutingDown::instance;

bool MonitorStateShutingDown::OnLayerClose(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateShutdown::Instance());
	return true;
}

}
