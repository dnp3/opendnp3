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
#include <openpal/crypto/SecureCompare.h>
#include <openpal/serialization/Format.h>

#include "opendnp3/LogLevels.h"

#include "opendnp3/app/parsing/APDUParser.h"
#include "opendnp3/app/parsing/ObjectHeaderParser.h"
#include "opendnp3/app/QualityFlags.h"
#include "opendnp3/outstation/OutstationContext.h"
#include "opendnp3/outstation/IINHelpers.h"


#include "secauth/AggressiveModeParser.h"
#include "secauth/outstation/KeyUnwrap.h"
#include "secauth/SingleObjectHandlers.h"
#include "secauth/outstation/OutstationErrorCodes.h"

#include "secauth/StringConversions.h"
#include "secauth/Crypto.h"

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
	SecurityStat zero(opendnp3::flags::ONLINE, security.settings.assocId, 0, DNPTime(0));

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
			openpal::ICryptoProvider& crypto
		) :
		OContext(config, EnableSecStats(dbTemplate), logger, executor, lower, commandHandler, application),
		security(config.params, settings, logger, executor, application, crypto)
{
	this->ConfigureSecStats(security.settings.statThresholds);
}

void OAuthContext::AddUser(opendnp3::User user, const std::string& userName, const secauth::UpdateKey& key, const secauth::Permissions& permissions)
{
	security.userDB.AddUser(user, userName, key, permissions);
}

void OAuthContext::ConfigureAuthority(uint32_t statusChangeSeqNumber, const secauth::AuthorityKey& key)
{
	security.credentials.Configure(statusChangeSeqNumber, key);
}

bool OAuthContext::OnLowerLayerDown()
{
	auto ret = OContext::OnLowerLayerDown();

	if (ret)
	{
		this->security.state = SecurityState::IDLE;
		this->security.challengeTimer.Cancel();
		this->security.sessions.Clear();
	}

	return ret;
}

void OAuthContext::CheckForTaskStart()
{
	if (this->CanTransmit() && security.deferred.IsSet())
	{
		auto handler = [this](const openpal::RSlice& apdu, const APDUHeader& header, const RSlice& objects)
		{
			this->ProcessAuthAPDU(apdu, header, objects);
			return true;
		};

		security.deferred.Process(handler);
	}

	OContext::CheckForTaskStart();
}
		
void OAuthContext::ReceiveParsedHeader(const openpal::RSlice& apdu, const APDUHeader& header, const openpal::RSlice& objects)
{	
	if (this->CanTransmit())
	{
		if (this->ProcessAuthAPDU(apdu, header, objects) == APDUResult::DISCARDED)
		{
			this->Increment(SecurityStatIndex::DISCARED_MESSAGES);
		}
	}
	else
	{
		security.deferred.SetASDU(header, apdu);
	}
}

OAuthContext::APDUResult OAuthContext::ProcessAuthAPDU(const openpal::RSlice& apdu, const APDUHeader& header, const openpal::RSlice& objects)
{
	switch (header.function)
	{
		case(FunctionCode::AUTH_REQUEST) :
			return this->ProcessAuthRequest(apdu, header, objects);
		case(FunctionCode::AUTH_RESPONSE) :
			SIMPLE_LOG_BLOCK(this->logger, flags::WARN, "AuthResponse not valid for outstation");
			return APDUResult::DISCARDED;
		case(FunctionCode::AUTH_REQUEST_NO_ACK) :
			SIMPLE_LOG_BLOCK(this->logger, flags::WARN, "AuthRequestNoAck not supported");
			return APDUResult::DISCARDED;
		default:
			return this->ProcessNormalFunction(apdu, header, objects);			
	}
}

