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
#ifndef ASIODNP3_IMASTERSA_H
#define ASIODNP3_IMASTERSA_H

#include "asiodnp3/IMaster.h"

#include <opendnp3/app/User.h>
#include <secauth/UpdateKey.h>

#include <secauth/master/UserStatusChange.h>
#include <secauth/master/BeginUpdateKeyChangeCallbackT.h>
#include <secauth/master/FinishUpdateKeyChangeArgs.h>

namespace asiodnp3
{

/**
* Extends the vanilla DNP3 master interface with secure authentication features.
*/
class IMasterSA : public IMaster
{
public:

	virtual ~IMasterSA() {}

	/**
	*	Add a user to the outstation. This is normally only done during initialization.
	*/
	virtual void AddUser(opendnp3::User user, const secauth::UpdateKey& key) = 0;

	/**
	*	Begins a users status change operation. The master just acts as a pass through for this operation
	*/
	virtual void ChangeUserStatus(const secauth::UserStatusChange& userStatusChange,  const opendnp3::TaskConfig& config = opendnp3::TaskConfig::Default()) = 0;

	/**
	*  Begins the first step in changing a user's update key
	*
	*  At the moment this only support symmetric with KeyChangeMethod::AES_256_SHA256_HMAC
	*/
	virtual void BeginUpdateKeyChange(const std::string& userName, const opendnp3::TaskConfig& config, const secauth::BeginUpdateKeyChangeCallbackT& handler) = 0;

	/**
	*	Begin the 2nd and final step in change a user's update key
	*
	*/
	virtual void FinishUpdateKeyChange(const secauth::FinishUpdateKeyChangeArgs& args, const opendnp3::TaskConfig& config) = 0;


};

}

#endif

