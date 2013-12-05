#ifndef __PHYSICAL_LAYER_MONITOR_H_
#define __PHYSICAL_LAYER_MONITOR_H_

#include <openpal/IHandlerAsync.h>
#include <openpal/IExecutor.h>
#include <openpal/Visibility.h>
#include <openpal/TimeDuration.h>

#include <opendnp3/gen/ChannelState.h>

#include <set>
#include <mutex>
#include <condition_variable>

namespace openpal
{
class IPhysicalLayerAsync;
}


namespace opendnp3
{

class IMonitorState;

/** Manages the lifecycle of a physical layer
  */
class DLL_LOCAL PhysicalLayerMonitor : public openpal::IHandlerAsync
{
	friend class MonitorStateActions;

public:
	PhysicalLayerMonitor(	openpal::Logger,
	                        openpal::IPhysicalLayerAsync*,
	                        openpal::TimeDuration aMinOpenRetry,
	                        openpal::TimeDuration aMaxOpenRetry);

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
	bool WaitForShutdown(openpal::TimeDuration aTimeout = openpal::TimeDuration::Min());

	openpal::Logger& GetLogger() {
		return mLogger;
	}

protected:


	virtual void OnPhysicalLayerOpenSuccessCallback() = 0;
	virtual void OnPhysicalLayerOpenFailureCallback() = 0;
	virtual void OnPhysicalLayerCloseCallback() = 0;
	virtual void OnStateChange(ChannelState aState) {}

	/// Begins the open timer
	void StartOpenTimer();

	openpal::IPhysicalLayerAsync* mpPhys;

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

	const openpal::TimeDuration mMinOpenRetry;
	const openpal::TimeDuration mMaxOpenRetry;

	openpal::TimeDuration mCurrentRetry;

	// Implement from IHandlerAsync - Try to reconnect using a timer
	void _OnOpenFailure();
	void _OnLowerLayerUp();
	void _OnLowerLayerDown();

};
}

#endif
