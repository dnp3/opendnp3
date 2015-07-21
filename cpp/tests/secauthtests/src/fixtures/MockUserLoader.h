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
#ifndef SECAUTH_MOCKUSERLOADER_H
#define SECAUTH_MOCKUSERLOADER_H

#include "secauth/outstation/ISecAuthOutstationApplication.h"

#include <vector>
#include <functional>

namespace opendnp3
{

class MockUserLoader
{

public:

	void AddUser(User user, uint8_t keyRepeat, UpdateKeyMode mode, secauth::Permissions permissions = secauth::Permissions::AllowAll())
	{
		auto apply = [=](secauth::IUserSink& sink)
		{
			sink.Load(user, secauth::UpdateKey(keyRepeat, mode), permissions);
		};

		users.push_back(apply);
	}
														
	void LoadUsers(secauth::IUserSink& sink)
	{
		for (auto& fun : users)
		{
			fun(sink);
		}
	}

private:

	std::vector<std::function<void(secauth::IUserSink&)>> users;
};


}

#endif
