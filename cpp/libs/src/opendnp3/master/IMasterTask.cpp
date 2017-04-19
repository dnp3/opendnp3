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
#include "IMasterTask.h"

#include "openpal/logging/LogMacros.h"
#include "opendnp3/LogLevels.h"

using namespace openpal;

namespace opendnp3
{

IMasterTask::IMasterTask(const std::shared_ptr<TaskContext>& context, IMasterApplication& app, const TaskBehavior& behavior, const openpal::Logger& logger, TaskConfig config) :
	context(context),
	application(&app),
	logger(logger),
	config(config),
	behavior(behavior)
{

}

IMasterTask::~IMasterTask()
{
	context->RemoveBlock(*this);

	if (config.pCallback)
	{
		config.pCallback->OnDestroyed();
	}
}

openpal::MonotonicTimestamp IMasterTask::ExpirationTime() const
{
	return this->IsEnabled() ? this->behavior.GetExpiration() : MonotonicTimestamp::Max();
}

openpal::MonotonicTimestamp IMasterTask::StartExpirationTime() const
{
	return this->behavior.GetStartExpiration();
}

IMasterTask::ResponseResult IMasterTask::OnResponse(const APDUResponseHeader& response, const openpal::RSlice& objects, openpal::MonotonicTimestamp now)
{
	auto result = this->ProcessResponse(response, objects);

	switch (result)
	{
	case(ResponseResult::ERROR_BAD_RESPONSE) :
		this->CompleteTask(TaskCompletion::FAILURE_BAD_RESPONSE, now);
		break;
	case(ResponseResult::OK_FINAL) :
		this->CompleteTask(TaskCompletion::SUCCESS, now);
		break;
	default:
		break;
	}

	return result;
}

void IMasterTask::CompleteTask(TaskCompletion result, openpal::MonotonicTimestamp now)
{
	switch (result)
	{

	// retry immediately when the comms come back online
	case(TaskCompletion::FAILURE_NO_COMMS):
		this->behavior.Reset();
		break;

	// back-off exponentially using the task retry
	case(TaskCompletion::FAILURE_RESPONSE_TIMEOUT):
		{
			this->behavior.OnResponseTimeout(now);
			if (this->BlocksLowerPriority()) this->context->AddBlock(*this);
			break;
		}

	case(TaskCompletion::SUCCESS):
		this->behavior.OnSuccess(now);
		this->context->RemoveBlock(*this);
		break;

	/**
	FAILURE_BAD_RESPONSE
	FAILURE_START_TIMEOUT
	FAILURE_MESSAGE_FORMAT_ERROR
	*/
	default:
		{
			this->behavior.Disable();
			if (this->BlocksLowerPriority()) this->context->AddBlock(*this);
		}
	}

	if (config.pCallback)
	{
		config.pCallback->OnComplete(result);
	}

	// notify the application
	this->application->OnTaskComplete(TaskInfo(this->GetTaskType(), result, config.taskId));

	// notify any super class implementations
	this->OnTaskComplete(result, now);
}

void IMasterTask::OnResponseTimeout(openpal::MonotonicTimestamp now)
{
	this->CompleteTask(TaskCompletion::FAILURE_RESPONSE_TIMEOUT, now);
}

void IMasterTask::OnLowerLayerClose(openpal::MonotonicTimestamp now)
{
	this->CompleteTask(TaskCompletion::FAILURE_NO_COMMS, now);
}

void IMasterTask::OnStartTimeout(openpal::MonotonicTimestamp now)
{
	this->CompleteTask(TaskCompletion::FAILURE_START_TIMEOUT, now);
}

void IMasterTask::OnMessageFormatError(openpal::MonotonicTimestamp now)
{
	this->CompleteTask(TaskCompletion::FAILURE_MESSAGE_FORMAT_ERROR, now);
}

void IMasterTask::OnStart()
{
	if (config.pCallback)
	{
		config.pCallback->OnStart();
	}

	this->application->OnTaskStart(this->GetTaskType(), config.taskId);

	this->Initialize();
}

void IMasterTask::SetMinExpiration()
{
	this->behavior.Reset();
}

bool IMasterTask::ValidateSingleResponse(const APDUResponseHeader& header)
{
	if (header.control.FIR && header.control.FIN)
	{
		return true;
	}
	else
	{
		SIMPLE_LOG_BLOCK(logger, flags::WARN, "Ignoring unexpected response FIR/FIN not set");
		return false;
	}
}

bool IMasterTask::ValidateNullResponse(const APDUResponseHeader& header, const openpal::RSlice& objects)
{
	return ValidateSingleResponse(header) && ValidateNoObjects(objects) && ValidateInternalIndications(header);
}


bool IMasterTask::ValidateInternalIndications(const APDUResponseHeader& header)
{
	if (header.IIN.HasRequestError())
	{
		FORMAT_LOG_BLOCK(logger, flags::WARN, "Task was explicitly rejected via response with error IIN bit(s): %s", this->Name());
		return false;
	}
	else
	{
		return true;
	}
}

bool IMasterTask::ValidateNoObjects(const openpal::RSlice& objects)
{
	if (objects.IsEmpty())
	{
		return true;
	}
	else
	{
		FORMAT_LOG_BLOCK(logger, flags::WARN, "Received unexpected response object headers for task: %s", this->Name());
		return false;
	}
}

}

