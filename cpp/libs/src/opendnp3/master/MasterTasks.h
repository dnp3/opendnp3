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
#ifndef OPENDNP3_MASTERTASKS_H
#define OPENDNP3_MASTERTASKS_H

#include "opendnp3/master/MasterParams.h"
#include "opendnp3/master/MasterScheduler.h"
#include "opendnp3/master/IMasterApplication.h"

#include <vector>

namespace opendnp3
{

class MasterTasks
{

public:

	MasterTasks(const MasterParams& params, const openpal::Logger& logger, IMasterApplication& application, ISOEHandler& SOEHandler);

	void Initialize(MasterScheduler& scheduler);

	// master tasks that can be "failed" (startup and in response to IIN bits)
	const std::shared_ptr<IMasterTask> enableUnsol;
	const std::shared_ptr<IMasterTask> clearRestart;
	const std::shared_ptr<IMasterTask> assignClass;
	const std::shared_ptr<IMasterTask> startupIntegrity;
	const std::shared_ptr<IMasterTask> disableUnsol;
	const std::shared_ptr<IMasterTask> eventScan;

	bool TryDemandTimeSync();

	void BindTask(const std::shared_ptr<IMasterTask>& task);

private:

	// same as above, but may be NULL based on configuration
	const std::shared_ptr<IMasterTask> timeSynchronization;

	static std::shared_ptr<IMasterTask> GetTimeSyncTask(TimeSyncMode mode, const openpal::Logger& logger, IMasterApplication& application);

	std::vector<std::shared_ptr<IMasterTask>> boundTasks;

};

}



#endif
