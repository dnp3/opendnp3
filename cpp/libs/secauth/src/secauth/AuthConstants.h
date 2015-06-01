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
#ifndef SECAUTH_AUTHCONSTANTS_H
#define SECAUTH_AUTHCONSTANTS_H

#include <openpal/util/Uncopyable.h>
#include <openpal/util/Comparisons.h>
#include <opendnp3/objects/Group120.h>

#include <cstdint>

namespace secauth
{



struct AuthConstants : openpal::StaticOnly
{	
private:

	static uint32_t WithModuloEightPadding(uint32_t size);

public:

	const static uint32_t MIN_CHALLENGE_DATA_SIZE = 4;
	const static uint32_t MAX_CHALLENGE_DATA_SIZE = 16;

	const static uint32_t MIN_SESSION_KEY_SIZE_BYTES = 16;
	const static uint32_t MAX_SESSION_KEY_SIZE_BYTES = 32;

	const static uint16_t MAX_HMAC_TRUNC_SIZE = 16;  // SHA256 Trunc 16 has the longest length
	const static uint16_t MAX_HMAC_OUTPUT_SIZE = 32; // SHA256 output is 32 bytes

	// the maximum size of an outstation challenge response
	// 4 bytes header + 6 bytes obj header  = 10	
	const static uint32_t MAX_OUTSTATION_CHALLENGE_RESPONSE_FRAGMENT_SIZE = 10 + opendnp3::Group120Var1::MIN_SIZE + MAX_CHALLENGE_DATA_SIZE;

	// the maximum size of a master challenge reply
	// 2 bytes header + 6 bytes obj header = 8
	const static uint32_t MAX_MASTER_CHALLENGE_REPLY_FRAG_SIZE = 8 + opendnp3::Group120Var2::MIN_SIZE + MAX_HMAC_TRUNC_SIZE;

#define MACRO_MAX_KEY_WRAP_BUFFER (2 + 2 * AuthConstants::MAX_SESSION_KEY_SIZE_BYTES + opendnp3::Group120Var5::MIN_SIZE + AuthConstants::MAX_CHALLENGE_DATA_SIZE)
#define MACRO_MAX_KEY_WRAP_BUFFER_MOD8 (MACRO_MAX_KEY_WRAP_BUFFER % 8)	

	const static uint32_t MAX_KEY_WRAP_BUFFER_SIZE = (MACRO_MAX_KEY_WRAP_BUFFER_MOD8 == 0) ? MACRO_MAX_KEY_WRAP_BUFFER : (MACRO_MAX_KEY_WRAP_BUFFER + (8 - MACRO_MAX_KEY_WRAP_BUFFER_MOD8));
		
	static uint32_t GetBoundedSessionKeySize(uint32_t size)
	{
		return openpal::Bounded(size, MIN_SESSION_KEY_SIZE_BYTES, MAX_SESSION_KEY_SIZE_BYTES);
	}

	static bool SessionKeySizeWithinLimits(uint32_t size)
	{
		return openpal::WithinLimits(size, MIN_SESSION_KEY_SIZE_BYTES, MAX_SESSION_KEY_SIZE_BYTES);
	}

	static bool ChallengeDataSizeWithinLimits(uint32_t size)
	{
		return openpal::WithinLimits(size, MIN_CHALLENGE_DATA_SIZE, MAX_CHALLENGE_DATA_SIZE);
	}

	static uint32_t GetBoundedChallengeSize(uint32_t size)
	{
		return openpal::Bounded(size, MIN_CHALLENGE_DATA_SIZE, MAX_CHALLENGE_DATA_SIZE);
	}
};

}

#endif

