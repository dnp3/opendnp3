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
#ifndef SECAUTH_SESSIONKEYTASK_H
#define SECAUTH_SESSIONKEYTASK_H

#include <opendnp3/master/IMasterTask.h>
#include <opendnp3/master/TaskPriority.h>

#include "secauth/User.h"
#include "secauth/SessionKeys.h"
#include "secauth/master/MasterSecurityState.h"
#include "secauth/master/KeyWrap.h"


namespace secauth
{

	/**
	* An auto event scan task that happens when the master sees the event IIN bits
	*/
	class SessionKeyTask : public opendnp3::IMasterTask
	{
		enum State
		{
			GetStatus,
			ChangeKey
		};


	public:

		SessionKeyTask(	opendnp3::IMasterApplication& application, 
						openpal::TimeDuration retryPeriod, 
						openpal::Logger logger, 
						const User& user, 
						MSState& msstate);

		virtual bool IsAuthTask() const override final { return true; }

		virtual char const* Name() const override final { return "Change Session Keys"; }
		
		virtual bool IsRecurring() const override final { return true; }

		virtual void BuildRequest(opendnp3::APDURequest& request, uint8_t seq) override final;

		virtual int Priority() const override final { return opendnp3::priority::SESSION_KEY; }		

		virtual bool BlocksLowerPriority() const { return true; }	

	private:	

		openpal::TimeDuration retryPeriod;
		User user;
		MSState* pmsstate;
		State state;
		SessionKeys keys;
		uint32_t keyChangeSeqNum;
		KeyWrapBuffer keyWrapBuffer;
		openpal::ReadBufferView txKeyWrapASDU;

		virtual void Initialize() override final;

		virtual opendnp3::MasterTaskType GetTaskType() const override final { return opendnp3::MasterTaskType::SET_SESSION_KEYS; }

		virtual bool IsEnabled() const override final { return true; }

		virtual opendnp3::IMasterTask::ResponseResult _OnResponse(const opendnp3::APDUResponseHeader& response, const openpal::ReadBufferView& objects) override final;

		virtual void OnResponseError(openpal::MonotonicTimestamp now) override final;

		virtual void OnResponseOK(openpal::MonotonicTimestamp now) override final;

		virtual void _OnResponseTimeout(openpal::MonotonicTimestamp now) override final;

		virtual void _OnLowerLayerClose(openpal::MonotonicTimestamp now) override final;

		/// ----- private helpers ------

		void BuildStatusRequest(opendnp3::APDURequest& request, uint8_t seq);

		void BuildSessionKeyRequest(opendnp3::APDURequest& request, uint8_t seq);

		opendnp3::IMasterTask::ResponseResult OnStatusResponse(const opendnp3::APDUResponseHeader& response, const openpal::ReadBufferView& objects);

		opendnp3::IMasterTask::ResponseResult OnChangeResponse(const opendnp3::APDUResponseHeader& response, const openpal::ReadBufferView& objects);
	};


} //end ns


#endif
