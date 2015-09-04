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
#ifndef SECAUTH_UPDATE_KEY_CHANGE_STATE_H
#define SECAUTH_UPDATE_KEY_CHANGE_STATE_H

#include <openpal/container/StaticBuffer.h>
#include <openpal/crypto/ICryptoProvider.h>
#include <openpal/logging/Logger.h>

#include <opendnp3/app/User.h>
#include <opendnp3/app/HeaderWriter.h>

#include "secauth/AuthSizes.h"
#include "secauth/UpdateKey.h"
#include "secauth/outstation/IOutstationUserDatabase.h"

#include <functional>

namespace secauth
{

///  a function used to find a free user number
typedef std::function<bool(opendnp3::User& user)> GetFreeUserFun;

class UpdateKeyChangeState
{
public:

	class VerificationData
	{
	public:

		VerificationData() : keyChangeSeqNum(0)
		{}

		VerificationData(
		    std::string username_,
		    openpal::RSlice masterChallenge_,
		    openpal::RSlice outstationChallenge_,
		    uint32_t keyChangeSeqNum_,
		    opendnp3::User user_
		) :
			username(username_),
			masterChallenge(masterChallenge_),
			outstationChallenge(outstationChallenge_),
			keyChangeSeqNum(keyChangeSeqNum_),
			user(user_)
		{}

		std::string username;
		openpal::RSlice masterChallenge;
		openpal::RSlice outstationChallenge;
		uint32_t keyChangeSeqNum;
		opendnp3::User user;
	};

	UpdateKeyChangeState(uint16_t challengeSize, openpal::Logger logger, openpal::ICryptoProvider& provider);

	void Reset();

	bool WriteUpdateKeyChangeResposne(
	    opendnp3::HeaderWriter& writer,
	    uint32_t ksq,
	    const std::string& username,
	    const openpal::RSlice& masterChallengeData,
	    const IFreeUser& freeUser);

	bool VerifyUserAndKSQ(uint32_t ksq, opendnp3::User user, VerificationData& data);

private:

	bool m_valid;
	VerificationData m_data;

	const uint16_t M_CHALLENGE_SIZE;
	openpal::Logger m_logger;
	openpal::ICryptoProvider* m_crypto;

	// static buffers for both nonces
	openpal::StaticBuffer<AuthSizes::MAX_CHALLENGE_DATA_SIZE> m_masterChallengeBuffer;
	openpal::StaticBuffer<AuthSizes::MAX_CHALLENGE_DATA_SIZE> m_outstationChallengeBuffer;
};

}

#endif

