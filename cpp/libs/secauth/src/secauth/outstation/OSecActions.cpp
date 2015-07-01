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

#include "OSecActions.h"
#include "KeyUnwrap.h"

#include "secauth/Crypto.h"

#include <opendnp3/LogLevels.h>
#include <opendnp3/objects/Group120.h>

#include <openpal/logging/LogMacros.h>

using namespace openpal;
using namespace opendnp3;

namespace secauth
{
	void OSecActions::ProcessChangeSessionKeys(SecurityState& sstate, opendnp3::OContext& ocontext, const openpal::ReadBufferView& fragment, const opendnp3::APDUHeader& header, const opendnp3::Group120Var6& change)
	{
		User user(change.userNum);

		if (!sstate.keyChangeState.CheckUserAndKSQMatches(user, change.keyChangeSeqNum))
		{			
			OSecActions::RespondWithAuthError(header, sstate, ocontext, change.keyChangeSeqNum, user, AuthErrorCode::UNKNOWN_USER);
			return;
		}

		UpdateKeyMode updateKeyType;
		ReadBufferView updateKey;
		
		if (!sstate.pUserDatabase->GetUpdateKey(user, updateKeyType, updateKey))
		{
			FORMAT_LOG_BLOCK(ocontext.logger, flags::WARN, "Ignoring session key change request for unknown user %u", change.userNum);
			OSecActions::RespondWithAuthError(header, sstate, ocontext, change.keyChangeSeqNum, user, AuthErrorCode::UNKNOWN_USER);
			return;
		}
				
		UnwrappedKeyData unwrapped;
		KeyUnwrapBuffer buffer;

		auto unwrapSuccess = buffer.Unwrap(
			GetKeyWrapAlgo(*sstate.pCrypto, updateKeyType),
			updateKey,
			change.keyWrapData,
			unwrapped,
			&ocontext.logger
		);

		if (!unwrapSuccess)
		{
			SIMPLE_LOG_BLOCK(ocontext.logger, flags::WARN, "Failed to unwrap key data");
			OSecActions::RespondWithAuthError(header, sstate, ocontext, change.keyChangeSeqNum, user, AuthErrorCode::AUTHENTICATION_FAILED);
			return;
		}
	
		if (!sstate.keyChangeState.EqualsLastStatusResponse(unwrapped.keyStatusObject))
		{			
			OSecActions::RespondWithAuthError(header, sstate, ocontext, change.keyChangeSeqNum, user, AuthErrorCode::AUTHENTICATION_FAILED);
			return;
		}		

		// At this point, we've successfully authenticated the session key change for this user
		// We compute the HMAC based on the full ASDU and the monitoring direction session key		

		std::error_code ec;
		auto hmac = sstate.hmac.Compute(unwrapped.keys.monitorKey,  { fragment }, ec);

		if (ec)
		{
			SIMPLE_LOG_BLOCK(ocontext.logger, flags::ERR, ec.message().c_str());
			return;
		}
		
		/*
		SIMPLE_LOG_BLOCK(ocontext.logger, flags::INFO, "control key: ");
		FORMAT_HEX_BLOCK(ocontext.logger, flags::INFO, unwrapped.keys.controlKey, 17, 17);		
		SIMPLE_LOG_BLOCK(ocontext.logger, flags::INFO, "monitor key: ");
		FORMAT_HEX_BLOCK(ocontext.logger, flags::INFO, unwrapped.keys.monitorKey, 17, 17);
		SIMPLE_LOG_BLOCK(ocontext.logger, flags::INFO, "");
		SIMPLE_LOG_BLOCK(ocontext.logger, flags::INFO, "Authenication, plain text");
		FORMAT_HEX_BLOCK(ocontext.logger, flags::INFO, fragment, 17, 17);
		FORMAT_LOG_BLOCK(ocontext.logger, flags::INFO, "Authenication, hased text %u", hmac.Size());
		FORMAT_HEX_BLOCK(ocontext.logger, flags::INFO, hmac, 17, 17);		
		*/

		sstate.sessions.SetSessionKeys(user, unwrapped.keys);
		
		auto rsp = sstate.StartResponse(ocontext);
		rsp.SetFunction(FunctionCode::AUTH_RESPONSE);
		rsp.SetControl(header.control);
		auto writer = rsp.GetWriter();
		auto hmacType = sstate.hmac.GetType();

		auto success = sstate.keyChangeState.FormatKeyStatusResponse(
			writer,
			user,
			hmacType,
			Crypto::ToKeyWrapAlgorithm(updateKeyType),
			KeyStatus::OK,
			hmac
		);

		if (success)
		{
			ocontext.BeginTx(rsp.ToReadOnly());
		}
	}
	
