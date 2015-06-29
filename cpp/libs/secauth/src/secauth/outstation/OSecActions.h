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
#ifndef SECAUTH_OSECACTIONS_H
#define SECAUTH_OSECACTIONS_H

#include <openpal/util/Uncopyable.h>

#include <opendnp3/outstation/OutstationContext.h>

#include <opendnp3/objects/Group120.h>

#include <opendnp3/gen/AuthErrorCode.h>

#include "SecurityState.h"

namespace secauth
{
	class OSecActions : private openpal::StaticOnly
	{
		public:
								
			static void ProcessRequestKeyStatus(SecurityState& sstate, opendnp3::OContext& ocontext, const opendnp3::APDUHeader& header, const opendnp3::Group120Var4& status);

			static void ProcessChangeSessionKeys(SecurityState& sstate, opendnp3::OContext& ocontext, const openpal::ReadBufferView& fragment, const opendnp3::APDUHeader& header, const opendnp3::Group120Var6& change);

			static void ProcessAuthReply(SecurityState& sstate, opendnp3::OContext& ocontext, const opendnp3::APDUHeader& header, const opendnp3::Group120Var2& reply);
			
			static bool TransmitChallenge(SecurityState& sstate, opendnp3::OContext& ocontext, const openpal::ReadBufferView& fragment, const opendnp3::APDUHeader& header);			
			

		private:
		
			static openpal::IKeyWrapAlgo& GetKeyWrapAlgo(openpal::ICryptoProvider& crypto, opendnp3::UpdateKeyMode type);

			static void RespondWithAuthError(
				const opendnp3::APDUHeader& header,
				SecurityState& sstate,
				opendnp3::OContext& ocontext,
				uint32_t seqNum,
				const User& user,
				opendnp3::AuthErrorCode code
			);
	};

	
}

#endif
