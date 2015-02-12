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

#include "SAv5OutstationAuthProvider.h"

#include <openpal/logging/LogMacros.h>
#include <openpal/logging/Logger.h>

#include "opendnp3/LogLevels.h"

#include "opendnp3/outstation/OutstationState.h"
#include "opendnp3/outstation/authv5/AuthRequestAdapter.h"

#include "AuthRequestParser.h"

using namespace openpal;

namespace opendnp3
{

SAv5OutstationAuthProvider::SAv5OutstationAuthProvider(uint32_t maxRxASDUSize, openpal::ICryptoProvider& crypto) :
	sstate(maxRxASDUSize, crypto)
{

}

void SAv5OutstationAuthProvider::Reset()
{
	sstate.Reset();
}

void SAv5OutstationAuthProvider::CheckState(OState& ostate)
{
	if (sstate.deferred.IsSet() && !ostate.isTransmitting)
	{
		auto handler = [&ostate, this](const APDUHeader& header, const ReadBufferView& objects)
		{
			this->Process(ostate, header, objects);
			return true;
		};

		sstate.deferred.Process(handler);
	}	
}
		
void SAv5OutstationAuthProvider::OnReceive(OState& ostate, const APDUHeader& header, const openpal::ReadBufferView& objects)
{	
	if (ostate.isTransmitting)
	{
		sstate.deferred.Set(header, objects);
	}
	else
	{
		this->Process(ostate, header, objects);
	}	
}

void SAv5OutstationAuthProvider::Process(OState& ostate, const APDUHeader& header, const openpal::ReadBufferView& objects)
{
	// examine the function code to determine what kind of ASDU it is
	switch (header.function)
	{
	case(FunctionCode::AUTH_REQUEST) :
		this->OnAuthRequest(ostate, header, objects);
		break;
	case(FunctionCode::AUTH_RESPONSE) :
		SIMPLE_LOG_BLOCK(ostate.logger, flags::WARN, "Auth response not valid for outstation");
		break;
	case(FunctionCode::AUTH_REQUEST_NO_ACK) :
		SIMPLE_LOG_BLOCK(ostate.logger, flags::WARN, "AuthRequestNoAck not supported");
		break;
	default:
		this->OnRegularRequest(ostate, header, objects);
		break;
	}
}

void SAv5OutstationAuthProvider::OnAuthRequest(OState& ostate, const APDUHeader& header, const openpal::ReadBufferView& objects)
{
	AuthRequestAdapter adapter(header, ostate, *this);
	AuthRequestParser::Parse(objects, adapter, &ostate.logger);
}

void SAv5OutstationAuthProvider::OnRegularRequest(OState& ostate, const APDUHeader& header, const openpal::ReadBufferView& objects)
{
	SIMPLE_LOG_BLOCK(ostate.logger, flags::WARN, "Regular requests not supported yet");
}

void SAv5OutstationAuthProvider::OnAuthChallenge(OState& ostate, const APDUHeader& header, const Group120Var1& challenge)
{
	SIMPLE_LOG_BLOCK(ostate.logger, flags::WARN, "Auth challenge not supported yet");
}

void SAv5OutstationAuthProvider::OnAuthReply(OState& ostate, const APDUHeader& header, const Group120Var2& reply)
{
	SIMPLE_LOG_BLOCK(ostate.logger, flags::WARN, "Auth reply not supported yet");
}

void SAv5OutstationAuthProvider::OnRequestKeyStatus(OState& ostate, const APDUHeader& header, const Group120Var4& status)
{
	SIMPLE_LOG_BLOCK(ostate.logger, flags::WARN, "Key status not supported yet");
}

void SAv5OutstationAuthProvider::OnChangeSessionKeys(OState& ostate, const APDUHeader& header, const Group120Var6& change)
{
	SIMPLE_LOG_BLOCK(ostate.logger, flags::WARN, "Session key change not supported yet");
}

}


