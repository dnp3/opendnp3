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

		if (user.IsDefault())
		{
			
			UpdateKeyType keyType;
			openpal::ReadBufferView updateKey;
			if (!sstate.pUserDatabase->GetUpdateKey(user, keyType, updateKey))
			{
				return;
			}
			
			UnwrappedKeyData unwrapped;
			KeyUnwrapBuffer buffer;
			auto success = buffer.Unwrap(
				sstate.pCrypto->GetAES128KeyWrap(),
				updateKey,
				change.data,
				unwrapped,
				&ostate.logger
				);

			if (success)
			{
				SIMPLE_LOG_BLOCK(ostate.logger, flags::EVENT, "Unwrapped key data!");
				FORMAT_HEX_BLOCK(ostate.logger, flags::EVENT, unwrapped.controlSessionKey, 18, 18);
				FORMAT_HEX_BLOCK(ostate.logger, flags::EVENT, unwrapped.monitorSessionKey, 18, 18);
				FORMAT_HEX_BLOCK(ostate.logger, flags::EVENT, unwrapped.keyStatusObject, 18, 18);
			}
			else
			{
				SIMPLE_LOG_BLOCK(ostate.logger, flags::EVENT, "Failed to unwrap key data!");
			}			
		}
		else
		{
			FORMAT_LOG_BLOCK(ostate.logger, flags::WARN, "Ignoring session key change request for user %u", change.user);
		}
	}
	
	void OSecActions::ProcessRequestKeyStatus(SecurityState& sstate, opendnp3::OState& ostate, const opendnp3::APDUHeader& header, const opendnp3::Group120Var4& status)
	{
		User user(status.userNum);
		UpdateKeyType type;
		if (sstate.pUserDatabase->GetUpdateKeyType(user, type))
		{
			auto keyChangeAlgo = ToKeyWrapAlgorithm(type);

			auto rsp = sstate.txBuffer.Start();
			auto success = sstate.keyChangeState.FormatKeyStatusResponse(rsp, header.control, KeyStatus::NOT_INIT);
			if (success)
			{
				OActions::BeginResponseTx(ostate, rsp.ToReadOnly());
			}
		}
		else
		{
			FORMAT_LOG_BLOCK(ostate.logger, flags::WARN, "User %u does not exist", user.GetId());
		}		
	}
}
