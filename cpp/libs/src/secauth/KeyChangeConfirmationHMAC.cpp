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
#include "KeyChangeConfirmationHMAC.h"

#include <openpal/serialization/Format.h>

#include "secauth/StringConversions.h"
#include "secauth/outstation/OutstationErrorCodes.h"

#include <openpal/crypto/SecureCompare.h>

using namespace openpal;
using namespace opendnp3;

namespace secauth
{
KeyChangeHMACData::KeyChangeHMACData(
    const std::string& name_,
    const openpal::RSlice& senderNonce_,
    const openpal::RSlice& receiverNonce_,
    uint32_t keyChangeSeqNum_,
    opendnp3::User user_
) :
	name(name_),
	senderNonce(senderNonce_),
	receiverNonce(receiverNonce_),
	keyChangeSeqNum(keyChangeSeqNum_),
	user(user_)
{}

KeyChangeConfirmationHMAC::KeyChangeConfirmationHMAC(openpal::IHMACAlgo& algorithm) : m_algorithm(&algorithm)
{}

bool KeyChangeConfirmationHMAC::ComputeAndCompare(
    const openpal::RSlice& key,
    const KeyChangeHMACData& data,
    openpal::IHMACAlgo& algorithm,
    const openpal::RSlice& expectedHMAC,
    std::error_code& ec
)
{
	// verify the HMAC value
	KeyChangeConfirmationHMAC calc(algorithm);

	auto hmac = calc.Compute(key, data, ec);

	if (ec)
	{
		return false;
	}

	if (!SecureEquals(hmac, expectedHMAC))
	{
		ec = make_error_code(OutstationError::KEY_CHANGE_CONFIRMATION_HMAC_MISMATCH);
		return false;
	}

	return true;
}

openpal::RSlice KeyChangeConfirmationHMAC::Compute(
    const openpal::RSlice& key,
    const KeyChangeHMACData& data,
    std::error_code& ec)
{
	openpal::StaticBuffer<6> ksqAndUser;

	{
		auto dest = ksqAndUser.GetWSlice();
		Format::Many(dest, data.keyChangeSeqNum, data.user.GetId());
	}

	auto outputDest = m_buffer.GetWSlice();

	return m_algorithm->Calculate(key,
	{	AsSlice(data.name), data.senderNonce, data.receiverNonce, ksqAndUser.ToRSlice() },
	outputDest,
	ec
	                             );
}

}



