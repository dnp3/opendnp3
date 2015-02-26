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

#include "OutstationAuthProvider.h"

#include <openpal/logging/LogMacros.h>
#include <openpal/logging/Logger.h>

#include "opendnp3/LogLevels.h"

#include "opendnp3/app/parsing/APDUParser.h"
#include "opendnp3/outstation/OutstationState.h"

#include "secauthv5/AggressiveModeParser.h"

#include "AuthRequestHandler.h"
#include "IOAuthState.h"


using namespace openpal;
using namespace opendnp3;

namespace secauthv5
{

OutstationAuthProvider::OutstationAuthProvider(
		const OutstationAuthSettings& settings,
		openpal::Logger logger,
		openpal::IExecutor& executor,
		openpal::IUTCTimeSource& timeSource,
		IUserDatabase& userDatabase,
		openpal::ICryptoProvider& crypto
		) :
		sstate(settings, logger, executor, timeSource, userDatabase, crypto)
{
	
}

void OutstationAuthProvider::Reset()
{
	sstate.Reset();
}

void OutstationAuthProvider::CheckState(OState& ostate)
{
	if (ostate.CanTransmit() && sstate.deferred.IsSet())
	{
		auto handler = [&ostate, this](const openpal::ReadBufferView& fragment, const APDUHeader& header, const ReadBufferView& objects)
		{
			this->Process(ostate, fragment, header, objects);
			return true;
		};

		sstate.deferred.Process(handler);
	}	
}
		
void OutstationAuthProvider::OnReceive(OState& ostate, const openpal::ReadBufferView& fragment, const APDUHeader& header, const openpal::ReadBufferView& objects)
{	
	if (ostate.CanTransmit())
	{
		this->Process(ostate, fragment, header, objects);		
	}
	else
	{
		sstate.deferred.SetASDU(header, fragment);
	}
}

void OutstationAuthProvider::Process(OState& ostate, const openpal::ReadBufferView& fragment, const APDUHeader& header, const openpal::ReadBufferView& objects)
{
	// examine the function code to determine what kind of ASDU it is
	switch (header.function)
	{
		case(FunctionCode::AUTH_REQUEST) :
			this->OnAuthRequest(ostate, fragment, header, objects);
			break;
		case(FunctionCode::AUTH_RESPONSE) :
			SIMPLE_LOG_BLOCK(ostate.logger, flags::WARN, "AuthResponse not valid for outstation");
			break;
		case(FunctionCode::AUTH_REQUEST_NO_ACK) :
			SIMPLE_LOG_BLOCK(ostate.logger, flags::WARN, "AuthRequestNoAck not supported");
			break;
		default:
			this->OnUnknownRequest(ostate, fragment, header, objects);
			break;
	}
}

void OutstationAuthProvider::OnAuthRequest(OState& ostate, const openpal::ReadBufferView& fragment, const APDUHeader& header, const openpal::ReadBufferView& objects)
{
	if (header.control.UNS)
	{
		SIMPLE_LOG_BLOCK(ostate.logger, flags::WARN, "Ignoring AuthRequest with UNS bit set");
	}
	else
	{
		AuthRequestHandler handler(fragment, header, ostate, *this);
		APDUParser::ParseSome(objects, handler, AuthRequestHandler::WhiteList, &ostate.logger);
	}
}

void OutstationAuthProvider::OnUnknownRequest(OState& ostate, const openpal::ReadBufferView& fragment, const APDUHeader& header, const openpal::ReadBufferView& objects)
{	
	/// We have to determine if this is a regular request or an aggressive mode request
	Group120Var3 aggModeRequest;
	auto result = AggressiveModeParser::IsAggressiveMode(objects, aggModeRequest, &ostate.logger);
	if (result.first == ParseResult::OK)
	{
		if (result.second)
		{
			// it's an aggressive mode request
			sstate.pState = sstate.pState->OnAggModeRequest(sstate, ostate, header, objects, aggModeRequest);
		}
		else
		{
			// it's a normal DNP3 request
			sstate.pState = sstate.pState->OnRegularRequest(sstate, ostate, header, objects);
		}
	}	
}

void OutstationAuthProvider::OnAuthChallenge(OState& ostate, const openpal::ReadBufferView& fragment, const APDUHeader& header, const Group120Var1& challenge)
{	
	sstate.pState = sstate.pState->OnAuthChallenge(sstate, ostate, header, challenge);
}

void OutstationAuthProvider::OnAuthReply(OState& ostate, const openpal::ReadBufferView& fragment, const APDUHeader& header, const Group120Var2& reply)
{	
	sstate.pState = sstate.pState->OnAuthReply(sstate, ostate, header, reply);
}

void OutstationAuthProvider::OnRequestKeyStatus(OState& ostate, const openpal::ReadBufferView& fragment, const APDUHeader& header, const Group120Var4& status)
{	
	sstate.pState = sstate.pState->OnRequestKeyStatus(sstate, ostate, header, status);	
}

void OutstationAuthProvider::OnChangeSessionKeys(OState& ostate, const openpal::ReadBufferView& fragment, const APDUHeader& header, const Group120Var6& change)
{
	sstate.pState = sstate.pState->OnChangeSessionKeys(sstate, ostate, fragment, header, change);	
}

}


