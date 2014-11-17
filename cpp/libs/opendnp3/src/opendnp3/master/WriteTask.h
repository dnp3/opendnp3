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
#ifndef OPENDNP3_WRITETASK_H
#define OPENDNP3_WRITETASK_H

#include "opendnp3/master/NullResponseTask.h"
#include "opendnp3/master/TaskPriority.h"

#include <string>
#include <functional>

namespace opendnp3
{

class IMasterApplication;

class WriteTask : public NullResponseTask
{	

public:	

	WriteTask(const MasterParams& params, const std::string& name, TaskId id, ITaskCallback* pCallback, const std::function<void(HeaderWriter&)> format, const openpal::Logger& logger);

	virtual TaskId Id() const override final { return id; }

	virtual ITaskCallback* GetTaskCallback() override final { return pCallback; }

	virtual char const* Name() const override final { return name.c_str(); }

	virtual bool IsRecurring() const override final { return false; }

	virtual void BuildRequest(APDURequest& request, uint8_t seq) override final;

	virtual int Priority(void) const override final { return priority::USER_WRITE; }

	virtual openpal::MonotonicTimestamp ExpirationTime() const override final { return openpal::MonotonicTimestamp(0); }

	virtual bool BlocksLowerPriority() const { return false; }

	virtual void OnLowerLayerClose(const openpal::MonotonicTimestamp& now) override final;

	virtual void OnResponseTimeout(const openpal::MonotonicTimestamp& now) override final;

	virtual void Demand() override final {}

private:

	openpal::MonotonicTimestamp expiration;
	const MasterParams* pParams;
	std::string name;
	TaskId id;	
	ITaskCallback* pCallback;
	std::function<void(HeaderWriter&)> format;

	virtual void OnSuccess(const openpal::MonotonicTimestamp& now) override final;

	virtual void OnBadControlOctet(const openpal::MonotonicTimestamp& now) override final;

	virtual void OnRejectedIIN(const openpal::MonotonicTimestamp& now) override final;

};


} //end ns


#endif