OAuthContext::APDUResult OAuthContext::ProcessAuthRequest(const openpal::RSlice& apdu, const APDUHeader& header, const openpal::RSlice& objects)
{
	if (header.control.UNS)
	{
		SIMPLE_LOG_BLOCK(this->logger, flags::WARN, "Discarding AuthRequest with UNS bit set");
		return APDUResult::DISCARDED;
	}	
	
	// the AUTH_REQUEST function code is massively overloaded and does many different things, which makes it
	// particularly hard to handle with a stream parser like the one opendnp3 uses. You only don't respond to 
	// every message you receive.
	//
	// We can, however, figure out the intent of the request by examing the first object header. This makes 
	// the actual parsing much easier to constrain with dedicated handlers for each semantic path.	

	GroupVariation gv = GroupVariation::UNKNOWN;
	if (!ObjectHeaderParser::ReadFirstGroupVariation(objects, gv))
	{
		SIMPLE_LOG_BLOCK(this->logger, flags::WARN, "Discarding AuthRequest w/ empty or malformed first object header");
		return APDUResult::DISCARDED;
	}

	switch (gv)
	{
		case(GroupVariation::Group120Var2) : // authentication reply
			return this->ProcessChallengeReply(apdu, header, objects);
		case(GroupVariation::Group120Var4) :
			return this->ProcessRequestKeyStatus(apdu, header, objects);
		case(GroupVariation::Group120Var6) :
			return this->ProcessChangeSessionKeys(apdu, header, objects);
		case(GroupVariation::Group120Var10) :
			return this->ProcessUserStatusChange(apdu, header, objects);
		default:
			// this function/object combination not supported
			return APDUResult::DISCARDED;
	}
}



OAuthContext::APDUResult OAuthContext::ProcessChallengeReply(const openpal::RSlice& apdu, const opendnp3::APDUHeader& header, const openpal::RSlice& objects)
{	
	if (security.state == SecurityState::IDLE)
	{		
		SIMPLE_LOG_BLOCK(logger, flags::WARN, "Discarding unexpected challenge reply");
		return APDUResult::DISCARDED;
	}

	// we're waiting for a response so we need to authenticate it
	ChallengeReplyHandler handler;
	auto result = APDUParser::Parse(objects, handler, this->logger);

	if (!(result == ParseResult::OK && handler.IsValid()))
	{
		SIMPLE_LOG_BLOCK(logger, flags::WARN, "Discarding malformed challenge reply");
		return APDUResult::DISCARDED;
	}

	auto& reply = handler.value;

	// no matter what happens with the autentication at this point, we return to the Idle state
	this->security.state = SecurityState::IDLE;
	this->security.challengeTimer.Cancel();

	// first look-up the session for the specified user	
	User user(reply.userNum);
	SessionKeysView keys;

	if (security.sessions.TryGetSessionKeys(user, keys) != KeyStatus::OK)
	{
		++(this->security.otherStats.authFailuresDueToExpiredKeys);
		this->Increment(SecurityStatIndex::AUTHENTICATION_FAILURES);
		FORMAT_LOG_BLOCK(this->logger, flags::WARN, "No valid session keys for user %u", user.GetId());
		return this->TryRespondWithAuthError(header.control.SEQ, reply.challengeSeqNum, user, AuthErrorCode::AUTHENTICATION_FAILED);		
	}
	
	if (!security.challenge.VerifyAuthenticity(keys.controlKey, security.hmac, reply.hmacValue, this->logger))
	{
		this->Increment(SecurityStatIndex::AUTHENTICATION_FAILURES);
		FORMAT_LOG_BLOCK(this->logger, flags::WARN, "Authentication failure for user %u", user.GetId());
		return this->TryRespondWithAuthError(header.control.SEQ, reply.challengeSeqNum, user, AuthErrorCode::AUTHENTICATION_FAILED);
	}

	// this increments both the security statistic and the count on the key
	this->IncrementSessionAuthCount(user);

	auto criticalHeader = security.challenge.GetCriticalHeader();
	
	if (!security.userDB.IsAuthorized(user, criticalHeader.function))
	{
		this->Increment(SecurityStatIndex::AUTHORIZATION_FAILURES);
		FORMAT_LOG_BLOCK(this->logger, flags::WARN, "Verified user %u is not authorized for function %s", user.GetId(), FunctionCodeToString(criticalHeader.function));
		return this->TryRespondWithAuthError(header.control.SEQ, reply.challengeSeqNum, user, AuthErrorCode::AUTHORIZATION_FAILED);
	}

	auto asdu = security.challenge.GetCriticalASDU();
	
	this->ProcessAPDU(asdu, criticalHeader, asdu.Skip(APDU_REQUEST_HEADER_SIZE)); // process as normal
	return APDUResult::PROCESSED;
}

