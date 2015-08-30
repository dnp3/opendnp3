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

#include "MasterUserDatabase.h"

using namespace opendnp3;

namespace secauth
{
void MasterUserDatabase::EnumerateUsers(const std::function<void(const opendnp3::User)>& fun) const
{
	for (auto & pair : userMap)
	{
		fun(User(pair.first));
	}
}

UpdateKey::View MasterUserDatabase::GetUpdateKeyView(const opendnp3::User& user) const
{
	auto iter = this->userMap.find(user.GetId());
	if (iter == userMap.end())
	{
		return UpdateKey::View();
	}
	else
	{
		return iter->second->GetView();
	}
}

bool MasterUserDatabase::UserExists(const User& user) const
{
	auto iter = this->userMap.find(user.GetId());
	return iter != userMap.end();
}

bool MasterUserDatabase::AddUser(const User& user, const UpdateKey& key)
{
	if (key.GetView().algorithm != KeyWrapAlgorithm::UNDEFINED)
	{
		userMap[user.GetId()] = std::unique_ptr<UpdateKey>(new UpdateKey(key));
		return true;
	}
	else
	{
		return false;
	}
}
}



