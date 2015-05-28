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

#include "SecurityState.h"

#include "OAuthStates.h"
#include <opendnp3/outstation/OutstationActions.h>

using namespace opendnp3;

namespace secauth
{
	SecurityState::SecurityState(
			const OutstationAuthSettings& settings_, 
			openpal::Logger logger, 
			openpal::IExecutor& executor, 
			openpal::IUTCTimeSource& timeSource, 
			IOutstationUserDatabase& userdb,
			openpal::ICryptoProvider& crypto) :

		settings(settings_),
		challenge(settings.challengeSize, settings.maxRxASDUSize),
		challengeTimer(executor),
		hmac(crypto, settings_.hmacMode),
		deferred(settings_.maxRxASDUSize),
		pExecutor(&executor),
		pTimeSource(&timeSource),
		pUserDatabase(&userdb),
		pCrypto(&crypto),
		keyStatus(KeyStatus::NOT_INIT),
		pState(OAuthStateIdle::Instance()),
		keyChangeState(1, 4, logger, crypto),
		sessions(executor),
		txBuffer(settings_.maxTxASDUSize)
	{
				
	}

	void SecurityState::Reset()
	{		
		keyStatus = KeyStatus::NOT_INIT;
		pState = OAuthStateIdle::Instance();
	}

	APDUResponse SecurityState::StartResponse(OState& ostate)
	{
		auto response = txBuffer.Start();
		response.SetIIN(OActions::GetResponseIIN(ostate));
		return response;
	}
}


