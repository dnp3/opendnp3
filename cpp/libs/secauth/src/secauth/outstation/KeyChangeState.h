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
#ifndef SECAUTH_KEYCHANGESTATE_H
#define SECAUTH_KEYCHANGESTATE_H

#include <openpal/container/StaticBuffer.h>
#include <openpal/crypto/ICryptoProvider.h>
#include <openpal/logging/Logger.h>

#include <opendnp3/objects/Group120.h>
#include <opendnp3/app/APDUResponse.h>
#include <opendnp3/app/User.h>

#include "secauth/AuthSizes.h"

namespace secauth
{

class KeyChangeState
{
	public:

	KeyChangeState(uint16_t userNum, uint16_t challengeSize, openpal::Logger logger, openpal::ICryptoProvider& provider);
	
	// Formats the key status response	
	bool FormatKeyStatusResponse(
		opendnp3::HeaderWriter& writer,
		const opendnp3::User& user,
		opendnp3::HMACType hmacType,
		opendnp3::KeyWrapAlgorithm keyWrapAlgo,
		opendnp3::KeyStatus status,	
		const openpal::RSlice& hmac = openpal::RSlice::Empty()
	);

	// Securely compare the specified serialized object data to the serialization
	// of the last status response
	bool EqualsLastStatusResponse(const openpal::RSlice& object);

	bool CheckUserAndKSQMatches(const opendnp3::User& user, uint32_t keyChangeSeq);

	private:

	opendnp3::User lastUser;
	uint16_t challengeSize;
	openpal::Logger logger;
	openpal::ICryptoProvider* pProvider;
	uint32_t keyChangeSeqNum;
	openpal::StaticBuffer<AuthSizes::MAX_CHALLENGE_DATA_SIZE> challengeData;
	opendnp3::Group120Var5 statusRsp;
	
	
};

}

#endif

