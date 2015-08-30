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
#ifndef SECAUTH_MASTER_SECURITY_H
#define SECAUTH_MASTER_SECURITY_H

#include <openpal/util/Uncopyable.h>

#include "secauth/master/SessionKeyTask.h"
#include "secauth/master/IMasterApplicationSA.h"
#include "secauth/master/MasterUserDatabase.h"
#include "secauth/master/MasterAuthSettings.h"

namespace secauth
{

class MasterSecurity : private openpal::Uncopyable
{

public:

	MasterSecurity(
	    openpal::IExecutor& executor,
	    IMasterApplicationSA& application,
	    const MasterAuthSettings& authSettings,
	    openpal::ICryptoProvider& crypto
	);

	typedef std::map<uint16_t, std::unique_ptr<SessionKeyTask>> SessionKeyTaskMap;

	MasterAuthSettings			settings;
	IMasterApplicationSA*		pApplicationSA;
	openpal::ICryptoProvider*	pCrypto;
	MasterUserDatabase			userDB;
	SessionStore				sessions;
	openpal::RSlice		lastRequest;
	SessionKeyTaskMap			sessionKeyTaskMap;

	openpal::StaticBuffer<AuthSizes::MAX_MASTER_CHALLENGE_REPLY_FRAG_SIZE> challengeReplyBuffer;
};


}

#endif
