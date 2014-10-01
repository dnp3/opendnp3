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

#include "SerialTimeSyncTask.h"

#include "opendnp3/objects/Group50.h"
#include "opendnp3/app/APDUParser.h"
#include "opendnp3/app/APDUBuilders.h"
#include "opendnp3/master/TimeSyncHandler.h"

#include <openpal/serialization/Serialization.h>

using namespace openpal;

namespace opendnp3
{

SerialTimeSyncTask::SerialTimeSyncTask(openpal::Logger* pLogger_, openpal::IUTCTimeSource* pTimeSource_) :
	SingleResponseTask(pLogger_),
	expiration(MonotonicTimestamp::Max()),
	pTimeSource(pTimeSource_),
	delay(-1)
{}

void SerialTimeSyncTask::Reset()
{
	delay = -1;
}

void SerialTimeSyncTask::BuildRequest(APDURequest& request, uint8_t seq)
{
	if (delay < 0)
	{
		start = pTimeSource->Now();
		build::MeasureDelay(request, seq);
	}
	else
	{
		auto now = pTimeSource->Now();
		Group50Var1 time;
		time.time = now.msSinceEpoch + delay;
		request.SetFunction(FunctionCode::WRITE);
		request.SetControl(AppControlField::Request(seq));
		auto writer = request.GetWriter();
		writer.WriteSingleValue<UInt8, Group50Var1>(QualifierCode::UINT8_CNT, time);
	}
}

openpal::MonotonicTimestamp SerialTimeSyncTask::ExpirationTime() const
{

}

void SerialTimeSyncTask::OnTimeoutOrBadControlOctet(const openpal::MonotonicTimestamp& now)
{
	// TODO - some kind of logging?
	// don't reschedule. Seeing the NeedTime bit again will automatically re-activate the task
}

TaskState SerialTimeSyncTask::OnSingleResponse(const APDUResponseHeader& response, const openpal::ReadOnlyBuffer& objects, const openpal::MonotonicTimestamp& now)
{
	if (delay < 0)
	{
		TimeSyncHandler handler(*pLogger);
		auto result = APDUParser::ParseTwoPass(objects, &handler, pLogger);
		if (result == APDUParser::Result::OK)
		{
			uint16_t rtuTurnAroundTime;
			if (handler.GetTimeDelay(rtuTurnAroundTime))
			{
				auto now = pTimeSource->Now();
				auto sendReceieveTime = now.msSinceEpoch - start.msSinceEpoch;

				// The later shouldn't happen, but could cause a negative delay which would result in a weird time setting				
				delay = (sendReceieveTime >= rtuTurnAroundTime) ? (sendReceieveTime - rtuTurnAroundTime) / 2 : 0;				
				return TaskState::REPEAT;
			}
			else
			{
				expiration = MonotonicTimestamp::Max();
				return TaskState::COMPLETE;
			}
		}
		else
		{		
			expiration = MonotonicTimestamp::Max();
			return TaskState::COMPLETE;
		}
	}
	else
	{
		expiration = MonotonicTimestamp::Max();
		return TaskState::COMPLETE;
	}
}
	
} //ens ns



