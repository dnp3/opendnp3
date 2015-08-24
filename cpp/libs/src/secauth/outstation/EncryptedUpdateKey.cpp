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

#include "EncryptedUpdateKey.h"

#include <openpal/crypto/SecureCompare.h>

#include "secauth/StringConversions.h"
#include "secauth/outstation/OutstationErrorCodes.h"

using namespace openpal;
using namespace opendnp3;

namespace secauth
{

bool EncryptedUpdateKey::DecryptAndVerify(
	IKeyWrapAlgo& algorithm, 
	const RSlice& kek,
	const RSlice& encryptedData,
	const std::string& username, 
	const RSlice& outstationChallengeData, 
	UpdateKey& updateKey,
	std::error_code& ec)
{
	// buffer where the decrypted data is written
	openpal::StaticBuffer<AuthSizes::MAX_UPDATE_KEY_UNWRAP_BUFFER_SIZE> buffer;

	auto dest = buffer.GetWSlice();	
	auto unwrapped = algorithm.UnwrapKey(kek, encryptedData, dest, ec);

	if (ec)
	{
		return false;
	}

	const auto MIN_UNWRAPPED_SIZE = username.size() + AuthSizes::MAX_UPDATE_KEY_SIZE_BYTES;

	if (unwrapped.Size() < MIN_UNWRAPPED_SIZE)
	{
		ec = make_error_code(OutstationError::BAD_UNWRAPPPED_UPDATE_KEY_DATA_SIZE);
		return false;
	}

	// the name should be the same as what was sent previously
	auto name = ToString(unwrapped.Take(username.size()));

	// this comparison does not need to be secure since it is not a secret
	if (name != username)
	{
		ec = make_error_code(OutstationError::DECRYPTED_USERNAME_MISMATCH);
		return false;
	}

	unwrapped.Advance(username.size()); // now points to beginning of the key

	// next locate the challenge data, which is whatever is after the key
	// remove any padding by truncating to the size of the expected challenge, although it may be less data
	auto challengeData = unwrapped.Skip(AuthSizes::MAX_UPDATE_KEY_SIZE_BYTES).Take(outstationChallengeData.Size());

	if (!SecureEquals(challengeData, outstationChallengeData))
	{
		ec = make_error_code(OutstationError::CHALLENGE_DATA_MISMATCH);
		return false;
	}	
	
	// we're fully authenticated so this data is the key
	updateKey = UpdateKey(unwrapped.Take(AuthSizes::MAX_UPDATE_KEY_SIZE_BYTES));

	return true;
}

}



