
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
// you under the GNU Affero General Public License Version 3.0
// (the "Additional License"). You may not use these modifications except in
// compliance with the additional license. You may obtain a copy of the Additional
// License at
//
// http://www.gnu.org/licenses/agpl.html
//
// Contact Automatak, LLC for a commercial license to these modifications
//
#ifndef __PHYSICAL_LAYER_MONITOR_H_
#define __PHYSICAL_LAYER_MONITOR_H_

#include "IHandlerAsync.h"
#include <openpal/IExecutor.h>

#include <opendnp3/ChannelStates.h>
#include <opendnp3/Location.h>

#include <set>
#include <mutex>
#include <condition_variable>
#include <chrono>

namespace opendnp3
{

class IPhysicalLayerAsync;
class IMonitorState;

/** Manages the lifecycle of a physical layer
  */
class DLL_LOCAL PhysicalLayerMonitor : public IHandlerAsync
{
	friend class MonitorStateActions;

public:
	PhysicalLayerMonitor(	Logger*,
	                        IPhysicalLayerAsync*,
	                        openpal::timer_clock::duration aMinOpenRetry,
	                        openpal::timer_clock::duration aMaxOpenRetry);

	~PhysicalLayerMonitor();

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
	ChannelState GetState();

	/** Posts a Shutdown() call and then waits for shutdown to complete.
	*/
	bool WaitForShutdown(openpal::millis_t aTimeout = -1);

	Logger* GetLogger() {
		return mpLogger;
	}

protected:


	virtual void OnPhysicalLayerOpenSuccessCallback() = 0;
	virtual void OnPhysicalLayerOpenFailureCallback() = 0;
	virtual void OnPhysicalLayerCloseCallback() = 0;
	virtual void OnStateChange(ChannelState aState) {}

	/// Begins the open timer
	void StartOpenTimer();

	IPhysicalLayerAsync* mpPhys;

private:

	openpal::ITimer* mpOpenTimer;
	IMonitorState* mpState;
	bool mFinalShutdown;

	/* --- Actions for the states to call --- */

	/// Internal function used to change the state
	void ChangeState(IMonitorState* apState);

	/// Internal callback when open timer expires
	void OnOpenTimerExpiration();

	/// Cancels the open timer
	void CancelOpenTimer();

	/* --- Internal helper functions --- */

	void DoFinalShutdown();

	std::mutex mMutex;
	std::condition_variable mCondition;

	const openpal::timer_clock::duration mMinOpenRetry;
	const openpal::timer_clock::duration mMaxOpenRetry;

	openpal::timer_clock::duration mCurrentRetry;

	// Implement from IHandlerAsync - Try to reconnect using a timer
	void _OnOpenFailure();
	void _OnLowerLayerUp();
	void _OnLowerLayerDown();

};
}

#endif
