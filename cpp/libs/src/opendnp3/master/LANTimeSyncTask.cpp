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
#include "LANTimeSyncTask.h"

#include "opendnp3/objects/Group50.h"
#include "opendnp3/app/APDUBuilders.h"

#include <openpal/serialization/Serialization.h>

using namespace openpal;

namespace opendnp3
{

LANTimeSyncTask::LANTimeSyncTask(const std::shared_ptr<TaskContext>& context, IMasterApplication& app, openpal::Logger logger) :
	IMasterTask(context, app, TaskBehavior::ReactsToIINOnly(), logger, TaskConfig::Default())
{}

void LANTimeSyncTask::Initialize()
{
	this->state = State::RECORD_CURRENT_TIME;
}

bool LANTimeSyncTask::BuildRequest(APDURequest& request, uint8_t seq)
{
	if (state  == State::RECORD_CURRENT_TIME)
	{
		this->start = this->application->Now();
		build::RecordCurrentTime(request, seq);
		return true;
	}
	else
	{
		Group50Var3 time;
		time.time = DNPTime(this->start.msSinceEpoch);
		request.SetFunction(FunctionCode::WRITE);
		request.SetControl(AppControlField::Request(seq));
		auto writer = request.GetWriter();
		return writer.WriteSingleValue<UInt8, Group50Var3>(QualifierCode::UINT8_CNT, time);
	}
}

IMasterTask::ResponseResult LANTimeSyncTask::ProcessResponse(const APDUResponseHeader& response, const openpal::RSlice& objects)
{
	return (state  == State::RECORD_CURRENT_TIME) ? OnResponseRecordCurrentTime(response, objects) : OnResponseWriteTime(response, objects);
}

IMasterTask::ResponseResult LANTimeSyncTask::OnResponseRecordCurrentTime(const APDUResponseHeader& response, const openpal::RSlice& objects)
{
	if (!ValidateNullResponse(response, objects)) return ResponseResult::ERROR_BAD_RESPONSE;

	this->state = State::WRITE_TIME;

	return ResponseResult::OK_REPEAT;
}

IMasterTask::ResponseResult LANTimeSyncTask::OnResponseWriteTime(const APDUResponseHeader& header, const openpal::RSlice& objects)
{
	return ValidateNullResponse(header, objects) ? ResponseResult::OK_FINAL : ResponseResult::ERROR_BAD_RESPONSE;
}

} //ens ns



