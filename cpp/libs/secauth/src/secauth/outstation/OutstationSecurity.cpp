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

#include "OutstationSecurity.h"

using namespace opendnp3;

namespace secauth
{
	OutstationSecurity::OutstationSecurity(
			const OutstationParams& params,
			const OutstationAuthSettings& settings_, 
			openpal::Logger logger, 
			openpal::IExecutor& executor, 			
			IOutstationApplicationSA& application,
			openpal::ICryptoProvider& crypto) :

		state(SecurityState::IDLE),
		settings(settings_),
		challenge(settings.challengeSize, params.maxRxFragSize),
		challengeTimer(executor),
		hmac(crypto, settings.hmacMode),
		deferred(params.maxRxFragSize),				
		pApplication(&application),
		pCrypto(&crypto),				
		keyChangeState(1, 4, logger, crypto),
		sessions(executor, settings.sessionKeyTimeout, settings.maxAuthMsgCount),
		txBuffer(params.maxTxFragSize)
	{
				
	}
	
}


