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

#include "IOAuthState.h"

#include <opendnp3/LogLevels.h>

#include <openpal/logging/LogMacros.h>

using namespace opendnp3;

namespace secauthv5
{

	IOAuthState* IOAuthState::IgnoreRegularRequest(SecurityState& sstate, opendnp3::OState& ostate, const opendnp3::APDUHeader& header, const openpal::ReadBufferView& objects)
	{
		FORMAT_LOG_BLOCK(ostate.logger, flags::WARN, "AuthState: %s - Ignoring regular request", this->GetName());
		return this;
	}

	IOAuthState* IOAuthState::IgnoreAuthChallenge(SecurityState& sstate, opendnp3::OState& ostate, const opendnp3::APDUHeader& header, const opendnp3::Group120Var1& challenge)
	{
		FORMAT_LOG_BLOCK(ostate.logger, flags::WARN, "AuthState: %s - Ignoring Auth challenge", this->GetName());
		return this;
	}

	IOAuthState* IOAuthState::IgnoreAuthReply(SecurityState& sstate, opendnp3::OState& ostate, const opendnp3::APDUHeader& header, const opendnp3::Group120Var2& reply)
	{
		FORMAT_LOG_BLOCK(ostate.logger, flags::WARN, "AuthState: %s - Ignoring Auth reply", this->GetName());
		return this;
	}

	IOAuthState* IOAuthState::IgnoreRequestKeyStatus(SecurityState& sstate, opendnp3::OState& ostate, const opendnp3::APDUHeader& header, const opendnp3::Group120Var4& status)
	{
		FORMAT_LOG_BLOCK(ostate.logger, flags::WARN, "AuthState: %s - Ignoring key status request for user: %u", this->GetName(), status.userNum);
		return this;
	}

}

