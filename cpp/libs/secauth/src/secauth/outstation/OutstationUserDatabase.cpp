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

#include "OutstationUserDatabase.h"

#include <algorithm>

using namespace openpal;
using namespace opendnp3;

namespace secauth
{
		
bool OutstationUserDatabase::GetUpdateKey(const User& user, UpdateKeyMode& type, openpal::RSlice& key) const
{
	auto iter = this->userMap.find(user.GetId());
	if (iter == userMap.end())
	{
		return false;
	}
	else
	{
		type = iter->second.key.GetKeyMode();
		key = iter->second.key.GetKeyView();
		return true;
	}
}

bool OutstationUserDatabase::GetUpdateKeyType(const User& user, UpdateKeyMode& type) const
{
	auto iter = this->userMap.find(user.GetId());
	if (iter == userMap.end())
	{
		return false;
	}
	else
	{
		type = iter->second.key.GetKeyMode();
		return true;
	}
}

bool OutstationUserDatabase::IsAuthorized(const User& user, opendnp3::FunctionCode code) const
{
	auto iter = this->userMap.find(user.GetId());
	if (iter == userMap.end())
	{
		return false;
	}
	else
	{
		return iter->second.permissions.IsAllowed(code);
	}	
}

bool OutstationUserDatabase::UserExists(const User& user) const
{
	auto iter = this->userMap.find(user.GetId());
	return iter != userMap.end();
}

bool OutstationUserDatabase::UserExists(const std::string& userName) const
{
	return FindByName(userName) != userMap.end();
}

bool OutstationUserDatabase::Delete(const std::string& userName, opendnp3::User& userOut)
{
	auto iter = FindByName(userName);
	if (iter == userMap.end())
	{
		return false;
	}
	else
	{
		userOut = User(iter->first);
		userMap.erase(iter);
		return true;
	}
}

bool OutstationUserDatabase::FindFreeUserId(opendnp3::User& user) const
{
	// the lowest admissable ID
	uint16_t lowest = 2;

	for (auto iter = userMap.begin(); iter != userMap.end(); ++iter)
	{
		if (iter->first < lowest)
		{
			continue;
		}

		if (iter->first > lowest)
		{
			user = User(lowest); // found a gap
			return true;
		}
		else
		{
			++lowest; // the values were equal
		}
	}

	return false;
}

void OutstationUserDatabase::AddUser(opendnp3::User user, const std::string& userName, const UpdateKey& key, Permissions permissions)
{
	if (key.IsValid())
	{
		userMap[user.GetId()] = UserData(key, userName, permissions);		
	}
}

OutstationUserDatabase::UserMap::const_iterator OutstationUserDatabase::FindByName(const std::string& userName) const
{
	auto byName = [&](const UserMap::value_type& value) { return value.second.userName == userName; };
	return std::find_if(userMap.begin(), userMap.end(), byName);
}

OutstationUserDatabase::UserMap::iterator OutstationUserDatabase::FindByName(const std::string& userName)
{
	auto byName = [&](const UserMap::value_type& value) { return value.second.userName == userName; };
	return std::find_if(userMap.begin(), userMap.end(), byName);
}

}