OAuthContext::APDUResult OAuthContext::ProcessRequestKeyStatus(const openpal::RSlice& apdu, const opendnp3::APDUHeader& header, const openpal::RSlice& objects)
{
	if (this->security.state == SecurityState::WAIT_FOR_REPLY)
	{
		SIMPLE_LOG_BLOCK(logger, flags::WARN, "Discarding key status request while waiting for challenge reply");
		this->Increment(SecurityStatIndex::UNEXPECTED_MESSAGES);
		return APDUResult::DISCARDED;
	}

	RequestKeyStatusHandler handler;
	auto result = APDUParser::Parse(objects, handler, this->logger);
	if (!(result == ParseResult::OK && handler.IsValid()))
	{
		SIMPLE_LOG_BLOCK(logger, flags::WARN, "Discarding bad key status request");
		return APDUResult::DISCARDED;
	}

	User user(handler.value.userNum);
	UpdateKeyMode type;
	if (!security.userDB.GetUpdateKeyType(user, type))
	{		
		FORMAT_LOG_BLOCK(this->logger, flags::WARN, "Discarding key status request for unknown user: %u", user.GetId());
		this->Increment(SecurityStatIndex::UNEXPECTED_MESSAGES);
		return APDUResult::DISCARDED;
	}

	auto keyStatus = security.sessions.GetSessionKeyStatus(user);

	auto response = this->StartAuthResponse(header.control.SEQ);
	auto writer = response.GetWriter();
	auto hmacType = HMACType::NO_MAC_VALUE;

	auto success = security.keyChangeState.FormatKeyStatusResponse(
		writer,
		user,
		hmacType,
		Crypto::ToKeyWrapAlgorithm(type),
		keyStatus,
		RSlice::Empty()
	);

	if (!success)
	{
		SIMPLE_LOG_BLOCK(this->logger, flags::WARN, "Unable to format key status response");
		return APDUResult::DISCARDED;
	}
	
	this->BeginTx(response.ToRSlice());
	return APDUResult::PROCESSED;
}

OAuthContext::APDUResult OAuthContext::ProcessChangeSessionKeys(const openpal::RSlice& apdu, const opendnp3::APDUHeader& header, const openpal::RSlice& objects)
{
	// TODO - this isn't quite right, but it's harmless for now, and possibly better behavior
	if (security.state == SecurityState::WAIT_FOR_REPLY)
	{
		SIMPLE_LOG_BLOCK(logger, flags::WARN, "Discarding session key change request while waiting for challenge response");
		this->Increment(SecurityStatIndex::UNEXPECTED_MESSAGES);
		return APDUResult::DISCARDED;
	}

	// First we need to determine if this is valid or not
	ChangeSessionKeysHandler handler;
	auto result = APDUParser::Parse(objects, handler, this->logger);
	if (!(result == ParseResult::OK && handler.IsValid()))
	{
		SIMPLE_LOG_BLOCK(logger, flags::WARN, "Discarding bad session key change request");		
		this->Increment(SecurityStatIndex::UNEXPECTED_MESSAGES);
		return APDUResult::DISCARDED;		
	}

	User user(handler.value.userNum);

	if (!security.keyChangeState.CheckUserAndKSQMatches(user, handler.value.keyChangeSeqNum))
	{
		return this->TryRespondWithAuthError(header.control.SEQ, handler.value.keyChangeSeqNum, user, AuthErrorCode::UNKNOWN_USER);
	}

	UpdateKeyMode updateKeyType;
	RSlice updateKey;

	if (!security.userDB.GetUpdateKey(user, updateKeyType, updateKey))
	{
		FORMAT_LOG_BLOCK(this->logger, flags::WARN, "Ignoring session key change request for unknown user %u", user.GetId());
		return TryRespondWithAuthError(header.control.SEQ, handler.value.keyChangeSeqNum, user, AuthErrorCode::UNKNOWN_USER);
	}

	UnwrappedKeyData unwrapped;
	KeyUnwrapBuffer buffer;

	auto unwrapSuccess = buffer.Unwrap(
		GetKeyWrapAlgo(*security.pCrypto, updateKeyType),
		updateKey,
		handler.value.keyWrapData,
		unwrapped,
		&this->logger
	);

	if (!unwrapSuccess)
	{
		SIMPLE_LOG_BLOCK(this->logger, flags::WARN, "Failed to unwrap key data");
		return this->TryRespondWithAuthError(header.control.SEQ, handler.value.keyChangeSeqNum, user, AuthErrorCode::AUTHENTICATION_FAILED);		
	}

	if (!security.keyChangeState.EqualsLastStatusResponse(unwrapped.keyStatusObject))
	{
		return this->TryRespondWithAuthError(header.control.SEQ, handler.value.keyChangeSeqNum, user, AuthErrorCode::AUTHENTICATION_FAILED);		
	}

	// At this point, we've successfully authenticated the session key change for this user
	// We compute the HMAC based on the full ASDU and the monitoring direction session key		

	std::error_code ec;
	auto hmac = security.hmac.Compute(unwrapped.keys.monitorKey, { apdu }, ec);

	if (ec)
	{
		SIMPLE_LOG_BLOCK(this->logger, flags::ERR, ec.message().c_str());
		return APDUResult::DISCARDED;
	}

	security.sessions.SetSessionKeys(user, unwrapped.keys);

	auto response = this->StartAuthResponse(header.control.SEQ);
	
	auto writer = response.GetWriter();
	auto hmacType = security.hmac.GetType();

	if (!security.keyChangeState.FormatKeyStatusResponse(
					writer,
					user,
					hmacType,
					Crypto::ToKeyWrapAlgorithm(updateKeyType),
					KeyStatus::OK,
					hmac)
		)
	{
		return APDUResult::DISCARDED;
	}


	this->BeginTx(response.ToRSlice());
	return APDUResult::PROCESSED;
}

