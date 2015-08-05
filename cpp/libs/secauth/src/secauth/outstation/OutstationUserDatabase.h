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
#ifndef SECAUTH_OUTSTATIONUSERDATABASE_H
#define SECAUTH_OUTSTATIONUSERDATABASE_H

#include "IOutstationUserDatabase.h"

#include "secauth/UpdateKey.h"
#include "secauth/outstation/Permissions.h"
#include "secauth/outstation/IOutstationApplicationSA.h"

#include <openpal/container/Buffer.h>

#include <map>
#include <memory>

namespace secauth
{

/** 
	A very simple update key store for the default user
*/
class OutstationUserDatabase : public IOutstationUserDatabase
{
	struct UserData
	{
	
		UserData(const UpdateKey& key_, const std::string& userName_, const Permissions& permissions_) :
			key(key_),
			userName(userName_),
			permissions(permissions_)
		{}

		UserData() : key(), permissions(Permissions::AllowNothing())
		{}

		UpdateKey key;
		std::string userName;
		Permissions permissions;
	};

	public:		
		
		virtual bool GetUpdateKey(const opendnp3::User& user, opendnp3::UpdateKeyMode& type, openpal::RSlice& key) const override final;

		virtual bool GetUpdateKeyType(const opendnp3::User& user, opendnp3::UpdateKeyMode& type) const override final;

		virtual bool IsAuthorized(const opendnp3::User& user, opendnp3::FunctionCode code) const override final;

		virtual bool UserExists(const opendnp3::User& user) const override final;

		virtual bool UserExists(const std::string& userName) const override final;

		virtual bool Delete(const std::string& userName, opendnp3::User& userOut) override final;

		
		void AddUser(opendnp3::User user, const std::string& userName, const UpdateKey& key, Permissions permissions);

	private:	

		typedef std::map<uint16_t, UserData> UserMap;

		UserMap::const_iterator FindByName(const std::string& userName) const;

		UserMap::iterator FindByName(const std::string& userName);

		UserMap userMap;
};

}

#endif

