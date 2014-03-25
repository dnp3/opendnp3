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


#include <openpal/Location.h>
#include <openpal/IPhysicalLayerAsync.h>
#include <openpal/LoggableMacros.h>

#include "opendnp3/LogLevels.h"

#include <sstream>

using namespace openpal;

namespace opendnp3
{

/* --- MonitorStateActions --- */

void MonitorStateActions::ChangeState(PhysicalLayerMonitor* apContext, IMonitorState* apState)
{
	apContext->ChangeState(apState);
}

void MonitorStateActions::StartOpenTimer(PhysicalLayerMonitor* apContext)
{
	apContext->StartOpenTimer();
}

void MonitorStateActions::CancelOpenTimer(PhysicalLayerMonitor* apContext)
{
	apContext->CancelOpenTimer();
}

void MonitorStateActions::AsyncClose(PhysicalLayerMonitor* apContext)
{
	apContext->pPhys->AsyncClose();
}

void MonitorStateActions::AsyncOpen(PhysicalLayerMonitor* apContext)
{
	apContext->pPhys->AsyncOpen();
}

/* --- IMonitorState --- */

std::string IMonitorState::ConvertToString()
{
	std::ostringstream oss;
	oss << this->Name() << "(" << ChannelStateToString(this->GetState()) << ")";
	return oss.str();
}

/* --- ExceptsOnLayerOpen --- */

bool CannotOpen::OnLayerOpen(PhysicalLayerMonitor* apContext)
{
	LOGGER_BLOCK(apContext->GetLogger(), flags::ERR, "Invalid action for state: " << this->Name());
	return false;
}

/* --- NotOpening --- */

bool NotOpening::OnOpenFailure(PhysicalLayerMonitor* apContext)
{
	LOGGER_BLOCK(apContext->GetLogger(), flags::ERR, "Invalid action for state: " << this->Name());
	return false;
}

/* --- NotOpen --- */

bool NotOpen::OnLayerClose(PhysicalLayerMonitor* apContext)
{
	LOGGER_BLOCK(apContext->GetLogger(), flags::ERR, "Invalid action for state: " << this->Name());
	return false;
}

/* --- NotWaitingForTimer --- */

bool NotWaitingForTimer::OnOpenTimeout(PhysicalLayerMonitor* apContext)
{
	LOGGER_BLOCK(apContext->GetLogger(), flags::ERR, "Invalid action for state: " << this->Name());
	return false;
}

/* --- IgnoresClose --- */

void IgnoresClose::OnCloseRequest(PhysicalLayerMonitor* apContext)
{
	LOGGER_BLOCK(apContext->GetLogger(), flags::DEBUG, "Ignoring Close(): " << this->Name());
}

/* --- IgnoresSuspend --- */

void IgnoresSuspend::OnSuspendRequest(PhysicalLayerMonitor* apContext)
{
	LOGGER_BLOCK(apContext->GetLogger(), flags::DEBUG, "Ignoring Suspend(): " << this->Name());
}

/* --- StartsOnClose --- */

bool StartsOnClose::OnLayerClose(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateWaiting::Inst());
	MonitorStateActions::StartOpenTimer(apContext);
	return true;
}

/* --- IgnoresShutdown --- */

void IgnoresShutdown::OnShutdownRequest(PhysicalLayerMonitor* apContext)
{
	LOGGER_BLOCK(apContext->GetLogger(), flags::DEBUG, "Ignoring Stop(): " << this->Name());
}

/* --- IgnoresStart --- */

void IgnoresStart::OnStartRequest(PhysicalLayerMonitor* apContext)
{
	LOGGER_BLOCK(apContext->GetLogger(), flags::DEBUG, "Ignoring Start(): " << this->Name());
}

/* --- IgnoresStartOne --- */

void IgnoresStartOne::OnStartOneRequest(PhysicalLayerMonitor* apContext)
{
	LOGGER_BLOCK(apContext->GetLogger(), flags::DEBUG, "Ignoring StartOne(): " << this->Name());
}

/* --- OpenFailureCausesWait --- */

bool OpenFailureCausesWait::OnOpenFailure(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateWaiting::Inst());
	MonitorStateActions::StartOpenTimer(apContext);
	return true;
}

/* --- WaitingBase --- */

void MonitorStateWaitingBase::OnSuspendRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::CancelOpenTimer(apContext);
	MonitorStateActions::ChangeState(apContext, MonitorStateSuspended::Inst());
}

void MonitorStateWaitingBase::OnShutdownRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::CancelOpenTimer(apContext);
	MonitorStateActions::ChangeState(apContext, MonitorStateShutdown::Inst());
}

/* ----------------- Concrete ----------------- */


/* --- Shutdown --- */

MonitorStateShutdown MonitorStateShutdown::mInstance;

/* --- Suspended --- */

MonitorStateSuspended MonitorStateSuspended::mInstance;

/* --- Init --- */

MonitorStateInit MonitorStateInit::mInstance;

/* ---- SuspendedBase --- */
void MonitorStateSuspendedBase::OnStartRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateOpening::Inst());
	MonitorStateActions::AsyncOpen(apContext);
}

void MonitorStateSuspendedBase::OnStartOneRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateOpeningOne::Inst());
	MonitorStateActions::AsyncOpen(apContext);
}

