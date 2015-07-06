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
#ifndef SECAUTH_OUTSTATIONAUTHCONTEXT_H
#define SECAUTH_OUTSTATIONAUTHCONTEXT_H

#include <opendnp3/outstation/OutstationContext.h>

#include "SecurityState.h"
#include "OutstationAuthSettings.h"
#include "IAuthRequestHandler.h"
#include "IOutstationUserDatabase.h"

#include <openpal/util/Uncopyable.h>
#include <openpal/executor/IUTCTimeSource.h>

namespace secauth
{

/**
	SAv5 outstation authentication provider
*/
class OAuthContext final : public opendnp3::OContext, private IAuthRequestHandler
{
	public:

	OAuthContext(
		const opendnp3::OutstationConfig& config,
		const opendnp3::DatabaseTemplate& dbTemplate,
		openpal::Logger logger,
		openpal::IExecutor& executor,
		opendnp3::ILowerLayer& lower,
		opendnp3::ICommandHandler& commandHandler,
		opendnp3::IOutstationApplication& application,
		const OutstationAuthSettings& settings,				
		openpal::IUTCTimeSource& timeSource,
		IOutstationUserDatabase& userDatabase,
		openpal::ICryptoProvider& crypto
	);

	virtual bool GoOffline() override final;

	virtual void CheckForTaskStart() override final;
		
	virtual void ReceiveAPDU(const openpal::ReadBufferView& apdu, const opendnp3::APDUHeader& header, const openpal::ReadBufferView& objects) override final;

	private:

	void ProcessAuthAPDU(const openpal::ReadBufferView& fragment, const opendnp3::APDUHeader& header, const openpal::ReadBufferView& objects);

	void OnAuthRequest(const openpal::ReadBufferView& fragment, const opendnp3::APDUHeader& header, const openpal::ReadBufferView& objects);

	void OnUnknownRequest(const openpal::ReadBufferView& fragment, const opendnp3::APDUHeader& header, const openpal::ReadBufferView& objects);

	//// --- IAuthRequestHandler ----

	virtual void OnAuthChallenge(const openpal::ReadBufferView& fragment, const opendnp3::APDUHeader& header, const opendnp3::Group120Var1& challenge) override final;

	virtual void OnAuthReply(const openpal::ReadBufferView& fragment, const opendnp3::APDUHeader& header, const opendnp3::Group120Var2& reply) override final;

	virtual void OnRequestKeyStatus(const openpal::ReadBufferView& fragment, const opendnp3::APDUHeader& header, const opendnp3::Group120Var4& status) override final;

	virtual void OnChangeSessionKeys(const openpal::ReadBufferView& fragment, const opendnp3::APDUHeader& header, const opendnp3::Group120Var6& change) override final;

	/// --- Helper methods ----
	
	static openpal::IKeyWrapAlgo& GetKeyWrapAlgo(openpal::ICryptoProvider& crypto, opendnp3::UpdateKeyMode type);

	void RespondWithAuthError(
		const opendnp3::APDUHeader& header,		
		uint32_t seqNum,
		const User& user,
		opendnp3::AuthErrorCode code
	);

	public:

	bool TransmitChallenge(const openpal::ReadBufferView& fragment, const opendnp3::APDUHeader& header);

	void ProcessRequestKeyStatus(const opendnp3::APDUHeader& header, const opendnp3::Group120Var4& status);

	void ProcessChangeSessionKeys(const openpal::ReadBufferView& fragment, const opendnp3::APDUHeader& header, const opendnp3::Group120Var6& change);

	void ProcessAuthReply(const opendnp3::APDUHeader& header, const opendnp3::Group120Var2& reply);

	/// --- State ---

	SecurityState sstate;
};

}

#endif

