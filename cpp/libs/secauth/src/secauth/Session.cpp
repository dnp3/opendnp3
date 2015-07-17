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

#include "Session.h"

#include <openpal/executor/TimeDuration.h>

using namespace openpal;
using namespace opendnp3;

namespace secauth
{		
	Session::Session(openpal::IMonotonicTimeSource& timeSource, const openpal::TimeDuration& duration, uint32_t maxAuthCount) :
		pTimeSource(&timeSource),
		DURATION(duration),
		MAX_AUTH_COUNT(maxAuthCount),
		status(KeyStatus::NOT_INIT),
		authCount(0)
	{
	
	}	

	opendnp3::KeyStatus Session::IncrementAuthCount()
	{
		if (this->status == KeyStatus::OK)
		{
			++this->authCount;
			if (this->authCount >= MAX_AUTH_COUNT)
			{
				this->status = KeyStatus::COMM_FAIL;
			}
		}

		return this->status;
	}

	opendnp3::KeyStatus Session::CheckTimeValidity()
	{
		if ((this->status == KeyStatus::OK) && (this->expirationTime < pTimeSource->GetTime()))
		{
			this->status = KeyStatus::COMM_FAIL;			
		}

		return this->status;
	}

	void Session::SetKeys(const SessionKeysView& view)
	{
		this->authCount = 0;
		this->expirationTime = pTimeSource->GetTime().Add(this->DURATION);
		this->keys.SetKeys(view);		
		this->status = KeyStatus::OK;
	}

	opendnp3::KeyStatus Session::GetKeyStatus()
	{
		return this->CheckTimeValidity();
	}

	opendnp3::KeyStatus Session::TryGetKeyView(SessionKeysView& view)
	{
		auto result = this->CheckTimeValidity();
		if (result == KeyStatus::OK)
		{
			view = keys.GetView();
		}
		return result;
	}


}



