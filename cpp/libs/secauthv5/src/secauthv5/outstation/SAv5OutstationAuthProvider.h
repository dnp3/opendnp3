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
#ifndef OPENDNP3_SAV5OUTSTATIONAUTHPROVIDER_H
#define OPENDNP3_SAV5OUTSTATIONAUTHPROVIDER_H

#include "opendnp3/outstation/IOutstationAuthProvider.h"

#include "SecurityState.h"
#include "IAuthRequestHandler.h"

#include <openpal/util/Uncopyable.h>

namespace secauthv5
{

/**
	SAv5 authentication provider
*/
class SAv5OutstationAuthProvider : private openpal::Uncopyable, public opendnp3::IOutstationAuthProvider, private IAuthRequestHandler
{
	public:

	SAv5OutstationAuthProvider(uint32_t maxRxASDUSize, openpal::ICryptoProvider& crypto);

	virtual void Reset() override final;	

	virtual void CheckState(opendnp3::OState& ostate) override final;
		
	virtual void OnReceive(opendnp3::OState& ostate, const opendnp3::APDUHeader& header, const openpal::ReadBufferView& objects) override final;

	private:

		void Process(opendnp3::OState& ostate, const opendnp3::APDUHeader& header, const openpal::ReadBufferView& objects);

		void OnAuthRequest(opendnp3::OState& ostate, const opendnp3::APDUHeader& header, const openpal::ReadBufferView& objects);

		void OnRegularRequest(opendnp3::OState& ostate, const opendnp3::APDUHeader& header, const openpal::ReadBufferView& objects);

	//// --- IAuthRequestHandler ----

	virtual void OnAuthChallenge(opendnp3::OState& ostate, const opendnp3::APDUHeader& header, const opendnp3::Group120Var1& challenge) override final;

	virtual void OnAuthReply(opendnp3::OState& ostate, const opendnp3::APDUHeader& header, const opendnp3::Group120Var2& reply) override final;

	virtual void OnRequestKeyStatus(opendnp3::OState& ostate, const opendnp3::APDUHeader& header, const opendnp3::Group120Var4& status) override final;

	virtual void OnChangeSessionKeys(opendnp3::OState& ostate, const opendnp3::APDUHeader& header, const opendnp3::Group120Var6& change) override final;

	/// --- State ---

	SecurityState sstate;
};

}

#endif

