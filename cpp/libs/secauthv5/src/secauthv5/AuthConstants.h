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
#ifndef SECAUTHV5_AUTHCONSTANTS_H
#define SECAUTHV5_AUTHCONSTANTS_H

#include <openpal/util/Uncopyable.h>
#include <openpal/util/Comparisons.h>

#include <opendnp3/objects/Group120Var5.h>

#include <cstdint>

namespace secauthv5
{

struct AuthConstants : openpal::PureStatic
{	
	const static uint16_t MIN_CHALLENGE_DATA_SIZE = 4;
	const static uint16_t MAX_CHALLENGE_DATA_SIZE = 64;

	const static uint32_t MIN_SESSION_KEY_SIZE_BYTES = 16;
	const static uint32_t MAX_SESSION_KEY_SIZE_BYTES = 32;

	const static uint16_t MAX_HMAC_TRUNC_SIZE = 16;  // SH
	const static uint16_t MAX_HMAC_OUTPUT_SIZE = 32; // SHA256 output is 32 bytes

	static uint32_t GetBoundedSessionKeySize(uint32_t size)
	{
		return openpal::Bounded(size, MIN_SESSION_KEY_SIZE_BYTES, MAX_SESSION_KEY_SIZE_BYTES);
	}

	static bool SessionKeySizeWithinLimits(uint32_t size)
	{
		return openpal::WithinLimits(size, MIN_SESSION_KEY_SIZE_BYTES, MAX_SESSION_KEY_SIZE_BYTES);
	}

	static uint16_t GetBoundedChallengeSize(uint16_t size)
	{
		return openpal::Bounded(size, MIN_CHALLENGE_DATA_SIZE, MAX_CHALLENGE_DATA_SIZE);
	}
};

}

#endif

