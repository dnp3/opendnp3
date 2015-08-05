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
#include <openpal/crypto/SecureCompare.h>

#include <opendnp3/LogLevels.h>
#include <opendnp3/app/User.h>


using namespace openpal;
using namespace opendnp3;

namespace secauth
{

	ChallengeState::ChallengeState(uint16_t challengeSize, uint32_t maxRxASDUSize) : 
		CHALLENGE_SIZE(AuthSizes::GetBoundedChallengeSize(challengeSize)),
		criticalASDU(maxRxASDUSize),
		seqNumber(0)
	{}
	
	bool ChallengeState::WriteChallenge(
		const openpal::RSlice& fragment,
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
		
		auto challengeDest = challengeDataBuffer.GetWriteBuffer(CHALLENGE_SIZE);
		std::error_code ec;
		auto random = crypto.GetSecureRandom(challengeDest, ec);
		if (ec)
		{
			SIMPLE_LOGGER_BLOCK(pLogger, flags::ERR, ec.message().c_str());
			return false;
		}		

		this->challengeData = random;

		++seqNumber; //increment the CSQ

		Group120Var1 challenge(
			seqNumber,
			User::UNKNOWN_ID,
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
		auto challengeFragmentDest = this->challengeFragmentBuffer.GetWriteBuffer();
		this->challengeFragment = asdu.CopyTo(challengeFragmentDest);

		if (this->challengeFragment.IsEmpty())
		{
			SIMPLE_LOGGER_BLOCK(pLogger, flags::ERR, "Insufficient space to persist challenge message");
			return false;
		}

		return true;
	}

	bool ChallengeState::VerifyAuthenticity(const openpal::RSlice& key, HMACProvider& provider, const openpal::RSlice& hmac, openpal::Logger logger)
	{
		if (provider.OutputSize() != hmac.Size())
		{
			FORMAT_LOG_BLOCK(logger, flags::WARN, "Received HMAC length of %u did not match expected HMAC length of %u", hmac.Size(), provider.OutputSize());
			return false;
		}		

		// calculate the hmac we expect		
		std::error_code ec;
		auto hmacCalc = provider.Compute(key, { challengeFragment, criticalASDU.GetFragment() }, ec);

		if (ec)
		{
			SIMPLE_LOG_BLOCK(logger, flags::ERR, ec.message().c_str());
			return false;
		}
		
		if (!openpal::SecureEquals(hmac, hmacCalc))
		{
			FORMAT_LOG_BLOCK(logger, flags::WARN, "HMAC comparison failed.");			
			return false;
		}

		return true;
	}
	

}



