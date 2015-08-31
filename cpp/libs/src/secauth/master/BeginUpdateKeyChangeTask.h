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
#ifndef SECAUTH_BEGIN_UPDATE_KEY_CHANGE_TASK_H
#define SECAUTH_BEGIN_UPDATE_KEY_CHANGE_TASK_H

#include <opendnp3/master/IMasterTask.h>
#include <opendnp3/master/TaskPriority.h>

#include <openpal/crypto/ICryptoProvider.h>
#include <openpal/container/StaticBuffer.h>

#include "secauth/AuthSizes.h"
#include "secauth/master/IMasterApplicationSA.h"
#include "secauth/master/BeginUpdateKeyChangeCallbackT.h"

namespace secauth
{

/**
* First step in changing a user's update key
*/
class BeginUpdateKeyChangeTask : public opendnp3::IMasterTask
{

public:

	BeginUpdateKeyChangeTask(
	    const std::string& username,
	    IMasterApplicationSA& application,
	    openpal::Logger logger,
	    const opendnp3::TaskConfig& config,
	    openpal::ICryptoProvider& crypto,
	    const BeginUpdateKeyChangeCallbackT& callback
	);


	virtual bool IsAuthTask() const override final
	{
		return true;
	}

	virtual char const* Name() const override final
	{
		return "Begin Update Key Change";
	}

	virtual bool IsRecurring() const override final
	{
		return false;
	}

	virtual bool BuildRequest(opendnp3::APDURequest& request, uint8_t seq) override final;

	virtual int Priority() const override final
	{
		return opendnp3::priority::USER_STATUS_CHANGE;
	}

	virtual bool BlocksLowerPriority() const override final
	{
		return false;
	}

private:
	const std::string m_username;
	openpal::ICryptoProvider* m_crypto;
	BeginUpdateKeyChangeCallbackT m_callback;

	openpal::StaticBuffer<AuthSizes::MAX_CHALLENGE_DATA_SIZE> m_challengeBuffer;
	openpal::RSlice m_challengeDataView;

	virtual void Initialize() override final {}

	virtual opendnp3::MasterTaskType GetTaskType() const override final
	{
		return opendnp3::MasterTaskType::USER_TASK;
	}

	virtual bool IsEnabled() const override final
	{
		return true;
	}

	virtual opendnp3::IMasterTask::ResponseResult ProcessResponse(const opendnp3::APDUResponseHeader& response, const openpal::RSlice& objects) override final;

	virtual IMasterTask::TaskState OnTaskComplete(opendnp3::TaskCompletion result, openpal::MonotonicTimestamp now) override final;


	/// --- helpers -----

	opendnp3::IMasterTask::ResponseResult ProcessErrorResponse(const openpal::RSlice& objects);

	opendnp3::IMasterTask::ResponseResult ProcessDataResponse(const openpal::RSlice& objects);

};


} //end ns


#endif
