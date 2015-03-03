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

#include "ChallengeState.h"

#include <openpal/logging/LogMacros.h>
#include <opendnp3/LogLevels.h>

#include "secauthv5/User.h"

using namespace openpal;
using namespace opendnp3;

namespace secauthv5
{

	ChallengeState::ChallengeState(uint16_t challengeSize, uint32_t maxRxASDUSize) : 
		CHALLENGE_SIZE(AuthConstants::GetBoundedChallengeSize(challengeSize)),
		criticalASDU(maxRxASDUSize),
		seqNumber(0)
	{}
	
	bool ChallengeState::WriteChallenge(
		const openpal::ReadBufferView& fragment,
		const opendnp3::APDUHeader& header,
		opendnp3::APDUResponse& response,
		HMACType hmacType,
		openpal::ICryptoProvider& crypto,
		openpal::Logger* pLogger)
	{
		// copy the critical ASDU over to the buffer
		criticalASDU.SetASDU(header, fragment);
		
		// configure the basics of the response
		response.SetFunction(FunctionCode::AUTH_RESPONSE);
		response.SetControl(header.control);		
		
		if (!crypto.GetSecureRandom(challengeDataBuffer.GetWriteBuffer(CHALLENGE_SIZE)))
		{
			SIMPLE_LOGGER_BLOCK(pLogger, flags::ERR, "Unable to get secure random data for challenge");
			return false;
		}		

		this->challengeData = challengeDataBuffer.ToReadOnly(CHALLENGE_SIZE);

		++seqNumber; //increment the CSQ

		Group120Var1 challenge(
			seqNumber,
			User::DEFAULT_ID,
			hmacType,
			ChallengeReason::CRITICAL,
			challengeData
		);
						
		if (!response.GetWriter().WriteFreeFormat(challenge))
		{
			SIMPLE_LOGGER_BLOCK(pLogger, flags::ERR, "Insufficient space to write challenge to buffer");
			return false;
		}		

		// copy the fragment over so we can calculate the hmac later
		auto asdu = response.ToReadOnly();
		this->challengeFragment = asdu.CopyTo(this->challengeFragmentBuffer.GetWriteBuffer());

		if (this->challengeFragment.IsEmpty())
		{
			SIMPLE_LOGGER_BLOCK(pLogger, flags::ERR, "Insufficient space to persist challenge message");
			return false;
		}

		return true;
	}

}



