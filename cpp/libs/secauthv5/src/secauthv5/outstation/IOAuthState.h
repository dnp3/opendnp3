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
#ifndef SECAUTHV5_IOAUTHSTATE_H
#define SECAUTHV5_IOAUTHSTATE_H

#include <opendnp3/objects/Group120.h>
#include <opendnp3/objects/Group120Var1.h>
#include <opendnp3/objects/Group120Var2.h>
#include <opendnp3/objects/Group120Var6.h>

#include <opendnp3/outstation/OutstationState.h>

#include "SecurityState.h"

namespace secauthv5
{

// An abstract Outstation Authentication state
class IOAuthState
{
	public:

		virtual IOAuthState* OnRegularRequest(SecurityState& sstate, opendnp3::OState& ostate, const opendnp3::APDUHeader& header, const openpal::ReadBufferView& objects) = 0;
		virtual IOAuthState* OnAuthChallenge(SecurityState& sstate, opendnp3::OState& ostate, const opendnp3::APDUHeader& header, const opendnp3::Group120Var1& challenge) = 0;
		virtual IOAuthState* OnAuthReply(SecurityState& sstate, opendnp3::OState& ostate, const opendnp3::APDUHeader& header, const opendnp3::Group120Var2& reply) = 0;
		virtual IOAuthState* OnRequestKeyStatus(SecurityState& sstate, opendnp3::OState& ostate, const opendnp3::APDUHeader& header, const opendnp3::Group120Var4& status) = 0;

		virtual const char* GetName() = 0;

	protected:

		IOAuthState* IgnoreRegularRequest(SecurityState& sstate, opendnp3::OState& ostate, const opendnp3::APDUHeader& header, const openpal::ReadBufferView& objects);
		IOAuthState* IgnoreAuthChallenge(SecurityState& sstate, opendnp3::OState& ostate, const opendnp3::APDUHeader& header, const opendnp3::Group120Var1& challenge);
		IOAuthState* IgnoreAuthReply(SecurityState& sstate, opendnp3::OState& ostate, const opendnp3::APDUHeader& header, const opendnp3::Group120Var2& reply);
		IOAuthState* IgnoreRequestKeyStatus(SecurityState& sstate, opendnp3::OState& ostate, const opendnp3::APDUHeader& header, const opendnp3::Group120Var4& status);
};

}

#endif

