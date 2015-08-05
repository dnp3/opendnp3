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
	keyChangeSeqNum(0),
	M_CHALLENGE_SIZE(AuthSizes::GetBoundedChallengeSize(challengeSize)),
	m_logger(logger),
	m_crypto(&provider)
{}

void UpdateKeyChangeState::Reset()
{
	m_valid = false;
}

bool UpdateKeyChangeState::RespondToUpdateKeyChangeRequest(
	HeaderWriter& writer,
	KeyChangeMethod method,
	const std::string& username,
	const RSlice& masterChallengeData,
	const IFreeUser& freeUser
	)
{
	m_valid = false;

	// first thing to do is see if we can establish a user id assignment	
	if (!freeUser.FindFreeUserId(m_assignedUser))
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
		m_masterChallenge = masterChallengeData.CopyTo(dest);
	}

	// next, produce the challenge data for our side
	std::error_code ec;
	{
		auto dest = m_outstationChallengeBuffer.GetWSlice();
		this->m_outstationChallenge = m_crypto->GetSecureRandom(dest, ec);
	}
	
	if (ec)
	{
		FORMAT_LOG_BLOCK(m_logger, flags::WARN, "Error producing update key change challenge data: %s", ec.message().c_str());
		return false;
	}	

	// TODO investigate when the KSQ increments for these. Same KSQ as session keys? WTF?
	
	auto success = writer.WriteFreeFormat(Group120Var12(keyChangeSeqNum, m_assignedUser.GetId(), m_outstationChallenge));
	this->m_valid = success;
	return success;
}

}



