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

#include "MasterAuthProvider.h"

#include <opendnp3/LogLevels.h>
#include <opendnp3/master/MasterState.h>

#include <openpal/logging/LogMacros.h>

#include "secauth/master/AuthResponseHandler.h"
#include "opendnp3/app/parsing/APDUParser.h"

using namespace openpal;
using namespace opendnp3;

namespace secauth
{

MasterAuthProvider::MasterAuthProvider(
	opendnp3::IMasterApplication& application,
	openpal::IExecutor& executor,
	openpal::Logger logger,
	openpal::ICryptoProvider& crypto,
	IMasterUserDatabase& userDB
	) : 
	msstate(application, executor, crypto, userDB),
	sessionKeyTask(application, TimeDuration::Seconds(5), logger, User::Default(), msstate)
{

}

void MasterAuthProvider::GoOnline()
{
	// add the session key task to the scheduler
	pMState->scheduler.Schedule(openpal::ManagedPtr<IMasterTask>::WrapperOnly(&sessionKeyTask));
}

void MasterAuthProvider::GoOffline()
{
	// TODO reset the sessions?
}

void MasterAuthProvider::OnReceive(const opendnp3::APDUResponseHeader& header, const openpal::ReadBufferView& objects)
{	
	switch (header.function)
	{
	case(FunctionCode::AUTH_RESPONSE) :
		this->OnReceiveAuthResponse(header, objects);
		break;
	case(FunctionCode::RESPONSE) :
		pMState->ProcessResponse(header, objects);
		break;
	case(FunctionCode::UNSOLICITED_RESPONSE) :
		pMState->ProcessUnsolicitedResponse(header, objects);
		break;
	default:
		FORMAT_LOG_BLOCK(pMState->logger, opendnp3::flags::WARN, "Ignoring unsupported function code: %s", FunctionCodeToString(header.function));
		break;
	}	
}

void MasterAuthProvider::OnReceiveAuthResponse(const opendnp3::APDUResponseHeader& header, const openpal::ReadBufferView& objects)
{
	// need to determine the context of the auth response
	
	if (pMState->pState->ExpectingResponse())
	{
		// an auth-based task is running and needs to receive this directly
		if (pMState->pActiveTask->AcceptsFunction(FunctionCode::AUTH_RESPONSE))
		{
			pMState->ProcessResponse(header, objects);
		}
		else
		{
			AuthResponseHandler handler(header, *this);
			APDUParser::Parse(objects, handler, pMState->logger);
		}		
	}
	else
	{
		SIMPLE_LOG_BLOCK(pMState->logger, flags::WARN, "Ignoring AuthResponse"); // TODO - better error message?
	}

}

void  MasterAuthProvider::OnAuthChallenge(const opendnp3::APDUHeader& header, const opendnp3::Group120Var1& challenge)
{
	// TODO - handle an authenticaiton challenge
	SIMPLE_LOG_BLOCK(pMState->logger, flags::WARN, "Ignoring authentication challenge");
}

void  MasterAuthProvider::OnAuthError(const opendnp3::APDUHeader& header, const opendnp3::Group120Var7& error)
{
	FORMAT_LOG_BLOCK(pMState->logger, flags::WARN,
		"Received auth error from outstation w/ code: %s",
		AuthErrorCodeToString(error.errorCode)
	);		
}

}

