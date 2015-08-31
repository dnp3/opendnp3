/*
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
#ifndef SECAUTH_AUTHSIZES_H
#define SECAUTH_AUTHSIZES_H

#include <openpal/util/Uncopyable.h>
#include <openpal/util/Comparisons.h>
#include <opendnp3/objects/Group120.h>

#include <cstdint>

// an expression of a size padded to an even multiple of 8 bytes
#define MACRO_PADDED_MODULO(mod, size) (((size % mod) == 0) ? size : (size + (mod - (size % 8))))

namespace secauth
{

struct AuthSizes : openpal::StaticOnly
{

public:

	// Although the specification does name one we define one and enforce it to allow static buffer allocation
	const static uint32_t MAX_USER_NAME_SIZE = 32;

	const static uint32_t MIN_CHALLENGE_DATA_SIZE = 4;
	const static uint32_t MAX_CHALLENGE_DATA_SIZE = 16;

	// TODO verify the default sizes
	const static uint32_t DEFAULT_CHALLENGE_SIZE = 4;
	const static uint32_t DEFAULT_SESSION_KEY_CHALLENGE_SIZE = 4;
	const static uint32_t DEFAULT_UPDATE_KEY_CHALLENGE_SIZE = 4;

#define MACRO_WITHIN_CHALLENGE_LIMITS(size) static_assert((size >= MIN_CHALLENGE_DATA_SIZE) && (size <= MAX_CHALLENGE_DATA_SIZE), "challenge size outside limits");

	MACRO_WITHIN_CHALLENGE_LIMITS(DEFAULT_CHALLENGE_SIZE);
	MACRO_WITHIN_CHALLENGE_LIMITS(DEFAULT_SESSION_KEY_CHALLENGE_SIZE);
	MACRO_WITHIN_CHALLENGE_LIMITS(DEFAULT_UPDATE_KEY_CHALLENGE_SIZE);

	const static uint32_t MIN_SESSION_KEY_SIZE_BYTES = 16;
	const static uint32_t MAX_SESSION_KEY_SIZE_BYTES = 32;

	const static uint32_t MIN_UPDATE_KEY_SIZE_BYTES = 16;
	const static uint32_t MAX_UPDATE_KEY_SIZE_BYTES = 32;

	const static uint16_t MAX_HMAC_TRUNC_SIZE = 16;  // SHA256 Trunc 16 has the longest length
	const static uint16_t MAX_HMAC_OUTPUT_SIZE = 32; // SHA256 output is 32 bytes

	// the maximum size of an outstation challenge response
	// 4 bytes header + 6 bytes obj header  = 10
	const static uint32_t MAX_OUTSTATION_CHALLENGE_RESPONSE_FRAGMENT_SIZE = 10 + opendnp3::Group120Var1::MIN_SIZE + MAX_CHALLENGE_DATA_SIZE;

	// 2 bytes header + 6 bytes obj header = 8
	const static uint8_t FREE_FORMAT_HEADER_SIZE = 8;

	// the maximum size of a master challenge reply
	const static uint32_t MAX_MASTER_CHALLENGE_REPLY_FRAG_SIZE = FREE_FORMAT_HEADER_SIZE + opendnp3::Group120Var2::MIN_SIZE + MAX_HMAC_TRUNC_SIZE;

	const static uint32_t MAX_SESSION_KEY_WRAP_BUFFER_SIZE = MACRO_PADDED_MODULO(
	            8, // AES key wrap requires data to be padded modulo 8
	            (2 + 2 * MAX_SESSION_KEY_SIZE_BYTES + opendnp3::Group120Var5::MIN_SIZE + MAX_CHALLENGE_DATA_SIZE) // the maximum unpadded space required to unwrap the session key
	        );

	const static uint32_t MAX_UPDATE_KEY_UNWRAP_BUFFER_SIZE = MACRO_PADDED_MODULO(
	            8, // AES key wrap requires data to be padded modulo 8
	            (8 + MAX_USER_NAME_SIZE + MAX_UPDATE_KEY_SIZE_BYTES + MAX_CHALLENGE_DATA_SIZE)	 // the maximum unpadded space required to unwrap the update key
	        );

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

