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
#ifndef ASIODNP3_PHYSICALLAYERMONITORSTATES_H
#define ASIODNP3_PHYSICALLAYERMONITORSTATES_H

#include <opendnp3/link/Singleton.h>
#include <opendnp3/gen/ChannelState.h>

#include "asiodnp3/impl/PhysicalLayerMonitor.h"


#define MACRO_MONITOR_SINGLETON(type, state, shuttingDown) \
	MACRO_NAME_SINGLETON_INSTANCE(type) \
	opendnp3::ChannelState GetState() const { return state; } \
	bool IsShuttingDown() const { return shuttingDown; }

namespace asiodnp3
{

class PhysicalLayerMonitor;

/* --- Base classes --- */

class IMonitorState
{
public:

	virtual void OnStartRequest(PhysicalLayerMonitor* apContext) = 0;
	virtual void OnStartOneRequest(PhysicalLayerMonitor* apContext) = 0;
	virtual void OnCloseRequest(PhysicalLayerMonitor* apContext) = 0;
	virtual void OnSuspendRequest(PhysicalLayerMonitor* apContext) = 0;
	virtual void OnShutdownRequest(PhysicalLayerMonitor* apContext) = 0;

	virtual bool OnOpenTimeout(PhysicalLayerMonitor* apContext) = 0;
	virtual bool OnOpenFailure(PhysicalLayerMonitor* apContext) = 0;
	virtual bool OnLayerOpen(PhysicalLayerMonitor* apContext) = 0;
	virtual bool OnLayerClose(PhysicalLayerMonitor* apContext) = 0;

	virtual opendnp3::ChannelState GetState() const = 0;

	virtual char const* Name() const = 0;

	virtual bool IsShuttingDown() const = 0;	
};

class MonitorStateActions
{
public:

