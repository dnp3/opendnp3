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

#include "OutstationAuthContext.h"

#include <openpal/logging/LogMacros.h>
#include <openpal/logging/Logger.h>

#include "opendnp3/LogLevels.h"

#include "opendnp3/app/parsing/APDUParser.h"
#include "opendnp3/app/QualityFlags.h"
#include "opendnp3/outstation/OutstationContext.h"


#include "secauth/AggressiveModeParser.h"
#include "secauth/outstation/KeyUnwrap.h"
#include "secauth/outstation/OAuthStates.h"
#include "secauth/Crypto.h"

#include "AuthRequestHandler.h"
#include "IOAuthState.h"
#include "secauth/StatThresholds.h"


using namespace openpal;
using namespace opendnp3;

namespace secauth
{

DatabaseTemplate OAuthContext::EnableSecStats(const DatabaseTemplate& dbTemplate)
{
	DatabaseTemplate copy(dbTemplate);
	copy.numSecurityStats = AuthConstants::NUM_SECURITY_STATS;
	return copy;
}

void OAuthContext::ConfigureSecStats(const StatThresholds& thresholds)
{
	auto stats = this->database.buffers.buffers.GetArrayView<SecurityStat>();
	SecurityStat zero(opendnp3::flags::ONLINE, sstate.settings.assocId, 0, DNPTime(0));

	for (uint16_t i = 0; i < AuthConstants::NUM_SECURITY_STATS; ++i)
	{		
		stats[i].SetInitialValue(zero);
		stats[i].metadata.deadband = thresholds.GetDeadband(i);
	}
}

OAuthContext::OAuthContext(
			const OutstationConfig& config,
			const DatabaseTemplate& dbTemplate,
			openpal::Logger logger,
			openpal::IExecutor& executor,
			ILowerLayer& lower,
			ICommandHandler& commandHandler,
			IOutstationApplicationSA& application,
			const OutstationAuthSettings& settings,
			openpal::IUTCTimeSource& timeSource,			
			openpal::ICryptoProvider& crypto
		) :
		OContext(config, EnableSecStats(dbTemplate), logger, executor, lower, commandHandler, application),
		sstate(config.params, settings, logger, executor, timeSource, application, crypto)
{
	this->ConfigureSecStats(sstate.settings.statThresholds);

	//ask the application to load user info
	application.LoadUsers(sstate.userDB);
}

bool OAuthContext::GoOffline()
{
	auto ret = OContext::GoOffline();

	if (ret)
	{
		this->sstate.pState = OAuthStateIdle::Instance();
		this->sstate.sessions.Clear();
	}

	return ret;
}

void OAuthContext::CheckForTaskStart()
{
	if (this->CanTransmit() && sstate.deferred.IsSet())
	{
		auto handler = [this](const openpal::ReadBufferView& apdu, const APDUHeader& header, const ReadBufferView& objects)
		{
			this->ProcessAuthAPDU(apdu, header, objects);
			return true;
		};

		sstate.deferred.Process(handler);
	}

	OContext::CheckForTaskStart();
}
		
void OAuthContext::ReceiveParsedHeader(const openpal::ReadBufferView& apdu, const APDUHeader& header, const openpal::ReadBufferView& objects)
{	
	if (this->CanTransmit())
	{
		this->ProcessAuthAPDU(apdu, header, objects);
	}
	else
	{
		sstate.deferred.SetASDU(header, apdu);
	}
}

void OAuthContext::Increment(SecurityStatIndex index)
{
	auto count = this->sstate.stats.Increment(index);

	DNPTime time(this->sstate.pTimeSource->Now().msSinceEpoch);

	SecurityStat stat(opendnp3::flags::ONLINE, this->sstate.settings.assocId, count, time);

	this->database.Update(stat, static_cast<uint16_t>(index));
}

void OAuthContext::ProcessAuthAPDU(const openpal::ReadBufferView& apdu, const APDUHeader& header, const openpal::ReadBufferView& objects)
{
	switch (header.function)
	{
	case(FunctionCode::AUTH_REQUEST) :
		this->OnAuthRequest(apdu, header, objects);
		break;
	case(FunctionCode::AUTH_RESPONSE) :
		SIMPLE_LOG_BLOCK(this->logger, flags::WARN, "AuthResponse not valid for outstation");
		break;
	case(FunctionCode::AUTH_REQUEST_NO_ACK) :
		SIMPLE_LOG_BLOCK(this->logger, flags::WARN, "AuthRequestNoAck not supported");
		break;
	default:
		this->OnUnknownRequest(apdu, header, objects);
		break;
	}
}

void OAuthContext::OnAuthRequest(const openpal::ReadBufferView& apdu, const APDUHeader& header, const openpal::ReadBufferView& objects)
{
	if (header.control.UNS)
	{
		SIMPLE_LOG_BLOCK(this->logger, flags::WARN, "Ignoring AuthRequest with UNS bit set");
	}
	else
	{
		AuthRequestHandler handler(apdu, header, *this, this->logger);
		APDUParser::Parse(objects, handler, &this->logger);
	}
}

void OAuthContext::OnUnknownRequest(const openpal::ReadBufferView& apdu, const APDUHeader& header, const openpal::ReadBufferView& objects)
{	
	/// We have to determine if this is a regular request or an aggressive mode request
	AggModeResult result = AggressiveModeParser::IsAggressiveMode(objects, &this->logger);
	if (result.result == ParseResult::OK)
	{
		if (result.isAggMode)
		{
			// it's an aggressive mode request
			sstate.pState = sstate.pState->OnAggModeRequest(*this, header, objects, result.request);
		}
		else
		{
			// it's a normal DNP3 request
			sstate.pState = sstate.pState->OnRegularRequest(*this, apdu, header, objects);
		}
	}	
}

opendnp3::APDUResponse OAuthContext::StartAuthResponse()
{
	auto response = sstate.txBuffer.Start();
	response.SetIIN(this->GetResponseIIN());
	return response;
}

void OAuthContext::OnAuthChallenge(const openpal::ReadBufferView& apdu, const APDUHeader& header, const Group120Var1& challenge)
{	
	sstate.pState = sstate.pState->OnAuthChallenge(*this, header, challenge);
}

void OAuthContext::OnAuthReply(const openpal::ReadBufferView& apdu, const APDUHeader& header, const Group120Var2& reply)
{	
	sstate.pState = sstate.pState->OnAuthReply(*this, header, reply);
}

void OAuthContext::OnRequestKeyStatus(const openpal::ReadBufferView& apdu, const APDUHeader& header, const Group120Var4& status)
{	
	sstate.pState = sstate.pState->OnRequestKeyStatus(*this, header, status);
}

void OAuthContext::OnChangeSessionKeys(const openpal::ReadBufferView& apdu, const APDUHeader& header, const Group120Var6& change)
{
	sstate.pState = sstate.pState->OnChangeSessionKeys(*this, apdu, header, change);
}

void OAuthContext::ProcessChangeSessionKeys(const openpal::ReadBufferView& apdu, const opendnp3::APDUHeader& header, const opendnp3::Group120Var6& change)
{
	User user(change.userNum);

	if (!sstate.keyChangeState.CheckUserAndKSQMatches(user, change.keyChangeSeqNum))
	{
		this->RespondWithAuthError(header, change.keyChangeSeqNum, user, AuthErrorCode::UNKNOWN_USER);
		return;
	}

	UpdateKeyMode updateKeyType;
	ReadBufferView updateKey;

	if (!sstate.userDB.GetUpdateKey(user, updateKeyType, updateKey))
	{
		FORMAT_LOG_BLOCK(this->logger, flags::WARN, "Ignoring session key change request for unknown user %u", change.userNum);
		this->RespondWithAuthError(header, change.keyChangeSeqNum, user, AuthErrorCode::UNKNOWN_USER);
		return;
	}

	UnwrappedKeyData unwrapped;
	KeyUnwrapBuffer buffer;

	auto unwrapSuccess = buffer.Unwrap(
		GetKeyWrapAlgo(*sstate.pCrypto, updateKeyType),
		updateKey,
		change.keyWrapData,
		unwrapped,
		&this->logger
		);

	if (!unwrapSuccess)
	{
		SIMPLE_LOG_BLOCK(this->logger, flags::WARN, "Failed to unwrap key data");
		this->RespondWithAuthError(header, change.keyChangeSeqNum, user, AuthErrorCode::AUTHENTICATION_FAILED);
		return;
	}

	if (!sstate.keyChangeState.EqualsLastStatusResponse(unwrapped.keyStatusObject))
	{
		this->RespondWithAuthError(header, change.keyChangeSeqNum, user, AuthErrorCode::AUTHENTICATION_FAILED);
		return;
	}

	// At this point, we've successfully authenticated the session key change for this user
	// We compute the HMAC based on the full ASDU and the monitoring direction session key		

	std::error_code ec;
	auto hmac = sstate.hmac.Compute(unwrapped.keys.monitorKey, { apdu }, ec);

	if (ec)
	{
		SIMPLE_LOG_BLOCK(this->logger, flags::ERR, ec.message().c_str());
		return;
	}

	/*
	SIMPLE_LOG_BLOCK(ocontext.logger, flags::INFO, "control key: ");
	FORMAT_HEX_BLOCK(ocontext.logger, flags::INFO, unwrapped.keys.controlKey, 17, 17);
	SIMPLE_LOG_BLOCK(ocontext.logger, flags::INFO, "monitor key: ");
	FORMAT_HEX_BLOCK(ocontext.logger, flags::INFO, unwrapped.keys.monitorKey, 17, 17);
	SIMPLE_LOG_BLOCK(ocontext.logger, flags::INFO, "");
	SIMPLE_LOG_BLOCK(ocontext.logger, flags::INFO, "Authenication, plain text");
	FORMAT_HEX_BLOCK(ocontext.logger, flags::INFO, apdu, 17, 17);
	FORMAT_LOG_BLOCK(ocontext.logger, flags::INFO, "Authenication, hased text %u", hmac.Size());
	FORMAT_HEX_BLOCK(ocontext.logger, flags::INFO, hmac, 17, 17);
	*/

	sstate.sessions.SetSessionKeys(user, unwrapped.keys);

	auto rsp = this->StartAuthResponse();
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
		this->BeginTx(rsp.ToReadOnly());
	}
}

