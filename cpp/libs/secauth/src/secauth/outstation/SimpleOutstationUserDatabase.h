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
#ifndef SECAUTH_SIMPLEOUTSTATIONUSERDATABASE_H
#define SECAUTH_SIMPLEOUTSTATIONUSERDATABASE_H

#include "IOutstationUserDatabase.h"

#include "secauth/UpdateKey.h"
#include "secauth/outstation/Permissions.h"

#include <openpal/container/Buffer.h>

#include <map>
#include <memory>

namespace secauth
{

/** 
	A very simple update key store for the default user
*/
class SimpleOutstationUserDatabase : public IOutstationUserDatabase
{
	struct UserData
	{
	
		UserData(const UpdateKey& key_, const Permissions& permissions_) :
			key(key_),
			permissions(permissions_)
		{}

		UserData() : key(), permissions(Permissions::AllowNothing())
		{}

		UpdateKey key;
		Permissions permissions;
	};

	public:		
		
		virtual bool GetUpdateKey(const User& user, opendnp3::UpdateKeyMode& type, openpal::ReadBufferView& key) const override final;

		virtual bool GetUpdateKeyType(const User& user, opendnp3::UpdateKeyMode& type) const override final;

		virtual bool IsAuthorized(const User& user, opendnp3::FunctionCode code) const override final;

		virtual bool UserExists(const User& user) const override final;

		// copies the update key into the key store permanently
		// fails if the update key is invalid
		bool ConfigureUser(const User& user, const UpdateKey& key, const Permissions& permissions);

	private:		

		std::map<uint16_t, UserData> userMap;
};

}

#endif

