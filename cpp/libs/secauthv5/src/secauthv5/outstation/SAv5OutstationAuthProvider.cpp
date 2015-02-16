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

#include "opendnp3/app/parsing/APDUParser.h"
#include "opendnp3/outstation/OutstationState.h"
#include "opendnp3/outstation/OutstationActions.h"

#include "AuthRequestHandler.h"
#include "IOAuthState.h"


using namespace openpal;
using namespace opendnp3;

namespace secauthv5
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
	if (ostate.CanTransmit() && sstate.deferred.IsSet())
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
	if (ostate.CanTransmit())
	{
		this->Process(ostate, header, objects);		
	}
	else
	{
		sstate.deferred.Set(header, objects);
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
	AuthRequestHandler handler(header, ostate, *this);
	APDUParser::ParseSome(objects, handler, AuthRequestHandler::WhiteList, &ostate.logger);
}

void SAv5OutstationAuthProvider::OnRegularRequest(OState& ostate, const APDUHeader& header, const openpal::ReadBufferView& objects)
{	
	sstate.pState = sstate.pState->OnRegularRequest(sstate, ostate, header, objects);
}

void SAv5OutstationAuthProvider::OnAuthChallenge(OState& ostate, const APDUHeader& header, const Group120Var1& challenge)
{	
	sstate.pState = sstate.pState->OnAuthChallenge(sstate, ostate, header, challenge);
}

void SAv5OutstationAuthProvider::OnAuthReply(OState& ostate, const APDUHeader& header, const Group120Var2& reply)
{	
	sstate.pState = sstate.pState->OnAuthReply(sstate, ostate, header, reply);
}

void SAv5OutstationAuthProvider::OnRequestKeyStatus(OState& ostate, const APDUHeader& header, const Group120Var4& status)
{	
	sstate.pState = sstate.pState->OnRequestKeyStatus(sstate, ostate, header, status);	
}

/*
uint8_t data[4];
sstate.pCrypto->GetSecureRandom(WriteBufferView(data, 4));

Group120Var5 gv;
gv.keyChangeSeqNum = 1;
gv.userNum = status.userNum;
gv.keywrapAlgorithm = KeyWrapAlgorithm::AES_128;
gv.keyStatus = KeyStatus::NOT_INIT;
gv.hmacType = HMACType::HMAC_SHA1_TRUNC_10;
gv.challengeData = ReadBufferView(data, 4);

auto rsp = ostate.sol.tx.Start();
rsp.SetControl(header.control);
rsp.SetFunction(FunctionCode::AUTH_RESPONSE);
rsp.GetWriter().WriteFreeFormat(gv);
OActions::BeginResponseTx(ostate, rsp.ToReadOnly());
*/

void SAv5OutstationAuthProvider::OnChangeSessionKeys(OState& ostate, const APDUHeader& header, const Group120Var6& change)
{
	SIMPLE_LOG_BLOCK(ostate.logger, flags::WARN, "Session key change not supported yet");
}

}


