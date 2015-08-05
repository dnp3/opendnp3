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

#include <openpal/crypto/ICryptoProvider.h>

#include "secauth/SessionStore.h"
#include "secauth/master/KeyWrap.h"
#include "secauth/master/IMasterUserDatabase.h"


namespace secauth
{

	/**
	* An auto event scan task that happens when the master sees the event IIN bits
	*/
	class SessionKeyTask : public opendnp3::IMasterTask
	{
		enum ChangeState
		{
			GetStatus,
			ChangeKey
		};


	public:

		SessionKeyTask(
			opendnp3::IMasterApplication& application,
			openpal::TimeDuration retryPeriod,
			openpal::TimeDuration changeInterval,
			openpal::Logger logger,
			const opendnp3::User& user,
			openpal::ICryptoProvider& crypto,
			IMasterUserDatabase& userDB,			
			SessionStore& sessionStore);

		virtual bool IsAuthTask() const override final { return true; }

		virtual char const* Name() const override final { return "Change Session Keys"; }
		
		virtual bool IsRecurring() const override final { return true; }

		virtual bool BuildRequest(opendnp3::APDURequest& request, uint8_t seq) override final;

		virtual int Priority() const override final { return opendnp3::priority::SESSION_KEY; }		

		virtual bool BlocksLowerPriority() const override final { return false; }

	private:	

		openpal::TimeDuration retryPeriod;
		openpal::TimeDuration changeInterval;
		opendnp3::User user;
		openpal::ICryptoProvider* pCrypto;
		IMasterUserDatabase* pUserDB;
		SessionStore* pSessionStore;
		ChangeState state;
		SessionKeys keys;
		uint32_t keyChangeSeqNum;
		KeyWrapBuffer keyWrapBuffer;
		openpal::RSlice txKeyWrapASDU;

		virtual void Initialize() override final;

		virtual opendnp3::MasterTaskType GetTaskType() const override final { return opendnp3::MasterTaskType::SET_SESSION_KEYS; }

		virtual bool IsEnabled() const override final { return true; }

		virtual opendnp3::IMasterTask::ResponseResult ProcessResponse(const opendnp3::APDUResponseHeader& response, const openpal::RSlice& objects) override final;

		virtual IMasterTask::TaskState OnTaskComplete(opendnp3::TaskCompletion result, openpal::MonotonicTimestamp now) override final;

		/// ----- private helpers ------

		bool BuildStatusRequest(opendnp3::APDURequest& request, uint8_t seq);

		bool BuildSessionKeyRequest(opendnp3::APDURequest& request, uint8_t seq);

		opendnp3::IMasterTask::ResponseResult OnStatusResponse(const opendnp3::APDUResponseHeader& response, const openpal::RSlice& objects);

		opendnp3::IMasterTask::ResponseResult OnChangeResponse(const opendnp3::APDUResponseHeader& response, const openpal::RSlice& objects);
	};


} //end ns


#endif
