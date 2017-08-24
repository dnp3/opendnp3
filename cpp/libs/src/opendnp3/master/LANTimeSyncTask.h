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
#ifndef OPENDNP3_LANTIMESYNCTASK_H
#define OPENDNP3_LANTIMESYNCTASK_H

#include <openpal/executor/IUTCTimeSource.h>

#include "opendnp3/master/IMasterTask.h"
#include "opendnp3/master/TaskPriority.h"

namespace opendnp3
{

// Synchronizes the time on the outstation
class LANTimeSyncTask : public IMasterTask
{

	enum State
	{
		RECORD_CURRENT_TIME,
		WRITE_TIME
	};

public:

	LANTimeSyncTask(const std::shared_ptr<TaskContext>& context, IMasterApplication& app, openpal::Logger logger);

	virtual char const* Name() const override final
	{
		return "LAN time sync";
	}

	virtual int Priority() const override final
	{
		return priority::TIME_SYNC;
	}

	virtual bool BlocksLowerPriority() const override final
	{
		return true;
	}

	virtual bool IsRecurring() const override final
	{
		return true;
	}

	virtual bool BuildRequest(APDURequest& request, uint8_t seq) override final;

private:

	virtual MasterTaskType GetTaskType() const override final
	{
		return MasterTaskType::LAN_TIME_SYNC;
	}

	virtual bool IsEnabled() const override final
	{
		return true;
	}

	virtual ResponseResult ProcessResponse(const APDUResponseHeader& response, const openpal::RSlice& objects) override final;

	ResponseResult OnResponseRecordCurrentTime(const APDUResponseHeader& header, const openpal::RSlice& objects);

	ResponseResult OnResponseWriteTime(const APDUResponseHeader& header, const openpal::RSlice& objects);

	virtual void Initialize() override final;

	State state = State::RECORD_CURRENT_TIME;

	// what time we sent the delay meas
	openpal::UTCTimestamp start;
};

} //ens ns

#endif

