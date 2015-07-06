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


using namespace openpal;
using namespace opendnp3;

namespace secauth
{	
	// -------- Idle ----------

	OAuthStateIdle OAuthStateIdle::instance;

	IOAuthState* OAuthStateIdle::OnRegularRequest(OAuthContext& ocontext, const openpal::ReadBufferView& fragment, const opendnp3::APDUHeader& header, const openpal::ReadBufferView& objects)
	{
		if (ocontext.sstate.settings.functions.IsCritical(header.function))
		{
			if (ocontext.TransmitChallenge(fragment, header))
			{				
				OAuthContext* pocontext = &ocontext;

				auto timeout = [pocontext]() 
				{
					pocontext->sstate.pState = pocontext->sstate.pState->OnChallengeTimeout(*pocontext);
				};

				pocontext->sstate.challengeTimer.Restart(pocontext->sstate.settings.challengeTimeout, timeout);

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
			ocontext.ProcessAPDU(fragment, header, objects);			
			return this;
		}		
	}

	IOAuthState* OAuthStateIdle::OnAggModeRequest(OAuthContext& ocontext, const opendnp3::APDUHeader& header, const openpal::ReadBufferView& objects, const opendnp3::Group120Var3& aggModeRequest)
	{
		// TODO
		return this->IgnoreAggModeRequest(ocontext.logger);
	}

	IOAuthState* OAuthStateIdle::OnAuthChallenge(OAuthContext& ocontext, const opendnp3::APDUHeader& header, const opendnp3::Group120Var1& challenge)
	{	
		// TODO
		return this->IgnoreAuthChallenge(ocontext.logger);
	}
	
	IOAuthState* OAuthStateIdle::OnAuthReply(OAuthContext& ocontext, const opendnp3::APDUHeader& header, const opendnp3::Group120Var2& reply)
	{
		// TODO
		return this->IgnoreAuthReply(ocontext.logger);
	}
	
	IOAuthState* OAuthStateIdle::OnRequestKeyStatus(OAuthContext& ocontext, const opendnp3::APDUHeader& header, const opendnp3::Group120Var4& status)
	{
		ocontext.ProcessRequestKeyStatus(header, status);
		return this;
	}

	IOAuthState* OAuthStateIdle::OnChangeSessionKeys(OAuthContext& ocontext, const openpal::ReadBufferView& fragment, const opendnp3::APDUHeader& header, const opendnp3::Group120Var6& change)
	{
		ocontext.ProcessChangeSessionKeys(fragment, header, change);		
		return this;
	}

	IOAuthState* OAuthStateIdle::OnChallengeTimeout(OAuthContext& ocontext)
	{
		return this->IgnoreChallengeTimeout(ocontext.logger);
	}

	// -------- WaitForReply ----------

	OAuthStateWaitForReply OAuthStateWaitForReply::instance;

	IOAuthState* OAuthStateWaitForReply::OnRegularRequest(OAuthContext& ocontext, const openpal::ReadBufferView& fragment, const opendnp3::APDUHeader& header, const openpal::ReadBufferView& objects)
	{
		return this->IgnoreRegularRequest(ocontext.logger);
	}

	IOAuthState* OAuthStateWaitForReply::OnAggModeRequest(OAuthContext& ocontext, const opendnp3::APDUHeader& header, const openpal::ReadBufferView& objects, const opendnp3::Group120Var3& aggModeRequest)
	{
		return this->IgnoreAggModeRequest(ocontext.logger);
	}

	IOAuthState* OAuthStateWaitForReply::OnAuthChallenge(OAuthContext& ocontext, const opendnp3::APDUHeader& header, const opendnp3::Group120Var1& challenge)
	{
		return this->IgnoreAuthChallenge(ocontext.logger);
	}

	IOAuthState* OAuthStateWaitForReply::OnAuthReply(OAuthContext& ocontext, const opendnp3::APDUHeader& header, const opendnp3::Group120Var2& reply)
	{
		ocontext.sstate.challengeTimer.Cancel();
		ocontext.ProcessAuthReply(header, reply);
		return OAuthStateIdle::Instance(); // no matter what, we return to idle
	}

	IOAuthState* OAuthStateWaitForReply::OnRequestKeyStatus(OAuthContext& ocontext, const opendnp3::APDUHeader& header, const opendnp3::Group120Var4& status)
	{
		return this->IgnoreRequestKeyStatus(ocontext.logger, status.userNum);
	}

	IOAuthState* OAuthStateWaitForReply::OnChangeSessionKeys(OAuthContext& ocontext, const openpal::ReadBufferView& fragment, const opendnp3::APDUHeader& header, const opendnp3::Group120Var6& change)
	{
		return this->IgnoreChangeSessionKeys(ocontext.logger, change.userNum);
	}

	IOAuthState* OAuthStateWaitForReply::OnChallengeTimeout(OAuthContext& ocontext)
	{
		SIMPLE_LOG_BLOCK(ocontext.logger, flags::WARN, "Timeout while waiting for challenge response");
		return OAuthStateIdle::Instance();
	}
}

