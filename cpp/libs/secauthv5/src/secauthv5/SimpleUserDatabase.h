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
#ifndef SECAUTHV5_SIMPLEUPDATEKEYSTORE_H
#define SECAUTHV5_SIMPLEUPDATEKEYSTORE_H

#include "IUserDatabase.h"

#include <openpal/container/DynamicBuffer.h>

#include <map>
#include <memory>

namespace secauthv5
{

/** 
	A very simple update key store for the default user
*/
class SimpleUserDatabase : public IUserDatabase
{
	public:		
		
		virtual bool GetUpdateKey(const User& user, UpdateKeyType& type, openpal::ReadBufferView& key) const override final;

		virtual bool GetUpdateKeyType(const User& user, UpdateKeyType& type) const override final;

		virtual bool IsAuthorized(const User& user, opendnp3::FunctionCode code) const override final;

		virtual void EnumerateUsers(std::function<void(User)> fun) const override final;

		// copies the update key into the key store permanently
		void ConfigureUser(const User& user, UpdateKeyType type, const openpal::ReadBufferView& key);

	private:

		typedef std::pair<UpdateKeyType, std::unique_ptr<openpal::DynamicBuffer>> StoredUserData;
		std::map<uint16_t, StoredUserData> userMap;
};

}

#endif

