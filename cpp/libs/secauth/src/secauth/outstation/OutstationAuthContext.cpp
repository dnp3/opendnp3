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

#include "OutstationAuthContext.h"

#include <openpal/logging/LogMacros.h>
#include <openpal/logging/Logger.h>

#include "opendnp3/LogLevels.h"

#include "opendnp3/app/parsing/APDUParser.h"
#include "opendnp3/outstation/OutstationContext.h"

#include "secauth/AggressiveModeParser.h"

#include "AuthRequestHandler.h"
#include "IOAuthState.h"


using namespace openpal;
using namespace opendnp3;

namespace secauth
{

OAuthContext::OAuthContext(
			const OutstationConfig& config,
			const DatabaseTemplate& dbTemplate,
			openpal::Logger logger,
			openpal::IExecutor& executor,
			ILowerLayer& lower,
			ICommandHandler& commandHandler,
			IOutstationApplication& application,
			const OutstationAuthSettings& settings,
			openpal::IUTCTimeSource& timeSource,
			IOutstationUserDatabase& userDatabase,
			openpal::ICryptoProvider& crypto
		) :
		OContext(config, dbTemplate, logger, executor, lower, commandHandler, application),
		sstate(settings, logger, executor, timeSource, userDatabase, crypto)
{
	
}

bool OAuthContext::GoOffline()
{
	if (this->isOnline)
	{
		sstate.Reset();
	}
	return OContext::GoOffline();
}

void OAuthContext::CheckForTaskStart()
{
	if (this->CanTransmit() && sstate.deferred.IsSet())
	{
		auto handler = [this](const openpal::ReadBufferView& fragment, const APDUHeader& header, const ReadBufferView& objects)
		{
			this->ProcessAuthAPDU(fragment, header, objects);
			return true;
		};

		sstate.deferred.Process(handler);
	}


}
		
void OAuthContext::ReceiveAPDU(const openpal::ReadBufferView& fragment, const APDUHeader& header, const openpal::ReadBufferView& objects)
{	
	if (this->CanTransmit())
	{
		this->ProcessAuthAPDU(fragment, header, objects);
	}
	else
	{
		sstate.deferred.SetASDU(header, fragment);
	}
}

void OAuthContext::ProcessAuthAPDU(const openpal::ReadBufferView& fragment, const APDUHeader& header, const openpal::ReadBufferView& objects)
{
	switch (header.function)
	{
	case(FunctionCode::AUTH_REQUEST) :
		this->OnAuthRequest(fragment, header, objects);
		break;
	case(FunctionCode::AUTH_RESPONSE) :
		SIMPLE_LOG_BLOCK(this->logger, flags::WARN, "AuthResponse not valid for outstation");
		break;
	case(FunctionCode::AUTH_REQUEST_NO_ACK) :
		SIMPLE_LOG_BLOCK(this->logger, flags::WARN, "AuthRequestNoAck not supported");
		break;
	default:
		this->OnUnknownRequest(fragment, header, objects);
		break;
	}
}

void OAuthContext::OnAuthRequest(const openpal::ReadBufferView& fragment, const APDUHeader& header, const openpal::ReadBufferView& objects)
{
	if (header.control.UNS)
	{
		SIMPLE_LOG_BLOCK(this->logger, flags::WARN, "Ignoring AuthRequest with UNS bit set");
	}
	else
	{
		AuthRequestHandler handler(fragment, header, *this, this->logger);
		APDUParser::Parse(objects, handler, &this->logger);
	}
}

void OAuthContext::OnUnknownRequest(const openpal::ReadBufferView& fragment, const APDUHeader& header, const openpal::ReadBufferView& objects)
{	
	/// We have to determine if this is a regular request or an aggressive mode request
	AggModeResult result = AggressiveModeParser::IsAggressiveMode(objects, &this->logger);
	if (result.result == ParseResult::OK)
	{
		if (result.isAggMode)
		{
			// it's an aggressive mode request
			sstate.pState = sstate.pState->OnAggModeRequest(sstate, *this, header, objects, result.request);
		}
		else
		{
			// it's a normal DNP3 request
			sstate.pState = sstate.pState->OnRegularRequest(sstate, *this, fragment, header, objects);
		}
	}	
}

void OAuthContext::OnAuthChallenge(const openpal::ReadBufferView& fragment, const APDUHeader& header, const Group120Var1& challenge)
{	
	sstate.pState = sstate.pState->OnAuthChallenge(sstate, *this, header, challenge);
}

void OAuthContext::OnAuthReply(const openpal::ReadBufferView& fragment, const APDUHeader& header, const Group120Var2& reply)
{	
	sstate.pState = sstate.pState->OnAuthReply(sstate, *this, header, reply);
}

void OAuthContext::OnRequestKeyStatus(const openpal::ReadBufferView& fragment, const APDUHeader& header, const Group120Var4& status)
{	
	sstate.pState = sstate.pState->OnRequestKeyStatus(sstate, *this, header, status);
}

void OAuthContext::OnChangeSessionKeys(const openpal::ReadBufferView& fragment, const APDUHeader& header, const Group120Var6& change)
{
	sstate.pState = sstate.pState->OnChangeSessionKeys(sstate, *this, fragment, header, change);
}

}


