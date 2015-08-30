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

void MonitorStateActions::ChangeState(PhysicalLayerMonitor& context, IMonitorState& state)
{
	context.ChangeState(state);
}

void MonitorStateActions::StartOpenTimer(PhysicalLayerMonitor& context)
{
	context.StartOpenTimer();
}

void MonitorStateActions::CancelOpenTimer(PhysicalLayerMonitor& context)
{
	context.CancelOpenTimer();
}

void MonitorStateActions::Close(PhysicalLayerMonitor& context)
{
	context.pPhys->BeginClose();
}

void MonitorStateActions::Open(PhysicalLayerMonitor& context)
{
	context.pPhys->BeginOpen();
}

/* --- ExceptsOnLayerOpen --- */

bool CannotOpen::OnLayerOpen(PhysicalLayerMonitor& context)
{
	FORMAT_LOG_BLOCK(context.GetLogger(), flags::ERR, "Invalid action for state: %s", this->Name());
	return false;
}

/* --- NotOpening --- */

bool NotOpening::OnOpenFailure(PhysicalLayerMonitor& context)
{
	FORMAT_LOG_BLOCK(context.GetLogger(), flags::ERR, "Invalid action for state: %s", this->Name());
	return false;
}

/* --- NotOpen --- */

bool NotOpen::OnLayerClose(PhysicalLayerMonitor& context)
{
	FORMAT_LOG_BLOCK(context.GetLogger(), flags::ERR, "Invalid action for state: %s", this->Name());
	return false;
}

/* --- NotWaitingForTimer --- */

bool NotWaitingForTimer::OnOpenTimeout(PhysicalLayerMonitor& context)
{
	FORMAT_LOG_BLOCK(context.GetLogger(), flags::ERR, "Invalid action for state: %s", this->Name());
	return false;
}

/* --- IgnoresClose --- */

void IgnoresClose::OnCloseRequest(PhysicalLayerMonitor& context)
{

}

/* --- IgnoresSuspend --- */

void IgnoresSuspend::OnSuspendRequest(PhysicalLayerMonitor& context)
{

}

/* --- StartsOnClose --- */

bool StartsOnClose::OnLayerClose(PhysicalLayerMonitor& context)
{
	MonitorStateActions::ChangeState(context, MonitorStateWaiting::Instance());
	MonitorStateActions::StartOpenTimer(context);
	return true;
}

/* --- IgnoresShutdown --- */

void IgnoresShutdown::OnShutdownRequest(PhysicalLayerMonitor& context)
{

}

/* --- IgnoresStart --- */

void IgnoresStart::OnStartRequest(PhysicalLayerMonitor& context)
{

}

/* --- IgnoresStartOne --- */

void IgnoresStartOne::OnStartOneRequest(PhysicalLayerMonitor& context)
{

}

/* --- OpenFailureCausesWait --- */

bool OpenFailureCausesWait::OnOpenFailure(PhysicalLayerMonitor& context)
{
	MonitorStateActions::ChangeState(context, MonitorStateWaiting::Instance());
	MonitorStateActions::StartOpenTimer(context);
	return true;
}

/* --- WaitingBase --- */

void MonitorStateWaitingBase::OnSuspendRequest(PhysicalLayerMonitor& context)
{
	MonitorStateActions::CancelOpenTimer(context);
	MonitorStateActions::ChangeState(context, MonitorStateSuspended::Instance());
}

void MonitorStateWaitingBase::OnShutdownRequest(PhysicalLayerMonitor& context)
{
	MonitorStateActions::CancelOpenTimer(context);
	MonitorStateActions::ChangeState(context, MonitorStateShutdown::Instance());
}

/* ----------------- Concrete ----------------- */


/* --- Shutdown --- */

MonitorStateShutdown MonitorStateShutdown::instance;

/* --- Suspended --- */

MonitorStateSuspended MonitorStateSuspended::instance;

/* --- Init --- */

MonitorStateInit MonitorStateInit::instance;

/* ---- SuspendedBase --- */
void MonitorStateSuspendedBase::OnStartRequest(PhysicalLayerMonitor& context)
{
	MonitorStateActions::ChangeState(context, MonitorStateOpening::Instance());
	MonitorStateActions::Open(context);
}

void MonitorStateSuspendedBase::OnStartOneRequest(PhysicalLayerMonitor& context)
{
	MonitorStateActions::ChangeState(context, MonitorStateOpeningOne::Instance());
	MonitorStateActions::Open(context);
}