	void OSecActions::ProcessRequestKeyStatus(SecurityState& sstate, opendnp3::OContext& ocontext, const opendnp3::APDUHeader& header, const opendnp3::Group120Var4& status)
	{
		User user(status.userNum);
		UpdateKeyMode type;
		if (!sstate.pUserDatabase->GetUpdateKeyType(user, type))
		{
			// TODO  - the spec appears to just say "ignore users that don't exist". Confirm this.
			FORMAT_LOG_BLOCK(ocontext.logger, flags::WARN, "User %u does not exist", user.GetId());
			return;
		}
		
		auto keyStatus = sstate.sessions.GetSessionKeyStatus(user);
			
		auto rsp = sstate.StartResponse(ocontext);
		rsp.SetFunction(FunctionCode::AUTH_RESPONSE);
		rsp.SetControl(header.control);
		auto writer = rsp.GetWriter();
		auto hmacType = HMACType::NO_MAC_VALUE;

		auto success = sstate.keyChangeState.FormatKeyStatusResponse(
			writer,
			user,
			hmacType,
			Crypto::ToKeyWrapAlgorithm(type),
			keyStatus,
			ReadBufferView::Empty()
			);

		if (!success)
		{
			SIMPLE_LOG_BLOCK(ocontext.logger, flags::WARN, "Unable to format key status response");
			return;
		}
				
		ocontext.BeginTx(rsp.ToReadOnly());		
	}	

	void OSecActions::ProcessAuthReply(SecurityState& sstate, opendnp3::OContext& ocontext, const opendnp3::APDUHeader& header, const opendnp3::Group120Var2& reply)
	{
		// first look-up the session for the specified user
		User user(reply.userNum);
		SessionKeysView view;
		if (sstate.sessions.GetSessionKeys(user, view) != KeyStatus::OK)
		{
			OSecActions::RespondWithAuthError(header, sstate, ocontext, reply.challengeSeqNum, user, AuthErrorCode::AUTHENTICATION_FAILED); // TODO - check this code
			return;
		}

		if (!sstate.challenge.VerifyAuthenticity(view.controlKey, sstate.hmac, reply.hmacValue, ocontext.logger))
		{
			// TODO  - log an auth failure
			OSecActions::RespondWithAuthError(header, sstate, ocontext, reply.challengeSeqNum, user, AuthErrorCode::AUTHENTICATION_FAILED);
			return;
		}
			
		auto criticalHeader = sstate.challenge.GetCriticalHeader();

		if (sstate.pUserDatabase->IsAuthorized(user, criticalHeader.function))
		{
			auto objects = sstate.challenge.GetCriticalASDU().Skip(APDU_REQUEST_HEADER_SIZE);
			ocontext.ProcessHeaderAndObjects(criticalHeader, objects);			
		}
		else
		{
			FORMAT_LOG_BLOCK(ocontext.logger, flags::WARN, "Verified user %u is not authorized for function %s", user.GetId(), FunctionCodeToString(criticalHeader.function));
			OSecActions::RespondWithAuthError(header, sstate, ocontext, reply.challengeSeqNum, user, AuthErrorCode::AUTHORIZATION_FAILED);			
		}				
	}

	bool OSecActions::TransmitChallenge(SecurityState& sstate, opendnp3::OContext& ocontext, const openpal::ReadBufferView& fragment, const opendnp3::APDUHeader& header)
	{
		auto response = sstate.StartResponse(ocontext);
		auto success = sstate.challenge.WriteChallenge(fragment, header, response, sstate.hmac.GetType(), *sstate.pCrypto, &ocontext.logger);
		if (success)
		{
			ocontext.BeginTx(response.ToReadOnly());
		}
		return success;
	}		

	openpal::IKeyWrapAlgo& OSecActions::GetKeyWrapAlgo(openpal::ICryptoProvider& crypto, UpdateKeyMode type)
	{
		switch (type)
		{
			case(UpdateKeyMode::AES128) :
				return crypto.GetAES128KeyWrap();
			default:
				return crypto.GetAES256KeyWrap();
		}
	}

	void OSecActions::RespondWithAuthError(
		const opendnp3::APDUHeader& header,
		SecurityState& sstate,
		opendnp3::OContext& ocontext,
		uint32_t seqNum,
		const User& user,
		AuthErrorCode code
		)
	{
		auto rsp = sstate.StartResponse(ocontext);
		rsp.SetFunction(FunctionCode::AUTH_RESPONSE);
		rsp.SetControl(header.control);
		auto writer = rsp.GetWriter();
		
		Group120Var7 error;
		error.challengeSeqNum = seqNum;
		error.userNum = user.GetId();
		error.assocId = sstate.settings.assocId;
		error.errorCode = code;
		error.time = openpal::UInt48Type(sstate.pTimeSource->Now().msSinceEpoch);

		writer.WriteFreeFormat(error);
		
		ocontext.BeginTx(rsp.ToReadOnly());
	}


}
