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

#include "SessionKeyStore.h"

using namespace opendnp3;
using namespace openpal;

namespace secauthv5
{
	SessionKeyState::SessionKeyState() :
		status(KeyStatus::NOT_INIT),
		authCount(0),
		authCountMax(0)
	{}		
	
	SessionKeyStore::SessionKeyStore(IMonotonicTimeSource& timeSource) :
		pTimeSource(&timeSource)
	{
	
	}

	opendnp3::KeyStatus SessionKeyStore::IncrementAuthCount(const User& user)
	{
		auto iter = stateMap.find(user.GetId());
		if (iter == stateMap.end())
		{
			return KeyStatus::NOT_INIT;
		}
		else
		{
			return this->IncrementAuthCount(*iter->second);
		}
	}

	opendnp3::KeyStatus SessionKeyStore::GetSessionKeys(const User& user, SessionKeysView& view)
	{
		auto iter = stateMap.find(user.GetId());
		if (iter == stateMap.end())
		{
			// create new session key status for this user
			stateMap[user.GetId()] = std::make_unique<SessionKeyState>();
			return KeyStatus::NOT_INIT;
		}
		else
		{
			auto status = this->CheckTimeValidity(*iter->second);
			if (status == KeyStatus::OK)
			{
				view = iter->second->keys.GetView();
			}

			return status;
		}
	}

	opendnp3::KeyStatus SessionKeyStore::CheckTimeValidity(SessionKeyState& state)
	{
		if (state.status == KeyStatus::OK)
		{
			if (state.expirationTime < pTimeSource->GetTime())
			{
				state.status = KeyStatus::COMM_FAIL;
			}
		}

		return state.status;
	}

	opendnp3::KeyStatus SessionKeyStore::IncrementAuthCount(SessionKeyState& state)
	{
		if (state.status == KeyStatus::OK)
		{
			++state.authCount;
			if (state.authCount > state.authCountMax)
			{
				state.status = KeyStatus::COMM_FAIL;
			}
		}

		return state.status;
	}
}



