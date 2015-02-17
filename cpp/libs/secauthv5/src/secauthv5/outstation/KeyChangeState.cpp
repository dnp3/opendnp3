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

#include "KeyChangeState.h"

#include <opendnp3/gen/KeyWrapAlgorithm.h>



using namespace openpal;
using namespace opendnp3;

namespace secauthv5
{

KeyChangeState::KeyChangeState(uint16_t userNum, uint8_t challengeSize_, openpal::ICryptoProvider& provider) :
	USER_NUM(userNum),
	challengeSize(AuthConstants::GetBoundedChallengeSize(challengeSize_)),
	pProvider(&provider),
	keyChangeSeqNum(0)
{}

bool KeyChangeState::FormatKeyStatusResponse(opendnp3::APDUResponse& rsp, const opendnp3::AppControlField& control, opendnp3::KeyStatus status, const openpal::ReadBufferView& hmac)
{
	WriteBufferView buff(challengeBuffer, challengeSize);

	if (pProvider->GetSecureRandom(buff))
	{
		++keyChangeSeqNum;		

		rsp.SetFunction(FunctionCode::AUTH_RESPONSE);
		rsp.SetControl(control);

		statusRsp.keyChangeSeqNum = keyChangeSeqNum;
		statusRsp.userNum = USER_NUM;
		statusRsp.keywrapAlgorithm = KeyWrapAlgorithm::AES_128;
		statusRsp.keyStatus = KeyStatus::NOT_INIT;
		statusRsp.hmacType = HMACType::HMAC_SHA1_TRUNC_10;
		statusRsp.challengeData = buff.ToReadOnly();		
		statusRsp.hmacValue = hmac;

		return rsp.GetWriter().WriteFreeFormat(statusRsp);
	}
	else
	{
		return false;
	}	
}
	
}