void OAuthContext::ProcessRequestKeyStatus(const opendnp3::APDUHeader& header, const opendnp3::Group120Var4& status)
{
	User user(status.userNum);
	UpdateKeyMode type;
	if (!sstate.userDB.GetUpdateKeyType(user, type))
	{
		// TODO  - the spec appears to just say "ignore users that don't exist". Confirm this.
		FORMAT_LOG_BLOCK(this->logger, flags::WARN, "User %u does not exist", user.GetId());
		return;
	}

	auto keyStatus = sstate.sessions.GetSessionKeyStatus(user);

	auto rsp = this->StartAuthResponse();
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
		SIMPLE_LOG_BLOCK(this->logger, flags::WARN, "Unable to format key status response");
		return;
	}

	this->BeginTx(rsp.ToReadOnly());
}

void OAuthContext::ProcessAuthReply(const opendnp3::APDUHeader& header, const opendnp3::Group120Var2& reply)
{
	// first look-up the session for the specified user
	User user(reply.userNum);
	SessionKeysView keys;

	if (sstate.sessions.TryGetSessionKeys(user, keys) != KeyStatus::OK)
	{
		++(this->sstate.otherStats.authFailuresDueToExpiredKeys);
		this->Increment(SecurityStatIndex::AUTHENTICATION_FAILURES);
		FORMAT_LOG_BLOCK(this->logger, flags::WARN, "No valid session keys for user %u", user.GetId());
		this->RespondWithAuthError(header, reply.challengeSeqNum, user, AuthErrorCode::AUTHENTICATION_FAILED);
		return;
	}

	if (!sstate.challenge.VerifyAuthenticity(keys.controlKey, sstate.hmac, reply.hmacValue, this->logger))
	{
		this->Increment(SecurityStatIndex::AUTHENTICATION_FAILURES);
		FORMAT_LOG_BLOCK(this->logger, flags::WARN, "Authentication failure for user %u", user.GetId());
		this->RespondWithAuthError(header, reply.challengeSeqNum, user, AuthErrorCode::AUTHENTICATION_FAILED);
		return;
	}

	auto criticalHeader = sstate.challenge.GetCriticalHeader();

	if (!sstate.userDB.IsAuthorized(user, criticalHeader.function))
	{
		this->Increment(SecurityStatIndex::AUTHORIZATION_FAILURES);
		FORMAT_LOG_BLOCK(this->logger, flags::WARN, "Verified user %u is not authorized for function %s", user.GetId(), FunctionCodeToString(criticalHeader.function));
		this->RespondWithAuthError(header, reply.challengeSeqNum, user, AuthErrorCode::AUTHORIZATION_FAILED);
		return;
	}

	// this increments both the security statistic and the count on the key
	this->IncrementSessionAuthCount(user);

	auto asdu = sstate.challenge.GetCriticalASDU();
	auto objects = asdu.Skip(APDU_REQUEST_HEADER_SIZE);
	this->ProcessAPDU(asdu, criticalHeader, objects); // process as normal	
}

