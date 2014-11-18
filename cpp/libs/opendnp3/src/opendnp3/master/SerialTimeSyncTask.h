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
#ifndef OPENDNP3_SERIALTIMESYNCTASK_H
#define OPENDNP3_SERIALTIMESYNCTASK_H

#include <openpal/executor/IUTCTimeSource.h>

#include "opendnp3/master/SingleResponseTask.h"
#include "opendnp3/master/TaskPriority.h"

namespace opendnp3
{

// Synchronizes the time on the outstation
class SerialTimeSyncTask : public SingleResponseTask
{

public:
	SerialTimeSyncTask(const openpal::Logger& logger, openpal::IUTCTimeSource* pTimeSource_);

	virtual TaskId Id() const override final { return TaskId::From(TaskIdValue::SERIAL_TIME_SYNC); }
	
	virtual char const* Name() const override final { return "serial time sync"; }

	virtual int Priority() const override final { return priority::TIME_SYNC; }
	
	virtual bool BlocksLowerPriority() const override final { return true; }
	
	virtual bool IsRecurring() const override final { return true; }
	
	virtual openpal::MonotonicTimestamp ExpirationTime() const override final;

	virtual void BuildRequest(APDURequest& request, uint8_t seq) override final;	

	virtual void OnResponseTimeout(const openpal::MonotonicTimestamp &) override final;

	void OnLowerLayerClose(const openpal::MonotonicTimestamp &) override final;

	virtual void Demand() override final { expiration = 0; }

protected:

	virtual void OnBadControlOctet(const openpal::MonotonicTimestamp& now) override final;

	virtual Result OnOnlyResponse(const APDUResponseHeader& response, const openpal::ReadOnlyBuffer& objects, const openpal::MonotonicTimestamp& now) override final;

private:

	void Initialize();

	openpal::MonotonicTimestamp expiration;

	openpal::IUTCTimeSource* pTimeSource;

	// < 0 implies the delay measure hasn't happened yet
	int64_t delay;

	// what time we sent the delay meas
	openpal::UTCTimestamp start;
};

} //ens ns

#endif

