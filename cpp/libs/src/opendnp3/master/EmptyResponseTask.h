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
#ifndef OPENDNP3_EMPTY_RESPONSE_TASK_H
#define OPENDNP3_EMPTY_RESPONSE_TASK_H

#include "opendnp3/master/IMasterTask.h"
#include "opendnp3/master/HeaderBuilder.h"
#include "opendnp3/master/TaskPriority.h"


#include <string>

namespace opendnp3
{

class EmptyResponseTask final : public IMasterTask
{

public:

	EmptyResponseTask(const std::shared_ptr<TaskContext>& context, IMasterApplication& app, const std::string& name, FunctionCode func, const HeaderBuilderT& format, openpal::MonotonicTimestamp startExpiration, openpal::Logger logger, const TaskConfig& config);

	virtual bool BuildRequest(APDURequest& request, uint8_t seq) override;

	virtual char const* Name() const override
	{
		return name.c_str();
	}

	virtual bool IsRecurring() const override
	{
		return false;
	}

	virtual int Priority() const override
	{
		return priority::USER_REQUEST;
	}

	virtual bool BlocksLowerPriority() const override
	{
		return false;
	}

private:

	virtual MasterTaskType GetTaskType() const
	{
		return MasterTaskType::USER_TASK;
	}

	const std::string name;

	FunctionCode func;
	HeaderBuilderT format;

	IMasterTask::ResponseResult ProcessResponse(const opendnp3::APDUResponseHeader& header, const openpal::RSlice& objects) override final;

};

} //end ns


#endif