bool OAuthContext::TransmitChallenge(const openpal::ReadBufferView& apdu, const opendnp3::APDUHeader& header)
{
	auto response = this->StartAuthResponse();
	auto success = sstate.challenge.WriteChallenge(apdu, header, response, sstate.hmac.GetType(), *sstate.pCrypto, &this->logger);
	if (success)
	{
		this->BeginTx(response.ToReadOnly());
	}
	return success;
}

openpal::IKeyWrapAlgo& OAuthContext::GetKeyWrapAlgo(openpal::ICryptoProvider& crypto, UpdateKeyMode type)
{
	switch (type)
	{
	case(UpdateKeyMode::AES128) :
		return crypto.GetAES128KeyWrap();
	default:
		return crypto.GetAES256KeyWrap();
	}
}

void OAuthContext::RespondWithAuthError(
	const opendnp3::APDUHeader& header,	
	uint32_t seqNum,
	const User& user,
	AuthErrorCode code	
	)
{
	auto rsp = this->StartAuthResponse();
	rsp.SetFunction(FunctionCode::AUTH_RESPONSE);
	rsp.SetControl(header.control);
	auto writer = rsp.GetWriter();

	DNPTime time(sstate.pTimeSource->Now().msSinceEpoch);

	Group120Var7 error(
		seqNum,
		user.GetId(),
		sstate.settings.assocId,
		code,
		DNPTime(sstate.pTimeSource->Now().msSinceEpoch),
		ReadBufferView::Empty()
	);
	
	this->Increment(SecurityStatIndex::ERROR_MESSAGES_TX);

	writer.WriteFreeFormat(error);

	this->BeginTx(rsp.ToReadOnly());
}

void OAuthContext::IncrementSessionAuthCount(const opendnp3::User& user)
{
	this->sstate.sessions.IncrementAuthCount(user);	
}

}