void MonitorStateSuspendedBase::OnShutdownRequest(PhysicalLayerMonitor& context)
{
	MonitorStateActions::ChangeState(context, MonitorStateShutdown::Instance());
}

/* --- OpeningBase --- */

void MonitorStateOpeningBase::OnShutdownRequest(PhysicalLayerMonitor& context)
{
	MonitorStateActions::ChangeState(context, MonitorStateOpeningStopping::Instance());
	MonitorStateActions::Close(context);
}

void MonitorStateOpeningBase::OnSuspendRequest(PhysicalLayerMonitor& context)
{
	MonitorStateActions::ChangeState(context, MonitorStateOpeningSuspending::Instance());
	MonitorStateActions::Close(context);
}

/* --- Opening --- */

MonitorStateOpening MonitorStateOpening::instance;

void MonitorStateOpening::OnStartOneRequest(PhysicalLayerMonitor& context)
{
	MonitorStateActions::ChangeState(context, MonitorStateOpeningOne::Instance());
}

void MonitorStateOpening::OnCloseRequest(PhysicalLayerMonitor& context)
{
	MonitorStateActions::ChangeState(context, MonitorStateOpeningClosing::Instance());
	MonitorStateActions::Close(context);
}

bool MonitorStateOpening::OnLayerOpen(PhysicalLayerMonitor& context)
{
	MonitorStateActions::ChangeState(context, MonitorStateOpen::Instance());
	return true;
}

/* --- OpeningOne --- */

MonitorStateOpeningOne MonitorStateOpeningOne::instance;

bool MonitorStateOpeningOne::OnOpenFailure(PhysicalLayerMonitor& context)
{
	MonitorStateActions::ChangeState(context, MonitorStateSuspended::Instance());
	return true;
}

void MonitorStateOpeningOne::OnStartRequest(PhysicalLayerMonitor& context)
{
	MonitorStateActions::ChangeState(context, MonitorStateOpening::Instance());
}

void MonitorStateOpeningOne::OnCloseRequest(PhysicalLayerMonitor& context)
{
	MonitorStateActions::ChangeState(context, MonitorStateOpeningSuspending::Instance());
	MonitorStateActions::Close(context);
}

bool MonitorStateOpeningOne::OnLayerOpen(PhysicalLayerMonitor& context)
{
	MonitorStateActions::ChangeState(context, MonitorStateOpenOne::Instance());
	return true;
}

/* --- OpeningClosing --- */

MonitorStateOpeningClosing MonitorStateOpeningClosing::instance;

void MonitorStateOpeningClosing::OnStartOneRequest(PhysicalLayerMonitor& context)
{
	MonitorStateActions::ChangeState(context, MonitorStateOpeningSuspending::Instance());
}

void MonitorStateOpeningClosing::OnShutdownRequest(PhysicalLayerMonitor& context)
{
	MonitorStateActions::ChangeState(context, MonitorStateOpeningStopping::Instance());
}

void MonitorStateOpeningClosing::OnSuspendRequest(PhysicalLayerMonitor& context)
{
	MonitorStateActions::ChangeState(context, MonitorStateOpeningSuspending::Instance());
}

/* --- OpeningStopping --- */

MonitorStateOpeningStopping MonitorStateOpeningStopping::instance;

/* --- OpeningSuspending --- */

MonitorStateOpeningSuspending MonitorStateOpeningSuspending::instance;

void MonitorStateOpeningSuspending::OnStartRequest(PhysicalLayerMonitor& context)
{
	MonitorStateActions::ChangeState(context, MonitorStateOpeningClosing::Instance());
}

void MonitorStateOpeningSuspending::OnShutdownRequest(PhysicalLayerMonitor& context)
{
	MonitorStateActions::ChangeState(context, MonitorStateOpeningStopping::Instance());
}

/* --- Open --- */

MonitorStateOpen MonitorStateOpen::instance;

void MonitorStateOpen::OnStartOneRequest(PhysicalLayerMonitor& context)
{
	MonitorStateActions::ChangeState(context, MonitorStateOpenOne::Instance());
}

void MonitorStateOpen::OnCloseRequest(PhysicalLayerMonitor& context)
{
	MonitorStateActions::ChangeState(context, MonitorStateClosing::Instance());
	MonitorStateActions::Close(context);
}

