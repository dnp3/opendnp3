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
#ifndef SECAUTH_OAUTHSTATES_H
#define SECAUTH_OAUTHSTATES_H

#include "IOAuthState.h"

namespace secauth
{
	class OAuthStateIdle : public IOAuthState, private openpal::Uncopyable
	{
	public:

		static IOAuthState* Instance() { return &instance; }

		virtual IOAuthState* OnRegularRequest(SecurityState& sstate, opendnp3::OContext& ocontext, const openpal::ReadBufferView& fragment, const opendnp3::APDUHeader& header, const openpal::ReadBufferView& objects) override final;
		virtual IOAuthState* OnAggModeRequest(SecurityState& sstate, opendnp3::OContext& ocontext, const opendnp3::APDUHeader& header, const openpal::ReadBufferView& objects, const opendnp3::Group120Var3& aggModeRequest) override final;
		virtual IOAuthState* OnAuthChallenge(SecurityState& sstate, opendnp3::OContext& ocontext, const opendnp3::APDUHeader& header, const opendnp3::Group120Var1& challenge) override final;
		virtual IOAuthState* OnAuthReply(SecurityState& sstate, opendnp3::OContext& ocontext, const opendnp3::APDUHeader& header, const opendnp3::Group120Var2& reply) override final;
		virtual IOAuthState* OnRequestKeyStatus(SecurityState& sstate, opendnp3::OContext& ocontext, const opendnp3::APDUHeader& header, const opendnp3::Group120Var4& status) override final;
		virtual IOAuthState* OnChangeSessionKeys(SecurityState& sstate, opendnp3::OContext& ocontext, const openpal::ReadBufferView& fragment, const opendnp3::APDUHeader& header, const opendnp3::Group120Var6& change) override final;
		virtual IOAuthState* OnChallengeTimeout(SecurityState& sstate, opendnp3::OContext& ocontext) override final;

	private:

		virtual const char* GetName() override final { return "Idle"; };

		static OAuthStateIdle instance;

		OAuthStateIdle() {}
	};

	class OAuthStateWaitForReply : public IOAuthState, private openpal::Uncopyable
	{
	public:

		static IOAuthState* Instance() { return &instance; }

		virtual IOAuthState* OnRegularRequest(SecurityState& sstate, opendnp3::OContext& ocontext, const openpal::ReadBufferView& fragment, const opendnp3::APDUHeader& header, const openpal::ReadBufferView& objects) override final;
		virtual IOAuthState* OnAggModeRequest(SecurityState& sstate, opendnp3::OContext& ocontext, const opendnp3::APDUHeader& header, const openpal::ReadBufferView& objects, const opendnp3::Group120Var3& aggModeRequest) override final;
		virtual IOAuthState* OnAuthChallenge(SecurityState& sstate, opendnp3::OContext& ocontext, const opendnp3::APDUHeader& header, const opendnp3::Group120Var1& challenge) override final;
		virtual IOAuthState* OnAuthReply(SecurityState& sstate, opendnp3::OContext& ocontext, const opendnp3::APDUHeader& header, const opendnp3::Group120Var2& reply) override final;
		virtual IOAuthState* OnRequestKeyStatus(SecurityState& sstate, opendnp3::OContext& ocontext, const opendnp3::APDUHeader& header, const opendnp3::Group120Var4& status) override final;
		virtual IOAuthState* OnChangeSessionKeys(SecurityState& sstate, opendnp3::OContext& ocontext, const openpal::ReadBufferView& fragment, const opendnp3::APDUHeader& header, const opendnp3::Group120Var6& change) override final;
		virtual IOAuthState* OnChallengeTimeout(SecurityState& sstate, opendnp3::OContext& ocontext) override final;

	private:

		virtual const char* GetName() override final { return "WaitForReply"; };

		static OAuthStateWaitForReply instance;

		OAuthStateWaitForReply() {}
	};
}

#endif
