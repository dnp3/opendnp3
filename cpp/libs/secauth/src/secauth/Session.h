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
#ifndef SECAUTH_SESSION_H
#define SECAUTH_SESSION_H

#include "secauth/SessionKeys.h"

#include <opendnp3/gen/KeyStatus.h>
#include <openpal/executor/IMonotonicTimesource.h>

namespace secauth
{
	// All the info for a session
	class Session
	{
		public:

		// construct an uninitialized session
		Session(openpal::IMonotonicTimeSource& timeSource);			

		void Session::SetKeys(const SessionKeysView& view);

		opendnp3::KeyStatus GetKeyStatus();

		opendnp3::KeyStatus GetKeys(SessionKeysView& view);
		
		opendnp3::KeyStatus IncrementAuthCount();

		private:

		opendnp3::KeyStatus CheckTimeValidity();

		// TODO - make these configurable and change them
		static const uint32_t AUTH_COUNT_MAX = 100;
		static const uint8_t SESSION_KEY_EXP_MINUTES = 10;

		openpal::IMonotonicTimeSource* pTimeSource;
		

		opendnp3::KeyStatus status;
		SessionKeys keys;
		openpal::MonotonicTimestamp expirationTime;
		uint32_t authCount;						
	};

}

#endif

