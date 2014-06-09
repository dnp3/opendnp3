
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
#ifndef __MASTER_SCHEDULE_H_
#define __MASTER_SCHEDULE_H_

#include <opendnp3/MasterConfig.h>
#include <opendnp3/Visibility.h>

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

    AsyncTaskBase* mpIntegrity;

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

	/**
	 * A task to transmit data from the Master::mVtoWriter queue.
	 */
	AsyncTaskBase* mpVtoTransmitTask;

	// Enables all of the tasks that run when the layer is Open
	void EnableOnlineTasks();

	// Disables all of the tasks that run when the layer is Open
	void DisableOnlineTasks();

	// Resets all of the tasks that run on startup. This is typically done after a failure
	void ResetStartupTasks();

private:

	void Init(const MasterConfig& arCfg, Master* mpMaster);


	AsyncTaskGroup* mpGroup;
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
