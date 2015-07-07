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
#ifndef SECAUTH_SECURITYSTATE_H
#define SECAUTH_SECURITYSTATE_H

#include <opendnp3/gen/KeyStatus.h>
#include <opendnp3/app/TxBuffer.h>
#include <opendnp3/outstation/OutstationContext.h>

#include <openpal/executor/IExecutor.h>
#include <openpal/crypto/ICryptoProvider.h>
#include <openpal/executor/IUTCTimeSource.h>
#include <openpal/executor/TimerRef.h>

#include "secauth/HMACProvider.h"
#include "secauth/DeferredASDU.h"
#include "secauth/SessionStore.h"

#include "IOutstationUserDatabase.h"
#include "KeyChangeState.h"
#include "OutstationAuthSettings.h"
#include "ChallengeState.h"

namespace secauth
{

class IOAuthState;

class SecurityState
{
	public:

	SecurityState(
		const opendnp3::OutstationParams& params,
		const OutstationAuthSettings& settings, 
		openpal::Logger logger, 
		openpal::IExecutor& executor,
		openpal::IUTCTimeSource& timeSource, 
		IOutstationUserDatabase& userdb, 
		openpal::ICryptoProvider& crypto
	);	
	
	OutstationAuthSettings settings;
	ChallengeState challenge;
	openpal::TimerRef challengeTimer;
	HMACProvider hmac;
	DeferredASDU deferred;	
	openpal::IUTCTimeSource* pTimeSource;
	IOutstationUserDatabase* pUserDatabase;
	openpal::ICryptoProvider* pCrypto;	
	IOAuthState* pState;
	KeyChangeState keyChangeState;
	SessionStore sessions;
	opendnp3::TxBuffer txBuffer;

};

}

#endif

