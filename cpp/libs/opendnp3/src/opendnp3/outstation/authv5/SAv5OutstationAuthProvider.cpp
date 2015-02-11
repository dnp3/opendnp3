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
#include "opendnp3/outstation/authv5/DefaultAuthRequestHandler.h"

#include "AuthRequestParser.h"

namespace opendnp3
{

SAv5OutstationAuthProvider::SAv5OutstationAuthProvider(uint32_t maxRxASDUSize, openpal::ICryptoProvider& crypto) :
	deferred(maxRxASDUSize),
	pCrypto(&crypto),
	keyStatus(KeyStatus::NOT_INIT)
{

}

void SAv5OutstationAuthProvider::Reset()
{
	keyStatus = KeyStatus::NOT_INIT;
}
		
void SAv5OutstationAuthProvider::OnReceive(OState& ostate, const APDUHeader& header, const openpal::ReadBufferView& objects)
{	
	FORMAT_HEX_BLOCK(ostate.logger, flags::APP_OBJECT_RX, objects, 16, 16);

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
			this->OnUnknownRequest(ostate, header, objects);
			break;
	}	

}

void SAv5OutstationAuthProvider::OnAuthRequest(OState& ostate, const APDUHeader& header, const openpal::ReadBufferView& objects)
{
	DefaultAuthRequestHandler handler;	
	AuthRequestParser::Parse(header, objects, handler, &ostate.logger);
}

void SAv5OutstationAuthProvider::OnUnknownRequest(OState& ostate, const APDUHeader& header, const openpal::ReadBufferView& objects)
{
	SIMPLE_LOG_BLOCK(ostate.logger, flags::WARN, "UnknownRequest not supported yet");
}

}


