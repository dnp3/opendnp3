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
#ifndef OPENDNP3_EMPTY_RESPONSE_TASK_H
#define OPENDNP3_EMPTY_RESPONSE_TASK_H

#include "opendnp3/master/IMasterTask.h"
#include "opendnp3/master/TaskPriority.h"

#include <string>
#include <functional>

namespace opendnp3
{

class IMasterApplication;

class EmptyResponseTask : public IMasterTask
{	

public:	

	EmptyResponseTask(IMasterApplication& app, const std::string& name, FunctionCode func_, const std::function<void(HeaderWriter&)> format_, openpal::Logger logger, ITaskCallback* pCallback, int userId);

	virtual char const* Name() const override final { return name.c_str(); }

	virtual bool IsRecurring() const override final { return false; }

	virtual void BuildRequest(APDURequest& request, uint8_t seq) override final;

	virtual int Priority(void) const override final { return priority::USER_WRITE; }	

	virtual bool BlocksLowerPriority() const override final { return false; }		

private:

	virtual  bool IsEnabled() const override final { return true; }

	virtual MasterTaskType GetTaskType() const override final { return MasterTaskType::USER_TASK; }
		
	std::string name;
	FunctionCode func;
	std::function<void(HeaderWriter&)> format;

	virtual ResponseResult _OnResponse(const opendnp3::APDUResponseHeader& header, const openpal::ReadBufferView& objects) override final;

	virtual void _OnLowerLayerClose(openpal::MonotonicTimestamp now) override final;

	virtual void _OnResponseTimeout(openpal::MonotonicTimestamp now) override final;	

	virtual void OnResponseOK(openpal::MonotonicTimestamp now) override final;

	virtual void OnResponseError(openpal::MonotonicTimestamp now) override final;	

};


} //end ns


#endif