void MonitorStateSuspendedBase::OnShutdownRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateShutdown::Inst());
}

/* --- OpeningBase --- */

void MonitorStateOpeningBase::OnShutdownRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateOpeningStopping::Inst());
	MonitorStateActions::AsyncClose(apContext);
}

void MonitorStateOpeningBase::OnSuspendRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateOpeningSuspending::Inst());
	MonitorStateActions::AsyncClose(apContext);
}

/* --- Opening --- */

MonitorStateOpening MonitorStateOpening::mInstance;

void MonitorStateOpening::OnStartOneRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateOpeningOne::Inst());
}

void MonitorStateOpening::OnCloseRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateOpeningClosing::Inst());
	MonitorStateActions::AsyncClose(apContext);
}

bool MonitorStateOpening::OnLayerOpen(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateOpen::Inst());
	return true;
}

/* --- OpeningOne --- */

MonitorStateOpeningOne MonitorStateOpeningOne::mInstance;

bool MonitorStateOpeningOne::OnOpenFailure(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateSuspended::Inst());
	return true;
}

void MonitorStateOpeningOne::OnStartRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateOpening::Inst());
}

void MonitorStateOpeningOne::OnCloseRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateOpeningSuspending::Inst());
	MonitorStateActions::AsyncClose(apContext);
}

bool MonitorStateOpeningOne::OnLayerOpen(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateOpenOne::Inst());
	return true;
}

/* --- OpeningClosing --- */

MonitorStateOpeningClosing MonitorStateOpeningClosing::mInstance;

void MonitorStateOpeningClosing::OnStartOneRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateOpeningSuspending::Inst());
}

void MonitorStateOpeningClosing::OnShutdownRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateOpeningStopping::Inst());
}

void MonitorStateOpeningClosing::OnSuspendRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateOpeningSuspending::Inst());
}

/* --- OpeningStopping --- */

MonitorStateOpeningStopping MonitorStateOpeningStopping::mInstance;

/* --- OpeningSuspending --- */

MonitorStateOpeningSuspending MonitorStateOpeningSuspending::mInstance;

void MonitorStateOpeningSuspending::OnStartRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateOpeningClosing::Inst());
}

void MonitorStateOpeningSuspending::OnShutdownRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateOpeningStopping::Inst());
}

/* --- Open --- */

MonitorStateOpen MonitorStateOpen::mInstance;

void MonitorStateOpen::OnStartOneRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateOpenOne::Inst());
}

void MonitorStateOpen::OnCloseRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateClosing::Inst());
	MonitorStateActions::AsyncClose(apContext);
}

void MonitorStateOpen::OnSuspendRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateSuspending::Inst());
	MonitorStateActions::AsyncClose(apContext);
}

void MonitorStateOpen::OnShutdownRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateShutingDown::Inst());
	MonitorStateActions::AsyncClose(apContext);
}

/* --- OpenOne --- */

MonitorStateOpenOne MonitorStateOpenOne::mInstance;

void MonitorStateOpenOne::OnStartRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateOpen::Inst());
}

void MonitorStateOpenOne::OnCloseRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateSuspending::Inst());
	MonitorStateActions::AsyncClose(apContext);
}

void MonitorStateOpenOne::OnSuspendRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateSuspending::Inst());
	MonitorStateActions::AsyncClose(apContext);
}

void MonitorStateOpenOne::OnShutdownRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateShutingDown::Inst());
	MonitorStateActions::AsyncClose(apContext);
}

bool MonitorStateOpenOne::OnLayerClose(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateSuspended::Inst());
	return true;
}

/* --- Waiting --- */

MonitorStateWaiting MonitorStateWaiting::mInstance;

void MonitorStateWaiting::OnStartOneRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateWaitingOne::Inst());
}

bool MonitorStateWaiting::OnOpenTimeout(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::AsyncOpen(apContext);
	MonitorStateActions::ChangeState(apContext, MonitorStateOpening::Inst());
	return true;
}

/* --- WaitingOne --- */

MonitorStateWaitingOne MonitorStateWaitingOne::mInstance;

void MonitorStateWaitingOne::OnStartRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateWaiting::Inst());
}

bool MonitorStateWaitingOne::OnOpenTimeout(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::AsyncOpen(apContext);
	MonitorStateActions::ChangeState(apContext, MonitorStateOpeningOne::Inst());
	return true;
}

/* --- Closing --- */

MonitorStateClosing MonitorStateClosing::mInstance;

void MonitorStateClosing::OnStartOneRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateSuspending::Inst());
}

void MonitorStateClosing::OnShutdownRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateShutingDown::Inst());
}

void MonitorStateClosing::OnSuspendRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateSuspending::Inst());
}

/* --- Suspending --- */

MonitorStateSuspending MonitorStateSuspending::mInstance;

bool MonitorStateSuspending::OnLayerClose(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateSuspended::Inst());
	return true;
}

void MonitorStateSuspending::OnStartRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateClosing::Inst());
}

void MonitorStateSuspending::OnShutdownRequest(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateShutingDown::Inst());
}

/* --- Stopping --- */

MonitorStateShutingDown MonitorStateShutingDown::mInstance;

bool MonitorStateShutingDown::OnLayerClose(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, MonitorStateShutdown::Inst());
	return true;
}

}