	static void ChangeState(PhysicalLayerMonitor* apContext, IMonitorState* apState);
	static void StartOpenTimer(PhysicalLayerMonitor* apContext);
	static void CancelOpenTimer(PhysicalLayerMonitor* apContext);
	static void Close(PhysicalLayerMonitor* apContext);
	static void Open(PhysicalLayerMonitor* apContext);
};

class CannotOpen : public virtual IMonitorState
{
public:
	bool OnLayerOpen(PhysicalLayerMonitor* apContext) override final;
};

class NotOpening : public CannotOpen
{
public:
	bool OnOpenFailure(PhysicalLayerMonitor* apContext) override final;
};

class NotOpen : public virtual IMonitorState
{
public:
	bool OnLayerClose(PhysicalLayerMonitor* apContext) override final;
};

class NotWaitingForTimer : public virtual IMonitorState
{
public:
	bool OnOpenTimeout(PhysicalLayerMonitor* apContext) override final;
};

class IgnoresClose : public virtual IMonitorState
{
public:
	void OnCloseRequest(PhysicalLayerMonitor* apContext) override final;
};

class IgnoresSuspend : public virtual IMonitorState
{
public:
	void OnSuspendRequest(PhysicalLayerMonitor* apContext) override final;
};

class StartsOnClose : public virtual IMonitorState
{
public:
	bool OnLayerClose(PhysicalLayerMonitor* apContext) override final;
};

class IgnoresShutdown : public virtual IMonitorState
{
public:
	void OnShutdownRequest(PhysicalLayerMonitor* apContext) override final;
};

class IgnoresStart : public virtual IMonitorState
{
public:
	void OnStartRequest(PhysicalLayerMonitor* apContext) override final;
};

class IgnoresStartOne : public virtual IMonitorState
{
public:
	void OnStartOneRequest(PhysicalLayerMonitor* apContext) override final;
};

class OpenFailureCausesWait : public virtual IMonitorState
{
public:
	bool OnOpenFailure(PhysicalLayerMonitor* apContext) override final;
};

template <class T>
class OpenFailureGoesToState : public virtual IMonitorState
{
public:
	bool OnOpenFailure(PhysicalLayerMonitor* apContext) override final;
};

// disable "inherits via dominance warning", it's erroneous b/c base
// class is pure virtual and G++ correctly deduces this and doesn't care
#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4250)
#endif

class MonitorStateWaitingBase : public virtual IMonitorState,
	private NotOpening, private NotOpen, private IgnoresClose
{
	void OnSuspendRequest(PhysicalLayerMonitor* apContext) override final;
	void OnShutdownRequest(PhysicalLayerMonitor* apContext) override final;
};

/* --- Concrete classes --- */

class MonitorStateShutdown : public virtual IMonitorState,
	private NotOpening,
	private NotOpen,
	private NotWaitingForTimer,
	private IgnoresClose,
	private IgnoresStart,
	private IgnoresStartOne,
	private IgnoresShutdown,
	private IgnoresSuspend
{
	MACRO_MONITOR_SINGLETON(MonitorStateShutdown, opendnp3::ChannelState::SHUTDOWN, true);
};

class MonitorStateSuspendedBase : public virtual IMonitorState,
	private NotOpening,
	private NotOpen,
	private NotWaitingForTimer,
	private IgnoresClose,
	private IgnoresSuspend
{
	void OnStartRequest(PhysicalLayerMonitor* apContext) override final;
	void OnStartOneRequest(PhysicalLayerMonitor* apContext) override final;
	void OnShutdownRequest(PhysicalLayerMonitor* apContext) override final;
};


class MonitorStateSuspended : public MonitorStateSuspendedBase
{
	MACRO_MONITOR_SINGLETON(MonitorStateSuspended, opendnp3::ChannelState::CLOSED, false);
};

class MonitorStateInit : public MonitorStateSuspendedBase
{
	MACRO_MONITOR_SINGLETON(MonitorStateInit, opendnp3::ChannelState::CLOSED, false);
};

class MonitorStateOpeningBase : public virtual IMonitorState,
	private NotOpen,
	private NotWaitingForTimer
{
	void OnShutdownRequest(PhysicalLayerMonitor* apContext) override final;
	void OnSuspendRequest(PhysicalLayerMonitor* apContext) override final;
};


class MonitorStateOpening : public MonitorStateOpeningBase,
	private OpenFailureCausesWait,
	private IgnoresStart
{
	MACRO_MONITOR_SINGLETON(MonitorStateOpening, opendnp3::ChannelState::OPENING, false);

	void OnStartOneRequest(PhysicalLayerMonitor* apContext) override final;
	void OnCloseRequest(PhysicalLayerMonitor* apContext) override final;
	bool OnLayerOpen(PhysicalLayerMonitor* apContext) override final;
};

class MonitorStateOpeningOne : public MonitorStateOpeningBase,
	private IgnoresStartOne
{
	MACRO_MONITOR_SINGLETON(MonitorStateOpeningOne, opendnp3::ChannelState::OPENING, false);

	bool OnOpenFailure(PhysicalLayerMonitor* apContext) override final;
	void OnStartRequest(PhysicalLayerMonitor* apContext) override final;
	void OnCloseRequest(PhysicalLayerMonitor* apContext) override final;
	bool OnLayerOpen(PhysicalLayerMonitor* apContext) override final;
};

class MonitorStateOpeningClosing : public virtual IMonitorState,
	private NotOpen,
	private NotWaitingForTimer,
	private CannotOpen,
	private OpenFailureCausesWait,
	private IgnoresStart,
	private IgnoresClose
{
	MACRO_MONITOR_SINGLETON(MonitorStateOpeningClosing, opendnp3::ChannelState::OPENING, false);

	void OnStartOneRequest(PhysicalLayerMonitor* apContext) override final;
	void OnShutdownRequest(PhysicalLayerMonitor* apContext) override final;
	void OnSuspendRequest(PhysicalLayerMonitor* apContext) override final;
};

class MonitorStateOpeningStopping : public virtual IMonitorState,
	private NotOpen,
	private NotWaitingForTimer,
	private CannotOpen,
	private OpenFailureGoesToState<MonitorStateShutdown>,
	private IgnoresStart,
	private IgnoresStartOne,
	private IgnoresClose,
	private IgnoresSuspend,
	private IgnoresShutdown
{
	MACRO_MONITOR_SINGLETON(MonitorStateOpeningStopping, opendnp3::ChannelState::OPENING, true);
};

class MonitorStateOpeningSuspending : public virtual IMonitorState,
	private NotOpen,
	private NotWaitingForTimer,
	private CannotOpen,
	private OpenFailureGoesToState<MonitorStateSuspended>,
	private IgnoresClose,
	private IgnoresStartOne,
	private IgnoresSuspend
{
	MACRO_MONITOR_SINGLETON(MonitorStateOpeningSuspending, opendnp3::ChannelState::OPENING, false);

	void OnStartRequest(PhysicalLayerMonitor* apContext) override final;
	void OnShutdownRequest(PhysicalLayerMonitor* apContext) override final;
};

class MonitorStateOpen : public virtual IMonitorState,
	private NotOpening,
	private NotWaitingForTimer,
	private IgnoresStart,
	private StartsOnClose
{
	MACRO_MONITOR_SINGLETON(MonitorStateOpen, opendnp3::ChannelState::OPEN, false);

	void OnStartOneRequest(PhysicalLayerMonitor* apContext) override final;
	void OnCloseRequest(PhysicalLayerMonitor* apContext) override final;
	void OnSuspendRequest(PhysicalLayerMonitor* apContext) override final;
	void OnShutdownRequest(PhysicalLayerMonitor* apContext) override final;
};

class MonitorStateOpenOne : public virtual IMonitorState,
	private NotOpening,
	private NotWaitingForTimer,
	private IgnoresStartOne
{
	MACRO_MONITOR_SINGLETON(MonitorStateOpenOne, opendnp3::ChannelState::OPEN, false);

	bool OnLayerClose(PhysicalLayerMonitor* apContext) override final;
	void OnStartRequest(PhysicalLayerMonitor* apContext) override final;
	void OnCloseRequest(PhysicalLayerMonitor* apContext) override final;
	void OnSuspendRequest(PhysicalLayerMonitor* apContext) override final;
	void OnShutdownRequest(PhysicalLayerMonitor* apContext) override final;
};

class MonitorStateWaiting : public MonitorStateWaitingBase, private IgnoresStart
{
	MACRO_MONITOR_SINGLETON(MonitorStateWaiting, opendnp3::ChannelState::WAITING, false);