OAuthContext::APDUResult OAuthContext::ProcessNormalFunction(const openpal::RSlice& apdu, const APDUHeader& header, const openpal::RSlice& objects)
{	
	const bool IS_CRITICAL = security.settings.functions.IsCritical(header.function);

	if (security.settings.functions.IsCritical(header.function))
	{
		this->Increment(SecurityStatIndex::CRITICAL_MESSAGES_RX);
	}

	if (security.state == SecurityState::WAIT_FOR_REPLY)
	{
		this->Increment(SecurityStatIndex::UNEXPECTED_MESSAGES);
		return APDUResult::DISCARDED;
	}
	
	if (!IS_CRITICAL)
	{
		// no authentication required, just process it
		this->ProcessAPDU(apdu, header, objects);
		return APDUResult::PROCESSED;
	}


	if (!this->TransmitChallenge(apdu, header)) // this could fail because of the PRNG
	{
		return APDUResult::DISCARDED;
	}

	this->security.state = SecurityState::WAIT_FOR_REPLY;	

	security.challengeTimer.Restart(security.settings.challengeTimeout, [this]() { this->OnChallengeTimeout(); });
	
	return APDUResult::PROCESSED;
}

OAuthContext::APDUResult OAuthContext::ProcessUserStatusChange(const openpal::RSlice& fragment, const opendnp3::APDUHeader& header, const openpal::RSlice& objects)
{
	if (security.state == SecurityState::WAIT_FOR_REPLY)
	{		
		this->Increment(SecurityStatIndex::UNEXPECTED_MESSAGES);
		return APDUResult::DISCARDED;
	}

	UserStatusChangeHandler handler;
	auto result = APDUParser::Parse(objects, handler, this->logger);
	if (!(result == ParseResult::OK && handler.IsValid()))
	{
		SIMPLE_LOG_BLOCK(logger, flags::WARN, "Discarding bad user status change request");
		this->Increment(SecurityStatIndex::UNEXPECTED_MESSAGES);
		return APDUResult::DISCARDED;
	}

	if (!this->AuthenticateUserStatusChange(header, handler.value))
	{
		return APDUResult::DISCARDED;
	}
	
	// only supported key change method for the time being
	if (handler.value.keyChangeMethod != KeyChangeMethod::AES_256_SHA256_HMAC)
	{
		FORMAT_LOG_BLOCK(logger, flags::WARN, "Unsupported key change method: %s", KeyChangeMethodToString(handler.value.keyChangeMethod));
		return this->TryRespondWithAuthError(header.control.SEQ, handler.value.statusChangeSeqNum, User::Unknown(), AuthErrorCode::UPDATE_KEY_METHOD_NOT_PERMITTED);		
	}

	auto userName = ToString(handler.value.userName);

	switch (handler.value.userOperation)
	{
		case(UserOperation::OP_ADD) :
			return this->ProcessUserStatusChange_Add(header, userName, handler.value);
		case(UserOperation::OP_CHANGE) :
			return this->ProcessUserStatusChange_Change(header, userName, handler.value);
		case(UserOperation::OP_DELETE) :
			return this->ProcessUserStatusChange_Delete(header, userName, handler.value);
		default:
			FORMAT_LOG_BLOCK(logger, flags::WARN, "Unsupported user operation: %s", UserOperationToString(handler.value.userOperation));
			//  TODO just ignore this for now
			return APDUResult::DISCARDED;
	}	
}