void MonitorStateOpen::OnSuspendRequest(PhysicalLayerMonitor& context)
{
	MonitorStateActions::ChangeState(context, MonitorStateSuspending::Instance());
	MonitorStateActions::Close(context);
}

void MonitorStateOpen::OnShutdownRequest(PhysicalLayerMonitor& context)
{
	MonitorStateActions::ChangeState(context, MonitorStateShutingDown::Instance());
	MonitorStateActions::Close(context);
}

/* --- OpenOne --- */

MonitorStateOpenOne MonitorStateOpenOne::instance;

void MonitorStateOpenOne::OnStartRequest(PhysicalLayerMonitor& context)
{
	MonitorStateActions::ChangeState(context, MonitorStateOpen::Instance());
}

void MonitorStateOpenOne::OnCloseRequest(PhysicalLayerMonitor& context)
{
	MonitorStateActions::ChangeState(context, MonitorStateSuspending::Instance());
	MonitorStateActions::Close(context);
}

void MonitorStateOpenOne::OnSuspendRequest(PhysicalLayerMonitor& context)
{
	MonitorStateActions::ChangeState(context, MonitorStateSuspending::Instance());
	MonitorStateActions::Close(context);
}

void MonitorStateOpenOne::OnShutdownRequest(PhysicalLayerMonitor& context)
{
	MonitorStateActions::ChangeState(context, MonitorStateShutingDown::Instance());
	MonitorStateActions::Close(context);
}

bool MonitorStateOpenOne::OnLayerClose(PhysicalLayerMonitor& context)
{
	MonitorStateActions::ChangeState(context, MonitorStateSuspended::Instance());
	return true;
}

/* --- Waiting --- */

MonitorStateWaiting MonitorStateWaiting::instance;

void MonitorStateWaiting::OnStartOneRequest(PhysicalLayerMonitor& context)
{
	MonitorStateActions::ChangeState(context, MonitorStateWaitingOne::Instance());
}

bool MonitorStateWaiting::OnOpenTimeout(PhysicalLayerMonitor& context)
{
	MonitorStateActions::Open(context);
	MonitorStateActions::ChangeState(context, MonitorStateOpening::Instance());
	return true;
}

/* --- WaitingOne --- */

MonitorStateWaitingOne MonitorStateWaitingOne::instance;

void MonitorStateWaitingOne::OnStartRequest(PhysicalLayerMonitor& context)
{
	MonitorStateActions::ChangeState(context, MonitorStateWaiting::Instance());
}

bool MonitorStateWaitingOne::OnOpenTimeout(PhysicalLayerMonitor& context)
{
	MonitorStateActions::Open(context);
	MonitorStateActions::ChangeState(context, MonitorStateOpeningOne::Instance());
	return true;
}

/* --- Closing --- */

MonitorStateClosing MonitorStateClosing::instance;

void MonitorStateClosing::OnStartOneRequest(PhysicalLayerMonitor& context)
{
	MonitorStateActions::ChangeState(context, MonitorStateSuspending::Instance());
}

void MonitorStateClosing::OnShutdownRequest(PhysicalLayerMonitor& context)
{
	MonitorStateActions::ChangeState(context, MonitorStateShutingDown::Instance());
}

void MonitorStateClosing::OnSuspendRequest(PhysicalLayerMonitor& context)
{
	MonitorStateActions::ChangeState(context, MonitorStateSuspending::Instance());
}

/* --- Suspending --- */

MonitorStateSuspending MonitorStateSuspending::instance;

bool MonitorStateSuspending::OnLayerClose(PhysicalLayerMonitor& context)
{
	MonitorStateActions::ChangeState(context, MonitorStateSuspended::Instance());
	return true;
}

void MonitorStateSuspending::OnStartRequest(PhysicalLayerMonitor& context)
{
	MonitorStateActions::ChangeState(context, MonitorStateClosing::Instance());
}

void MonitorStateSuspending::OnShutdownRequest(PhysicalLayerMonitor& context)
{
	MonitorStateActions::ChangeState(context, MonitorStateShutingDown::Instance());
}

/* --- Stopping --- */

MonitorStateShutingDown MonitorStateShutingDown::instance;

bool MonitorStateShutingDown::OnLayerClose(PhysicalLayerMonitor& context)
{
	MonitorStateActions::ChangeState(context, MonitorStateShutdown::Instance());
	return true;
}

}
