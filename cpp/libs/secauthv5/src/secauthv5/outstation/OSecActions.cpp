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

#include "OSecActions.h"
#include "KeyUnwrap.h"

#include <opendnp3/LogLevels.h>
#include <opendnp3/outstation/OutstationActions.h>

#include <openpal/logging/LogMacros.h>

using namespace openpal;
using namespace opendnp3;

namespace secauthv5
{
	void OSecActions::ProcessChangeSessionKeys(SecurityState& sstate, opendnp3::OState& ostate, const opendnp3::APDUHeader& header, const opendnp3::Group120Var6& change)
	{
		User user(change.user);
		UpdateKeyType updateKeyType;
		ReadBufferView updateKey;
		
		if (!sstate.pUserDatabase->GetUpdateKey(user, updateKeyType, updateKey))
		{
			FORMAT_LOG_BLOCK(ostate.logger, flags::WARN, "Ignoring session key change request for unknown user %u", change.user);
			return;
		}
		
		if (sstate.keyChangeState.GetLastKeyChangeUser().GetId() != user.GetId())
		{
			FORMAT_LOG_BLOCK(ostate.logger, flags::WARN, "No prior key change status for user %u", change.user);
			return;
		}


		UnwrappedKeyData unwrapped;
		KeyUnwrapBuffer buffer;

		auto unwrapSuccess = buffer.Unwrap(
			GetKeyWrapAlgo(*sstate.pCrypto, updateKeyType),
			updateKey,
			change.data,
			unwrapped,
			&ostate.logger
		);

		if (!unwrapSuccess)
		{
			SIMPLE_LOG_BLOCK(ostate.logger, flags::WARN, "Failed to unwrap key data");
			return;
		}
	
		if (!sstate.keyChangeState.EqualsLastStatusResponse(unwrapped.keyStatusObject))
		{
			SIMPLE_LOG_BLOCK(ostate.logger, flags::WARN, "Key change authentication failed");
			return;
		}		

		SIMPLE_LOG_BLOCK(ostate.logger, flags::EVENT, "Key change authentication success");
	}
	
	void OSecActions::ProcessRequestKeyStatus(SecurityState& sstate, opendnp3::OState& ostate, const opendnp3::APDUHeader& header, const opendnp3::Group120Var4& status)
	{
		User user(status.userNum);
		UpdateKeyType type;
		if (sstate.pUserDatabase->GetUpdateKeyType(user, type))
		{
			ReadBufferView lastKeyChangeHMAC;
			auto keyStatus = sstate.sessions.GetKeyStatus(user, lastKeyChangeHMAC);
			
			auto rsp = sstate.txBuffer.Start();
			rsp.SetFunction(FunctionCode::AUTH_RESPONSE);
			rsp.SetControl(header.control);
			auto writer = rsp.GetWriter();
			auto hmacType = (keyStatus == KeyStatus::NOT_INIT) ? HMACType::NO_MAC_VALUE : HMACType::HMAC_SHA1_TRUNC_10; // TODO from configuration

			auto success = sstate.keyChangeState.FormatKeyStatusResponse(
				writer,
				user,
				hmacType,
				ToKeyWrapAlgorithm(type),
				keyStatus,
				lastKeyChangeHMAC
			);

			if (success)
			{ 
				OActions::BeginResponseTx(ostate, rsp.ToReadOnly());
			}
		}
		else
		{
			// TODO  - the spec appears to just say "ignore users that don't exist". Confirm this.
			FORMAT_LOG_BLOCK(ostate.logger, flags::WARN, "User %u does not exist", user.GetId());
		}		
	}

	openpal::IKeyWrapAlgo& OSecActions::GetKeyWrapAlgo(openpal::ICryptoProvider& crypto, UpdateKeyType type)
	{
		switch (type)
		{
			case(UpdateKeyType::AES128) :
				return crypto.GetAES128KeyWrap();
			default:
				return crypto.GetAES256KeyWrap();
		}
	}
}
