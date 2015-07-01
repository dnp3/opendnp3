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
#include "OSecActions.h"

#include <opendnp3/LogLevels.h>

#include <openpal/logging/LogMacros.h>

#include "opendnp3/outstation/OutstationActions.h"


using namespace openpal;
using namespace opendnp3;

namespace secauth
{	
	// -------- Idle ----------

	OAuthStateIdle OAuthStateIdle::instance;

	IOAuthState* OAuthStateIdle::OnRegularRequest(SecurityState& sstate, opendnp3::OContext& ocontext, const openpal::ReadBufferView& fragment, const opendnp3::APDUHeader& header, const openpal::ReadBufferView& objects)
	{
		if (sstate.settings.functions.IsCritical(header.function))
		{
			if (OSecActions::TransmitChallenge(sstate, ocontext, fragment, header))
			{
				SecurityState* psstate = &sstate;
				OContext* pocontext = &ocontext;

				auto timeout = [psstate, pocontext]() 
				{
					psstate->pState = psstate->pState->OnChallengeTimeout(*psstate, *pocontext);
				};

				sstate.challengeTimer.Restart(sstate.settings.challengeTimeout, timeout);

				return OAuthStateWaitForReply::Instance();
			}
			else
			{
				return this;
			}			
		}
		else
		{
			// no authentication required, just process it
			ocontext.ProcessHeaderAndObjects(header, objects);			
			return this;
		}		
	}

	IOAuthState* OAuthStateIdle::OnAggModeRequest(SecurityState& sstate, opendnp3::OContext& ocontext, const opendnp3::APDUHeader& header, const openpal::ReadBufferView& objects, const opendnp3::Group120Var3& aggModeRequest)
	{
		// TODO
		return this->IgnoreAggModeRequest(ocontext.logger);
	}

	IOAuthState* OAuthStateIdle::OnAuthChallenge(SecurityState& sstate, opendnp3::OContext& ocontext, const opendnp3::APDUHeader& header, const opendnp3::Group120Var1& challenge)
	{	
		// TODO
		return this->IgnoreAuthChallenge(ocontext.logger);
	}
	
	IOAuthState* OAuthStateIdle::OnAuthReply(SecurityState& sstate, opendnp3::OContext& ocontext, const opendnp3::APDUHeader& header, const opendnp3::Group120Var2& reply)
	{
		// TODO
		return this->IgnoreAuthReply(ocontext.logger);
	}
	
	IOAuthState* OAuthStateIdle::OnRequestKeyStatus(SecurityState& sstate, opendnp3::OContext& ocontext, const opendnp3::APDUHeader& header, const opendnp3::Group120Var4& status)
	{
		OSecActions::ProcessRequestKeyStatus(sstate, ocontext, header, status);
		return this;
	}

	IOAuthState* OAuthStateIdle::OnChangeSessionKeys(SecurityState& sstate, opendnp3::OContext& ocontext, const openpal::ReadBufferView& fragment, const opendnp3::APDUHeader& header, const opendnp3::Group120Var6& change)
	{
		OSecActions::ProcessChangeSessionKeys(sstate, ocontext, fragment, header, change);		
		return this;
	}

	IOAuthState* OAuthStateIdle::OnChallengeTimeout(SecurityState& sstate, opendnp3::OContext& ocontext)
	{
		return this->IgnoreChallengeTimeout(ocontext.logger);
	}

	// -------- WaitForReply ----------

	OAuthStateWaitForReply OAuthStateWaitForReply::instance;

	IOAuthState* OAuthStateWaitForReply::OnRegularRequest(SecurityState& sstate, opendnp3::OContext& ocontext, const openpal::ReadBufferView& fragment, const opendnp3::APDUHeader& header, const openpal::ReadBufferView& objects)
	{
		return this->IgnoreRegularRequest(ocontext.logger);
	}

	IOAuthState* OAuthStateWaitForReply::OnAggModeRequest(SecurityState& sstate, opendnp3::OContext& ocontext, const opendnp3::APDUHeader& header, const openpal::ReadBufferView& objects, const opendnp3::Group120Var3& aggModeRequest)
	{
		return this->IgnoreAggModeRequest(ocontext.logger);
	}

	IOAuthState* OAuthStateWaitForReply::OnAuthChallenge(SecurityState& sstate, opendnp3::OContext& ocontext, const opendnp3::APDUHeader& header, const opendnp3::Group120Var1& challenge)
	{
		return this->IgnoreAuthChallenge(ocontext.logger);
	}

	IOAuthState* OAuthStateWaitForReply::OnAuthReply(SecurityState& sstate, opendnp3::OContext& ocontext, const opendnp3::APDUHeader& header, const opendnp3::Group120Var2& reply)
	{
		sstate.challengeTimer.Cancel();
		OSecActions::ProcessAuthReply(sstate, ocontext, header, reply);		
		return OAuthStateIdle::Instance(); // no matter what, we return to idle
	}

	IOAuthState* OAuthStateWaitForReply::OnRequestKeyStatus(SecurityState& sstate, opendnp3::OContext& ocontext, const opendnp3::APDUHeader& header, const opendnp3::Group120Var4& status)
	{
		return this->IgnoreRequestKeyStatus(ocontext.logger, status.userNum);
	}

	IOAuthState* OAuthStateWaitForReply::OnChangeSessionKeys(SecurityState& sstate, opendnp3::OContext& ocontext, const openpal::ReadBufferView& fragment, const opendnp3::APDUHeader& header, const opendnp3::Group120Var6& change)
	{
		return this->IgnoreChangeSessionKeys(ocontext.logger, change.userNum);
	}

	IOAuthState* OAuthStateWaitForReply::OnChallengeTimeout(SecurityState& sstate, opendnp3::OContext& ocontext)
	{
		SIMPLE_LOG_BLOCK(ocontext.logger, flags::WARN, "Timeout while waiting for challenge response");
		return OAuthStateIdle::Instance();
	}
}

