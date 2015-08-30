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

#include "UpdateKeyChangeState.h"

#include <openpal/logging/LogMacros.h>
#include <opendnp3/LogLevels.h>

#include "secauth/AuthSizes.h"

using namespace openpal;
using namespace opendnp3;

namespace secauth
{

UpdateKeyChangeState::UpdateKeyChangeState(uint16_t challengeSize, Logger logger, ICryptoProvider& provider) :
	m_valid(false),
	M_CHALLENGE_SIZE(AuthSizes::GetBoundedChallengeSize(challengeSize)),
	m_logger(logger),
	m_crypto(&provider)
{}

void UpdateKeyChangeState::Reset()
{
	m_valid = false;
}

bool UpdateKeyChangeState::WriteUpdateKeyChangeResposne(
    HeaderWriter& writer,
    uint32_t ksq,
    const std::string& username,
    const RSlice& masterChallengeData,
    const IFreeUser& freeUser
)
{
	m_valid = false;

	this->m_data.username = username;
	this->m_data.keyChangeSeqNum = ksq;

	// first thing to do is see if we can establish a user id assignment
	if (!freeUser.FindFreeUserId(m_data.user))
	{
		return false;
	}

	// validate and set the master challenge data

	if (!AuthSizes::ChallengeDataSizeWithinLimits(masterChallengeData.Size()))
	{
		FORMAT_LOG_BLOCK(m_logger, flags::WARN, "Out of bounds master challenge size: %u", masterChallengeData.Size());
		return false;
	}

	{
		auto dest = m_masterChallengeBuffer.GetWSlice();
		this->m_data.masterChallenge = masterChallengeData.CopyTo(dest);
	}

	// next, produce the challenge data for our side
	std::error_code ec;
	{
		auto dest = m_outstationChallengeBuffer.GetWSlice(this->M_CHALLENGE_SIZE);
		this->m_data.outstationChallenge = m_crypto->GetSecureRandom(dest, ec);
	}

	if (ec)
	{
		FORMAT_LOG_BLOCK(m_logger, flags::WARN, "Error producing update key change challenge data: %s", ec.message().c_str());
		return false;
	}

	// TODO investigate when the KSQ increments for these. Same KSQ as session keys? WTF?

	auto success = writer.WriteFreeFormat(Group120Var12(m_data.keyChangeSeqNum, m_data.user.GetId(), m_data.outstationChallenge));
	this->m_valid = success;
	return success;
}

bool UpdateKeyChangeState::VerifyUserAndKSQ(uint32_t ksq, User user, VerificationData& data)
{
	if (!m_valid)
	{
		SIMPLE_LOG_BLOCK(m_logger, flags::WARN, "No prior begin update key change request");
		return false;
	}

	m_valid = false; // regardless of whether this succeeds or fails we invalidate the state

	if (ksq != m_data.keyChangeSeqNum)
	{
		FORMAT_LOG_BLOCK(m_logger, flags::WARN, "Received KSQ %u does not match expected KSQ %u", ksq, m_data.keyChangeSeqNum);
		return false;
	}

	if (user.GetId() != m_data.user.GetId())
	{
		FORMAT_LOG_BLOCK(m_logger, flags::WARN, "User %u does not match expected user %u", user.GetId(), m_data.user.GetId());
		return false;
	}

	data = this->m_data;

	return true;
}

}



