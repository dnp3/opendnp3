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

#include "OAuthStates.h"

#include <opendnp3/LogLevels.h>

#include <openpal/logging/LogMacros.h>

using namespace opendnp3;

namespace secauthv5
{	
	// -------- Idle ----------

	OAuthStateIdle OAuthStateIdle::instance;

	IOAuthState* OAuthStateIdle::OnRegularRequest(SecurityState& sstate, opendnp3::OState& ostate, const opendnp3::APDUHeader& header, const openpal::ReadBufferView& objects)
	{
		return this->IgnoreRegularRequest(sstate, ostate, header, objects);
	}

	IOAuthState* OAuthStateIdle::OnAuthChallenge(SecurityState& sstate, opendnp3::OState& ostate, const opendnp3::APDUHeader& header, const opendnp3::Group120Var1& challenge)
	{		
		return this->IgnoreAuthChallenge(sstate, ostate, header, challenge);
	}
	
	IOAuthState* OAuthStateIdle::OnAuthReply(SecurityState& sstate, opendnp3::OState& ostate, const opendnp3::APDUHeader& header, const opendnp3::Group120Var2& reply)
	{
		return this->IgnoreAuthReply(sstate, ostate, header, reply);
	}
	
	IOAuthState* OAuthStateIdle::OnRequestKeyStatus(SecurityState& sstate, opendnp3::OState& ostate, const opendnp3::APDUHeader& header, const opendnp3::Group120Var4& status)
	{
		return this->IgnoreRequestKeyStatus(sstate, ostate, header, status);
	}

	// -------- WaitForReply ----------
}

