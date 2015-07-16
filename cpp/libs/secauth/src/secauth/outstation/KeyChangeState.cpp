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

namespace secauth
{

	KeyChangeState::KeyChangeState(uint16_t userNum, uint16_t challengeSize_, openpal::Logger logger_, openpal::ICryptoProvider& provider) :		
		challengeSize(AuthSizes::GetBoundedChallengeSize(challengeSize_)),
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
		auto dest = challengeData.GetWriteBuffer(challengeSize);

		std::error_code ec;
		auto challenge = pProvider->GetSecureRandom(dest, ec);
		if (ec)
		{
			SIMPLE_LOG_BLOCK(logger, flags::ERR, ec.message().c_str());
			return false;
		}

		++keyChangeSeqNum;			

		this->lastUser = user;

		statusRsp.keyChangeSeqNum = keyChangeSeqNum;
		statusRsp.userNum = user.GetId();
		statusRsp.keyWrapAlgo = keyWrapAlgo;
		statusRsp.keyStatus = status;
		statusRsp.hmacAlgo = hmacType;
		statusRsp.challengeData = challenge;
		statusRsp.hmacValue = hmac;

		return writer.WriteFreeFormat(statusRsp);		
	}

	bool KeyChangeState::EqualsLastStatusResponse(const openpal::ReadBufferView& unwrappedKeyStatus)
	{
		Group120Var5 copy(statusRsp);
		copy.hmacValue = ReadBufferView::Empty(); // exclude the HMAC from the comparison

		const uint32_t MAX_SIZE = Group120Var5::MIN_SIZE + AuthSizes::MAX_CHALLENGE_DATA_SIZE;
		openpal::StaticBuffer<MAX_SIZE> buffer;
		
		auto dest = buffer.GetWriteBuffer();
		if (!copy.Write(dest))
		{
			SIMPLE_LOG_BLOCK(logger, flags::ERR, "Unable to write last response to buffer");
			return false;
		}
		
		// this is what we sent
		auto sent = buffer.ToReadOnly(copy.Size());
		// the unwrapped data may be larger due to padding so truncate it to the length of what we're expecting before comparing
		auto unwrappedTrunc = unwrappedKeyStatus.Take(sent.Size());
					
		if (!SecureEquals(sent, unwrappedTrunc))
		{
			SIMPLE_LOG_BLOCK(logger, flags::ERR, "Unwrapped key data did not equal last response");
			return false;
		}

		return true;
	}

	bool KeyChangeState::CheckUserAndKSQMatches(const User& user, uint32_t keyChangeSeq)
	{
		if (lastUser.GetId() != user.GetId())
		{
			FORMAT_LOG_BLOCK(logger, flags::WARN, "No prior key change status for user %u", user.GetId());
			return false;
		}

		if (keyChangeSeqNum != keyChangeSeq)
		{
			FORMAT_LOG_BLOCK(logger, flags::WARN, "KSQ of %u doesn't match expected KSQ of %u", keyChangeSeq, keyChangeSeqNum);
			return false;
		}
			
		return true;
	}

}
