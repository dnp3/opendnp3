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
#ifndef SECAUTH_OUTSTATIONAUTHSETTINGS_H
#define SECAUTH_OUTSTATIONAUTHSETTINGS_H

#include "secauth/HMACMode.h"
#include "secauth/CriticalFunctions.h"
#include "secauth/StatThresholds.h"

#include <openpal/executor/TimeDuration.h>

namespace secauth
{

/**
	SAv5 outstation authentication provider
*/
struct OutstationAuthSettings
{	
	OutstationAuthSettings();	
	
	/// response timeout period for challenges
	openpal::TimeDuration challengeTimeout;	
	/// the number of bytes in a challenge
	uint16_t challengeSize;			
	/// the association Id reported in tranmissions
	uint16_t assocId;					
	/// The hmac mode to request when challenging
	HMACMode hmacMode;
	/// A datastructure that defines what function codes are deemed 'critical'
	CriticalFunctions functions;
	/// The maximum number of auth-ed messages before a session key change is required
	uint32_t maxAuthMsgCount;
	/// The maximum time allowed before a session key expires 
	openpal::TimeDuration sessionKeyTimeout;
	/// Thresholds for various security statistics
	StatThresholds statThresholds;

	
};

}

#endif

