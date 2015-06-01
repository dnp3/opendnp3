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

#include "SessionKeyTask.h"

#include "KeyStatusHandler.h"
#include "KeyWrap.h"

#include "secauth/AuthConstants.h"

#include "secauth/Crypto.h"
#include "secauth/HMACProvider.h"

#include <opendnp3/app/parsing/APDUParser.h>

#include <openpal/crypto/SecureCompare.h>


using namespace opendnp3;
using namespace openpal;

namespace secauth
{

SessionKeyTask::SessionKeyTask(	opendnp3::IMasterApplication& application,
								openpal::TimeDuration retryPeriod_,
								openpal::Logger logger,
								const User& user_,
								MSState& msstate) :

							opendnp3::IMasterTask(application, openpal::MonotonicTimestamp::Min(), logger, nullptr, -1),
							retryPeriod(retryPeriod_),
							user(user_),
							pmsstate(&msstate),
							keyChangeSeqNum(0)
{
	
}

void SessionKeyTask::BuildRequest(opendnp3::APDURequest& request, uint8_t seq)
{
	if (state == State::GetStatus)
	{
		this->BuildStatusRequest(request, seq);
	}
	else
	{
		this->BuildSessionKeyRequest(request, seq);
	}
}

void SessionKeyTask::Initialize()
{
	this->state = State::GetStatus;
}

IMasterTask::ResponseResult SessionKeyTask::_OnResponse(const APDUResponseHeader& header, const openpal::ReadBufferView& objects)
{
	if (ValidateSingleResponse(header))
	{
		return (state == State::GetStatus) ? OnStatusResponse(header, objects) : OnChangeResponse(header, objects);
	}
	else
	{
		return ResponseResult::ERROR_BAD_RESPONSE;
	}
}

void SessionKeyTask::OnResponseError(openpal::MonotonicTimestamp now)
{
	expiration = now.Add(this->retryPeriod);
}

void SessionKeyTask::OnResponseOK(openpal::MonotonicTimestamp now)
{
	expiration = MonotonicTimestamp::Max();
}

void SessionKeyTask::_OnResponseTimeout(openpal::MonotonicTimestamp now)
{
	expiration = now.Add(this->retryPeriod);
}

void SessionKeyTask::_OnLowerLayerClose(openpal::MonotonicTimestamp now)
{
	expiration = 0;
}

void SessionKeyTask::BuildStatusRequest(opendnp3::APDURequest& request, uint8_t seq)
{
	request.ConfigureHeader(FunctionCode::AUTH_REQUEST, seq);

	Group120Var4 ksrequest;
	ksrequest.userNum = user.GetId();

	request.GetWriter().WriteSingleValue<UInt8, Group120Var4>(QualifierCode::UINT8_CNT, ksrequest);
}

void SessionKeyTask::BuildSessionKeyRequest(opendnp3::APDURequest& request, uint8_t seq)
{
	request.ConfigureHeader(FunctionCode::AUTH_REQUEST, seq);

	Group120Var6 sessionKeyChange;
	sessionKeyChange.keyChangeSeqNum = this->keyChangeSeqNum;
	sessionKeyChange.userNum = user.GetId();
	sessionKeyChange.keyWrapData = this->keyWrapBuffer.GetWrappedData();
	
	request.GetWriter().WriteFreeFormat(sessionKeyChange);

	// save a view of what we're transmitting as we'll need it to validate the HMAC
	this->txKeyWrapASDU = request.ToReadOnly();
}

IMasterTask::ResponseResult SessionKeyTask::OnStatusResponse(const APDUResponseHeader& response, const ReadBufferView& objects)
{
	KeyStatusHandler handler(this->logger);
	if (IsFailure(APDUParser::Parse(objects, handler, this->logger)))
	{
		return ResponseResult::ERROR_BAD_RESPONSE;
	}

	Group120Var5 status;
	ReadBufferView rawObject;
	
	if (!handler.GetStatus(status, rawObject))
	{
		SIMPLE_LOG_BLOCK(this->logger, flags::WARN, "Response did not contain a key status object");
		return ResponseResult::ERROR_BAD_RESPONSE;
	}

	// save the KSQ
	this->keyChangeSeqNum = status.keyChangeSeqNum;
	
	if (!AuthConstants::ChallengeDataSizeWithinLimits(status.challengeData.Size()))
	{
		SIMPLE_LOG_BLOCK(this->logger, flags::WARN, "Bad challenge data size");
		return ResponseResult::ERROR_BAD_RESPONSE;
	}

	// before we derive keys, make sure we support the specified key wrap algorithm
	const auto pKeyWrapAlgo = Crypto::TryGetKeyWrap(*pmsstate->pCrypto, status.keyWrapAlgo);
	if (!pKeyWrapAlgo)
	{
		FORMAT_LOG_BLOCK(this->logger, flags::WARN, "Unsupported key wrap algorithm: %s", KeyWrapAlgorithmToString(status.keyWrapAlgo));
		return ResponseResult::ERROR_BAD_RESPONSE;
	}

	// TODO - make the session key size configurable
	if (!keys.DeriveFrom(*(pmsstate->pCrypto), AuthConstants::MIN_SESSION_KEY_SIZE_BYTES))
	{
		SIMPLE_LOG_BLOCK(this->logger, flags::WARN, "Unable to derive session keys");
		return ResponseResult::ERROR_BAD_RESPONSE; // TODO - add a different return code or is this good enough?
	}

	// get the users update key
	openpal::ReadBufferView updateKey;
	UpdateKeyMode updateKeyMode = this->pmsstate->pUser->GetUpdateKey(updateKey);

	if (!Crypto::KeyLengthMatchesRequestedAlgorithm(status.keyWrapAlgo, updateKey.Size()))
	{
		SIMPLE_LOG_BLOCK(this->logger, flags::WARN, "Update key length does not match outstation KeyWrapAlgorithm");
		return ResponseResult::ERROR_BAD_RESPONSE;
	}
	
	if (!this->keyWrapBuffer.Wrap(*pKeyWrapAlgo, updateKey, this->keys.GetView(), rawObject, this->logger))
	{
		SIMPLE_LOG_BLOCK(this->logger, flags::WARN, "Unable to wrap session keys");
		return ResponseResult::ERROR_BAD_RESPONSE;
	}

	// The wrapped key data is now stored in the keyWrapBuffer until we can send it out

	this->state = State::ChangeKey; // we're now ready to try changing the key itself

	return ResponseResult::OK_REPEAT;
 }

IMasterTask::ResponseResult SessionKeyTask::OnChangeResponse(const APDUResponseHeader& response, const ReadBufferView& objects)
{
	KeyStatusHandler handler(this->logger);
	if (IsFailure(APDUParser::Parse(objects, handler, this->logger)))
	{
		return ResponseResult::ERROR_BAD_RESPONSE;
	}

	Group120Var5 status;
	ReadBufferView rawObject;

	if (!handler.GetStatus(status, rawObject))
	{
		SIMPLE_LOG_BLOCK(this->logger, flags::WARN, "Response did not contain a key status object");
		return ResponseResult::ERROR_BAD_RESPONSE;
	}
	
	HMACMode hmacMode;
	if (!Crypto::TryGetHMACMode(status.hmacAlgo, hmacMode))
	{
		FORMAT_LOG_BLOCK(this->logger, flags::WARN, "Outstation requested unsupported HMAC algorithm: %s", HMACTypeToString(status.hmacAlgo));
		return ResponseResult::ERROR_BAD_RESPONSE;
	}

	HMACProvider hmac(*pmsstate->pCrypto, hmacMode);
	auto hmacValue = hmac.Compute(keys.GetView().monitorKey, { this->txKeyWrapASDU });
	if (hmacValue.IsEmpty())
	{
		SIMPLE_LOG_BLOCK(this->logger, flags::ERR, "Unable to calculate HMAC value");
		return ResponseResult::ERROR_BAD_RESPONSE;
	}	

	if (!SecureEquals(hmacValue, status.hmacValue))
	{
		SIMPLE_LOG_BLOCK(this->logger, flags::ERR, "Invalid HMAC received from outstation during session key swap");
		return ResponseResult::ERROR_BAD_RESPONSE;
	}

	// Set the session keys and return!
	pmsstate->session.SetKeys(keys.GetView());

	return ResponseResult::OK_FINAL;
}

}

