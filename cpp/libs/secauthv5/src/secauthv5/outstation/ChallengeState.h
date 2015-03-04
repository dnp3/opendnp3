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
#ifndef SECAUTHV5_CHALLENGESTATE_H
#define SECAUTHV5_CHALLENGESTATE_H

#include <cstdint>

#include <openpal/container/StaticBuffer.h>

#include <opendnp3/app/APDUResponse.h>
#include <opendnp3/app/APDUHeader.h>

#include "secauthv5/HMACProvider.h"
#include "secauthv5/DeferredASDU.h"


namespace secauthv5
{

class ChallengeState
{
	public:

	ChallengeState(uint16_t challengeSize, uint32_t maxRxASDUSize);

	bool WriteChallenge(
		const openpal::ReadBufferView& fragment,
		const opendnp3::APDUHeader& header, 
		opendnp3::APDUResponse& response, 
		opendnp3::HMACType hmacType, 
		openpal::ICryptoProvider& crypto,
		openpal::Logger* pLogger
	);

	bool VerifyAuthenticity(
		const openpal::ReadBufferView& key,
		HMACProvider& provider, 
		const openpal::ReadBufferView& hmac, 
		openpal::Logger logger
	);

	openpal::ReadBufferView GetCriticalASDU() const { return criticalASDU.GetFragment(); }

	opendnp3::APDUHeader GetCriticalHeader() const { return criticalASDU.GetHeader(); }
	
private:

	const uint16_t CHALLENGE_SIZE;

	DeferredASDU criticalASDU;

	openpal::ReadBufferView challengeFragment;
	openpal::StaticBuffer<AuthConstants::MAX_CHALLENGE_RESPONSE_FRAGMENT_SIZE> challengeFragmentBuffer;

	openpal::ReadBufferView challengeData;
	openpal::StaticBuffer<AuthConstants::MAX_CHALLENGE_DATA_SIZE> challengeDataBuffer;
	
	uint32_t seqNumber;
};

}

#endif

