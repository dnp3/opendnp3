#ifndef __MASTER_SCHEDULE_H_
#define __MASTER_SCHEDULE_H_

#include <opendnp3/MasterConfig.h>
#include <openpal/Visibility.h>

#include "TrackingTaskGroup.h"

namespace opendnp3
{

class Master;

/**
 * Create all the tasks required for the Master from the AsyncTaskGroup.
 * Defines the types and the dependencies between them.
 */
class DLL_LOCAL MasterSchedule
{
public:

	MasterSchedule(AsyncTaskGroup* apGroup, Master* apMaster, const MasterConfig& arCfg);

	/**
	* Task that controls the integrity poll
	*/
	AsyncTaskBase* mpIntegrityPoll;

	/**
	 * A task to read the Master::mCommandQueue and pass objects to
	 * Master::ProcessControl.
	 */
	AsyncTaskBase* mpCommandTask;

	/**
	 * A task to synchronize time on the outstation.
	 */
	AsyncTaskBase* mpTimeTask;

	/**
	 * A task to clear the restart IIN bit.
	 */
	AsyncTaskBase* mpClearRestartTask;

	// Enables all of the tasks that run when the layer is Open
	void EnableOnlineTasks();

	// Disables all of the tasks that run when the layer is Open
	void DisableOnlineTasks();

	// Resets all of the tasks that run on startup. This is typically done after a failure
	void ResetStartupTasks();

	AsyncTaskBase* AddClassScan(int aClassMask, openpal::TimeDuration aScanRate, openpal::TimeDuration aRetryRate);

private:

	MasterSchedule();
	MasterSchedule(const MasterSchedule&);

	void Init(const MasterConfig& arCfg);


	AsyncTaskGroup* mpGroup;
	Master* mpMaster;
	TrackingTaskGroup mTracking;

	enum MasterPriority {
		AMP_VTO_TRANSMIT,
		AMP_POLL,
		AMP_TIME_SYNC,
		AMP_UNSOL_CHANGE,
		AMP_CLEAR_RESTART,
		AMP_COMMAND
	};

	enum TaskTypes {
		ONLINE_ONLY_TASKS = 1,
		START_UP_TASKS = 2
	};
};

}

/* vim: set ts=4 sw=4: */

#endif
