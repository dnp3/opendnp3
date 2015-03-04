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
#ifndef SECAUTHV5_SECURITYSTATE_H
#define SECAUTHV5_SECURITYSTATE_H

#include <opendnp3/gen/KeyStatus.h>
#include <opendnp3/outstation/TxBuffer.h>
#include <opendnp3/outstation/OutstationState.h>

#include <openpal/executor/IExecutor.h>
#include <openpal/crypto/ICryptoProvider.h>
#include <openpal/executor/IUTCTimeSource.h>
#include <openpal/executor/TimerRef.h>

#include "secauthv5/IUserDatabase.h"
#include "secauthv5/HMACProvider.h"
#include "secauthv5/DeferredASDU.h"

#include "KeyChangeState.h"
#include "OutstationAuthSettings.h"
#include "SessionStore.h"
#include "ChallengeState.h"


namespace secauthv5
{

class IOAuthState;

class SecurityState
{
	public:

	SecurityState(
		const OutstationAuthSettings& settings, 
		openpal::Logger logger, 
		openpal::IExecutor& executor,
		openpal::IUTCTimeSource& timeSource, 
		IUserDatabase& userdb, 
		openpal::ICryptoProvider& crypto
	);

	void Reset();
	
	OutstationAuthSettings settings;
	ChallengeState challenge;
	openpal::TimerRef challengeTimer;
	HMACProvider hmac;
	DeferredASDU deferred;
	openpal::IExecutor* pExecutor;
	openpal::IUTCTimeSource* pTimeSource;
	IUserDatabase* pUserDatabase;
	openpal::ICryptoProvider* pCrypto;
	opendnp3::KeyStatus keyStatus;
	IOAuthState* pState;
	KeyChangeState keyChangeState;
	SessionStore sessions;

	opendnp3::APDUResponse StartResponse(opendnp3::OState& ostate);
	
private:
	opendnp3::TxBuffer txBuffer;

};

}

#endif