	void OnStartOneRequest(PhysicalLayerMonitor* apContext) override final;
	bool OnOpenTimeout(PhysicalLayerMonitor* apContext) override final;
};

class MonitorStateWaitingOne : public MonitorStateWaitingBase, private IgnoresStartOne
{
	MACRO_MONITOR_SINGLETON(MonitorStateWaitingOne, opendnp3::ChannelState::WAITING, false);

	void OnStartRequest(PhysicalLayerMonitor* apContext) override final;
	bool OnOpenTimeout(PhysicalLayerMonitor* apContext) override final;
};

class MonitorStateClosing : public virtual IMonitorState,
	private NotOpening, private NotWaitingForTimer, private IgnoresStart, private IgnoresClose, private StartsOnClose
{
	MACRO_MONITOR_SINGLETON(MonitorStateClosing, opendnp3::ChannelState::CLOSED, false);

	void OnStartOneRequest(PhysicalLayerMonitor* apContext) override final;
	void OnSuspendRequest(PhysicalLayerMonitor* apContext) override final;
	void OnShutdownRequest(PhysicalLayerMonitor* apContext) override final;
};

class MonitorStateSuspending : public virtual IMonitorState,
	private NotOpening, private NotWaitingForTimer, private IgnoresClose, private IgnoresSuspend, private IgnoresStartOne
{
	MACRO_MONITOR_SINGLETON(MonitorStateSuspending, opendnp3::ChannelState::CLOSED, false);

	bool OnLayerClose(PhysicalLayerMonitor* apContext) override final;
	void OnStartRequest(PhysicalLayerMonitor* apContext) override final;
	void OnShutdownRequest(PhysicalLayerMonitor* apContext) override final;
};

class MonitorStateShutingDown : public virtual IMonitorState,
	private NotOpening,
	private NotWaitingForTimer,
	private IgnoresStart,
	private IgnoresStartOne,
	private IgnoresClose,
	private IgnoresShutdown,
	private IgnoresSuspend
{
	MACRO_MONITOR_SINGLETON(MonitorStateShutingDown, opendnp3::ChannelState::CLOSED, true);

	bool OnLayerClose(PhysicalLayerMonitor* apContext) override final;
};

template <class T>
bool OpenFailureGoesToState<T>::OnOpenFailure(PhysicalLayerMonitor* apContext)
{
	MonitorStateActions::ChangeState(apContext, T::Inst());
	return true;
}

#ifdef WIN32
#pragma warning(pop)
#endif

}

#endif

