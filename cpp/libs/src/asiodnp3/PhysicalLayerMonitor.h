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
#ifndef ASIODNP3_PHYSICALLAYERMONITOR_H
#define ASIODNP3_PHYSICALLAYERMONITOR_H

#include <openpal/channel/IPhysicalLayerCallbacks.h>
#include <openpal/executor/IExecutor.h>
#include <openpal/executor/TimeDuration.h>
#include <openpal/logging/LogRoot.h>

#include <opendnp3/gen/ChannelState.h>
#include <opendnp3/link/ChannelRetry.h>

namespace openpal
{
class IPhysicalLayer;
}

namespace asiodnp3
{

class IMonitorState;

/** Manages the lifecycle of a physical layer
  */
class PhysicalLayerMonitor : public openpal::IPhysicalLayerCallbacks
{
	friend class MonitorStateActions;

public:

	PhysicalLayerMonitor(	openpal::Logger logger,
	                        openpal::IExecutor& executor,
	                        openpal::IPhysicalLayer*,
	                        const opendnp3::ChannelRetry& retry);

	/** Begin monitor execution, retry indefinitely on failure - Idempotent*/
	void Start();

	/** Begin monitor execution, don't reconnect automatically on failure - Idempotent*/
	void StartOne();

	/** Close the physical layer if it's open */
	void Close();

	/** Close the physical layer and don't try until someone calls Start() */
	void Suspend();

	/** Permanently shutdown the monitor, further calls to Start() will do nothing - Idempotent */
	void Shutdown();

	/// @return ChannelState enumeration
	opendnp3::ChannelState GetState();

	openpal::Logger& GetLogger()
	{
		return logger;
	}

	// Implement from IHandler - Try to reconnect using a timer

	virtual void OnOpenFailure() override final;
	virtual void OnLowerLayerUp() override final;
	virtual void OnLowerLayerDown() override final;

protected:

	openpal::Logger logger;

	virtual void OnPhysicalLayerOpenSuccessCallback() = 0;
	virtual void OnPhysicalLayerOpenFailureCallback() = 0;
	virtual void OnPhysicalLayerCloseCallback() = 0;
	virtual void OnStateChange(opendnp3::ChannelState aState) {}

	/// Begins the open timer
	void StartOpenTimer();

	// called when the router shuts down permanently
	virtual void OnShutdown() {}

	openpal::IPhysicalLayer* pPhys;
	openpal::IExecutor* pExecutor;

	bool IsOnline() const
	{
		return isOnline;
	}

private:

	bool isOnline;
	openpal::ITimer* mpOpenTimer;
	IMonitorState* mpState;
	bool mFinalShutdown;

	/* --- Actions for the states to call --- */

	/// Internal function used to change the state
	void ChangeState(IMonitorState& state);

	/// Internal callback when open timer expires
	void OnOpenTimerExpiration();

	/// Cancels the open timer
	void CancelOpenTimer();

	/* --- Internal helper functions --- */

	void DoFinalShutdown();

	opendnp3::ChannelRetry retry;

	openpal::TimeDuration currentRetry;
};

}

#endif
