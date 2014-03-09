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
#ifndef __MASTER_SCHEDULE_H_
#define __MASTER_SCHEDULE_H_

#include "MasterConfig.h"


#include "TrackingTaskGroup.h"

namespace opendnp3
{

class Master;

/**
 * Create all the tasks required for the Master from the AsyncTaskGroup.
 * Defines the types and the dependencies between them.
 */
class MasterSchedule
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

	enum MasterPriority
	{
	    AMP_VTO_TRANSMIT,
	    AMP_POLL,
	    AMP_TIME_SYNC,
	    AMP_UNSOL_CHANGE,
	    AMP_CLEAR_RESTART,
	    AMP_COMMAND
	};

	enum TaskTypes
	{
	    ONLINE_ONLY_TASKS = 1,
	    START_UP_TASKS = 2
	};
};

}



#endif