OAuthContext::APDUResult OAuthContext::ProcessUserStatusChange_Add(const opendnp3::APDUHeader& header, const std::string& username, const opendnp3::Group120Var10& change)
{
	auto expiration = this->pExecutor->GetTime().Add(TimeDuration::Days(change.userRoleExpDays));	

	ChangeData data(
		change.keyChangeMethod,
		UserRoleFromType(change.userRole),
		expiration
	);

	this->security.statusChanges.QueueChange(username, data);

	auto response = this->StartAuthResponse(header.control.SEQ);	
	this->BeginTx(response.ToRSlice());

	return APDUResult::PROCESSED;
}

OAuthContext::APDUResult OAuthContext::ProcessUserStatusChange_Change(const opendnp3::APDUHeader& header, const std::string& username, const opendnp3::Group120Var10& change)
{	
	if (!security.userDB.UserExists(username))
	{
		FORMAT_LOG_BLOCK(this->logger, flags::WARN, "User Change - user does not exist: %s", username);
		return this->TryRespondWithAuthError(header.control.SEQ, change.statusChangeSeqNum, User::Unknown(), AuthErrorCode::UNKNOWN_USER);
	}

	// Treat changes just like adds except that the user must exist
	return this->ProcessUserStatusChange_Add(header, username, change);
}

OAuthContext::APDUResult OAuthContext::ProcessUserStatusChange_Delete(const opendnp3::APDUHeader& header, const std::string& username, const opendnp3::Group120Var10& change)
{	
	User userid;	
	if (security.userDB.Delete(username, userid)) // if the user exists, delete and get the associated Id
	{
		FORMAT_LOG_BLOCK(this->logger, flags::WARN, "Deleting user: %s", username);

		security.sessions.Invalidate(userid); // invalidate any active sessions
		security.pApplication->OnDeleteUser(username, userid);
		
		auto response = this->StartAuthResponse(header.control.SEQ); // send empty response
		this->BeginTx(response.ToRSlice());

		return APDUResult::PROCESSED;
	}
	else
	{
		FORMAT_LOG_BLOCK(this->logger, flags::WARN, "User Delete - user does not exist: %s", username);
		return this->TryRespondWithAuthError(header.control.SEQ, change.statusChangeSeqNum, User::Unknown(), AuthErrorCode::UNKNOWN_USER);
	}	
}

void OAuthContext::OnChallengeTimeout()
{
	if (this->security.state == SecurityState::WAIT_FOR_REPLY)
	{
		this->security.state = SecurityState::IDLE;
		SIMPLE_LOG_BLOCK(logger, flags::WARN, "Timeout while waiting for challenge response");
		this->Increment(SecurityStatIndex::DISCARED_MESSAGES);
	}	
}

