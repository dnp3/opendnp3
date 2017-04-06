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
#ifndef OPENDNP3_MASTERSCHEDULERBACKEND_H
#define OPENDNP3_MASTERSCHEDULERBACKEND_H

#include "opendnp3/master/IMasterTaskRunner.h"

#include <vector>
#include <memory>

namespace opendnp3
{

class MasterSchedulerBackend
{
	// Tasks are associated with a particular runner
	struct TaskRecord
	{		
		TaskRecord() = delete;

		TaskRecord(
			const std::shared_ptr<IMasterTask>& task,
			const std::shared_ptr<IMasterTaskRunner>& runner
		) :
			task(task),
			runner(runner)
		{}

		std::shared_ptr<IMasterTask> task;
		std::shared_ptr<IMasterTaskRunner> runner;
	};


public:

	explicit MasterSchedulerBackend(const std::shared_ptr<openpal::IExecutor>& executor);	

private:

	const std::shared_ptr<openpal::IExecutor> executor;
	
	std::vector<TaskRecord> records;
};

}



#endif
