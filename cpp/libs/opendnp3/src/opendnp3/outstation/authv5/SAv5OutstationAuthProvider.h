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

#include "opendnp3/outstation/authv5/SecurityState.h"
#include "opendnp3/outstation/authv5/IAuthRequestHandler.h"

#include <openpal/util/Uncopyable.h>

namespace opendnp3
{

/**
	SAv5 authentication provider
*/
class SAv5OutstationAuthProvider : private openpal::Uncopyable, public IOutstationAuthProvider, private IAuthRequestHandler
{
	public:

	SAv5OutstationAuthProvider(uint32_t maxRxASDUSize, openpal::ICryptoProvider& crypto);

	virtual void Reset() override final;	
		
	virtual void OnReceive(OState& ostate, const APDUHeader& header, const openpal::ReadBufferView& objects) override final;

	private:

	void OnAuthRequest(OState& ostate, const APDUHeader& header, const openpal::ReadBufferView& objects);

	void OnRegularRequest(OState& ostate, const APDUHeader& header, const openpal::ReadBufferView& objects);

	//// --- IAuthRequestHandler ----

	virtual void OnAuthChallenge(const APDUHeader& header, OState& ostate, const Group120Var1& challenge) override final;

	virtual void OnAuthReply(const APDUHeader& header, OState& ostate, const Group120Var2& reply) override final;

	virtual void OnRequestKeyStatus(const APDUHeader& header, OState& ostate, const Group120Var4& status) override final;

	virtual void OnChangeSessionKeys(const APDUHeader& header, OState& ostate, const Group120Var6& change) override final;

	SecurityState sstate;
};

}

#endif