bool OAuthContext::AuthenticateUserStatusChange(const opendnp3::APDUHeader& header, const opendnp3::Group120Var10& change)
{
	RSlice key;
	uint32_t statusChangeSeq = 0;

	/// check if the outstation is even configured for user status changes
	if (!security.credentials.GetSymmetricKey(statusChangeSeq, key))
	{
		SIMPLE_LOG_BLOCK(logger, flags::WARN, "Cannot process user status change because no authority credentials have been defined");
		this->TryRespondWithAuthError(header.control.SEQ, change.statusChangeSeqNum, User::Unknown(), AuthErrorCode::UPDATE_KEY_METHOD_NOT_PERMITTED);
		return false;
	}

	/**
	* A static buffer to hold some of the values for the HMAC (pg 743)
	*
	* Operation
	* Status Change Sequennce Number
	* User Role
	* User Role Expiry Interval
	* User Name Length
	* User Name
	*/
	openpal::StaticBuffer<1 + 4 + 2 + 2 + 2> fields;

	{
		auto dest = fields.GetWSlice();
		Format::Many(
			dest,
			UserOperationToType(change.userOperation), 		// 1
			change.statusChangeSeqNum,		     			// 4
			change.userRole,                           		// 2
			change.userRoleExpDays,                    		// 2
			static_cast<uint16_t>(change.userName.Size())	// 2
			);
	}

	// Verify the integrity of the message
	openpal::StaticBuffer<AuthSizes::MAX_HMAC_OUTPUT_SIZE> hmacBuffer;

	std::error_code ec;
	auto dest = hmacBuffer.GetWSlice();
	auto output = this->security.pCrypto->GetSHA256HMAC().Calculate(key, { fields.ToRSlice(), change.userName }, dest, ec);
	if (ec)
	{
		FORMAT_LOG_BLOCK(logger, flags::WARN, "Error calculating HMAC value: %s", ec.message().c_str());
		return false;
	}

	if (!openpal::SecureEquals(output, change.certificationData))
	{
		SIMPLE_LOG_BLOCK(logger, flags::WARN, "Invalid certification data in user status change request");
		this->TryRespondWithAuthError(header.control.SEQ, change.statusChangeSeqNum, User::Unknown(), AuthErrorCode::INVALID_CERTIFICATION_DATA);
		this->Increment(SecurityStatIndex::AUTHENTICATION_FAILURES);
		return false;
	}

	if (change.statusChangeSeqNum < statusChangeSeq)
	{
		SIMPLE_LOG_BLOCK(logger, flags::WARN, "Invalid certification data in user status change request");
		this->TryRespondWithAuthError(header.control.SEQ, change.statusChangeSeqNum, User::Unknown(), AuthErrorCode::INVALID_CERTIFICATION_DATA); // TODO - Is this the right error code?
		// TODO update an official stats?
		this->security.otherStats.badStatusChangeSeqNum++;
		return false;
	}

	/// Now that we've authenticated a new SCSN, tell the application to persist this count to non-volatile
	auto nextSCSN = change.statusChangeSeqNum + 1;
	this->security.credentials.SetSCSN(nextSCSN);
	this->security.pApplication->OnNewSCSN(nextSCSN);
	return true;
}

bool OAuthContext::TransmitChallenge(const openpal::RSlice& apdu, const opendnp3::APDUHeader& header)
{	
	auto response = this->StartAuthResponse(header.control.SEQ);
	auto success = security.challenge.WriteChallenge(apdu, header, response, security.hmac.GetType(), *security.pCrypto, &this->logger);
	if (success)
	{
		this->BeginTx(response.ToRSlice());
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

OAuthContext::APDUResult OAuthContext::TryRespondWithAuthError(
	AppSeqNum seq,
	uint32_t authSeqNum,
	const opendnp3::User& user,
	opendnp3::AuthErrorCode code
	)
{
	auto response = this->StartAuthResponse(seq);
	
	Group120Var7 error(
		authSeqNum,
		user.GetId(),
		security.settings.assocId,
		code,
		DNPTime(security.pApplication->Now().msSinceEpoch),
		RSlice::Empty()
	);

	response.GetWriter().WriteFreeFormat(error);	

	this->Increment(SecurityStatIndex::ERROR_MESSAGES_TX);
	

	this->BeginTx(response.ToRSlice());
	return APDUResult::PROCESSED;
}

void OAuthContext::IncrementSessionAuthCount(const opendnp3::User& user)
{
	this->security.sessions.IncrementAuthCount(user);	
}

APDUResponse OAuthContext::StartAuthResponse(uint8_t seq)
{
	auto response = security.txBuffer.Start();
	response.SetFunction(FunctionCode::AUTH_RESPONSE);
	response.SetControl(AppControlField(true, true, false, false, seq));
	response.SetIIN(this->GetResponseIIN());
	return response;
}

void OAuthContext::Increment(SecurityStatIndex index)
{
	auto count = this->security.stats.Increment(index);

	DNPTime time(this->security.pApplication->Now().msSinceEpoch);

	SecurityStat stat(opendnp3::flags::ONLINE, this->security.settings.assocId, count, time);

	this->database.Update(stat, static_cast<uint16_t>(index));
}

}


