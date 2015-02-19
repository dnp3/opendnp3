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

#include "KeyChangeState.h"

#include <opendnp3/gen/KeyWrapAlgorithm.h>
#include <opendnp3/LogLevels.h>

#include <openpal/logging/LogMacros.h>
#include <openpal/crypto/SecureCompare.h>

using namespace openpal;
using namespace opendnp3;

namespace secauthv5
{

	KeyChangeState::KeyChangeState(uint16_t userNum, uint16_t challengeSize_, openpal::Logger logger_, openpal::ICryptoProvider& provider) :		
		challengeSize(AuthConstants::GetBoundedChallengeSize(challengeSize_)),
		logger(logger_),
		pProvider(&provider),
		keyChangeSeqNum(0)
	{}

	bool KeyChangeState::FormatKeyStatusResponse(
			opendnp3::HeaderWriter& writer,
			const User& user,
			opendnp3::HMACType hmacType,
			opendnp3::KeyWrapAlgorithm keyWrapAlgo,
			opendnp3::KeyStatus status, 
			const openpal::ReadBufferView& hmac
		)
	{				
		if (pProvider->GetSecureRandom(challengeData.GetWriteBuffer(challengeSize)))
		{
			++keyChangeSeqNum;			

			this->lastUser = user;

			statusRsp.keyChangeSeqNum = keyChangeSeqNum;
			statusRsp.userNum = user.GetId();
			statusRsp.keywrapAlgorithm = keyWrapAlgo;
			statusRsp.keyStatus = status;
			statusRsp.hmacType = hmacType;
			statusRsp.challengeData = challengeData.ToReadOnly(challengeSize);
			statusRsp.hmacValue = hmac;

			return writer.WriteFreeFormat(statusRsp);
		}
		else
		{
			SIMPLE_LOG_BLOCK(logger, flags::ERR, "Unable to get secure random data for KeyStatus response");
			return false;
		}
	}

	bool KeyChangeState::EqualsLastStatusResponse(const openpal::ReadBufferView& unwrappedKeyStatus)
	{
		Group120Var5 copy(statusRsp);
		copy.hmacValue = ReadBufferView::Empty(); // exclude the HMAC from the comparison

		const uint32_t MAX_SIZE = Group120Var5::FIXED_BASE_SIZE + AuthConstants::MAX_CHALLENGE_DATA_SIZE;
		openpal::StaticBuffer<MAX_SIZE> buffer;
		
		if (!Group120Var5::Write(copy, buffer.GetWriteBuffer()))
		{
			SIMPLE_LOG_BLOCK(logger, flags::ERR, "Unable to write last response to buffer");
			return false;
		}
		else
		{	
			// this is what we sent
			auto sent = buffer.ToReadOnly(copy.Size());
			// the unwrapped data may be larger due to padding so truncate it to the length of what we're expecting before comparing
			auto unwrappedTrunc = unwrappedKeyStatus.Take(sent.Size());

			return SecureEquals(sent, unwrappedTrunc);
		}
	}

}