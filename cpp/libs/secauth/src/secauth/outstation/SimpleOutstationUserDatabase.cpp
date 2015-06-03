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

#include "SimpleOutstationUserDatabase.h"

using namespace openpal;
using namespace opendnp3;

namespace secauth
{
		
bool SimpleOutstationUserDatabase::GetUpdateKey(const User& user, UpdateKeyMode& type, openpal::ReadBufferView& key) const
{
	auto iter = this->userMap.find(user.GetId());
	if (iter == userMap.end())
	{
		return false;
	}
	else
	{
		type = iter->second.first;
		key = iter->second.second->ToReadOnly();
		return true;
	}
}

bool SimpleOutstationUserDatabase::GetUpdateKeyType(const User& user, UpdateKeyMode& type) const
{
	auto iter = this->userMap.find(user.GetId());
	if (iter == userMap.end())
	{
		return false;
	}
	else
	{
		type = iter->second.first;		
		return true;
	}
}

bool SimpleOutstationUserDatabase::IsAuthorized(const User& user, opendnp3::FunctionCode code) const
{
	// for the time being, if the user exists they are authorized
	return UserExists(user);
}

bool SimpleOutstationUserDatabase::UserExists(const User& user) const
{
	auto iter = this->userMap.find(user.GetId());
	return iter != userMap.end();
}

void SimpleOutstationUserDatabase::ConfigureUser(const User& user, UpdateKeyMode type, const openpal::ReadBufferView& key)
{
	userMap[user.GetId()] = StoredUserData(type, std::unique_ptr<openpal::Buffer>(new openpal::Buffer(key)));
}

}



