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
#ifndef ASIODNP3_MASTERSTACKSA_H
#define ASIODNP3_MASTERSTACKSA_H

#include "MasterStackBase.h"
#include "IMasterSA.h"

#include <secauth/master/MasterAuthContext.h>
#include <secauth/master/MasterAuthStackConfig.h>
#include <secauth/master/IMasterApplicationSA.h>

#include <openpal/crypto/ICryptoProvider.h>

namespace asiodnp3
{


class MasterStackSA : public MasterStackBase<IMasterSA>
{
public:
		
	MasterStackSA(
		const char* id,
		openpal::LogRoot& root,
		asiopal::ASIOExecutor& executor,
		opendnp3::ISOEHandler& SOEHandler,
		secauth::IMasterApplicationSA& application,
		const secauth::MasterAuthStackConfig& config,
		IStackLifecycle& lifecycle,
		opendnp3::ITaskLock& taskLock,		
		openpal::ICryptoProvider& crypto
	);	

	virtual void AddUser(opendnp3::User user, const secauth::UpdateKey& key) override final;

	virtual void ChangeUserStatus(const secauth::UserStatusChange& userStatusChange, const opendnp3::TaskConfig& config = opendnp3::TaskConfig::Default()) override final;

	virtual void BeginUpdateKeyChange(const std::string& username, const opendnp3::TaskConfig& config, const secauth::BeginUpdateKeyChangeCallbackT& callback) override final;

	virtual void FinishUpdateKeyChange(const secauth::FinishUpdateKeyChangeArgs& args, const opendnp3::TaskConfig& config) override final;
	
private:	
	
	secauth::MAuthContext mcontext;	
};

}

#endif

