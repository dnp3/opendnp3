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

#include "SessionStore.h"

using namespace opendnp3;
using namespace openpal;

namespace secauth
{				
	SessionStore::SessionStore(
			IMonotonicTimeSource& timeSource,
			openpal::TimeDuration sessionKeyValidity_,
			uint32_t maxAuthMessageCount_
	) :		
		pTimeSource(&timeSource),
		sessionKeyValidity(sessionKeyValidity_),
		maxAuthMessageCount(maxAuthMessageCount_)
	{
		
		
	}

	void SessionStore::Clear()
	{
		sessionMap.clear();
	}

	opendnp3::KeyStatus SessionStore::IncrementAuthCount(const User& user)
	{
		auto iter = sessionMap.find(user.GetId());
		if (iter == sessionMap.end())
		{
			return KeyStatus::UNDEFINED;
		}
		else
		{
			return iter->second->IncrementAuthCount();
		}
	}
	
	void SessionStore::SetSessionKeys(const User& user, const SessionKeysView& view)
	{
		auto iter = sessionMap.find(user.GetId());
		if (iter == sessionMap.end())
		{			
			auto session = std::unique_ptr<Session>(new Session(*pTimeSource));
			session->SetKeys(view);
			sessionMap[user.GetId()] = std::move(session);
		}
		else
		{
			iter->second->SetKeys(view);			
		}		
	}

	opendnp3::KeyStatus SessionStore::GetSessionKeys(const User& user, SessionKeysView& view)
	{
		auto iter = sessionMap.find(user.GetId());
		if (iter == sessionMap.end())
		{			
			return KeyStatus::UNDEFINED;
		}
		else
		{
			return iter->second->GetKeys(view);			
		}
	}

	opendnp3::KeyStatus SessionStore::GetSessionKeyStatus(const User& user)
	{
		auto iter = sessionMap.find(user.GetId());
		if (iter == sessionMap.end())
		{			
			// initialize new session info
			sessionMap[user.GetId()] = std::unique_ptr<Session>(new Session(*pTimeSource));
			return KeyStatus::NOT_INIT;			
		}
		else
		{
			return iter->second->GetKeyStatus();			
		}
	}
				
}



