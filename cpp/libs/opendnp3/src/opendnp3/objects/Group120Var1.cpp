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

#include "Group120Var1.h"

#include <openpal/logging/LogMacros.h>
#include <openpal/serialization/Serialization.h>

#include <opendnp3/LogLevels.h>

using namespace openpal;

namespace opendnp3 {

Group120Var1::Group120Var1() : 
	challengeSeqNum(0),
	userNum(0),
	hmacType(HMACType::UNKNOWN),
	reason(ChallengeReason::UNKNOWN)	
{
		
}

Group120Var1::Group120Var1(
	uint32_t challengeSeqNum_,
	uint16_t userNum_,
	HMACType hmacType_,
	ChallengeReason reason_,
	const openpal::ReadBufferView& challengeData_
	) : 
	challengeSeqNum(challengeSeqNum_),
	userNum(userNum_),
	hmacType(hmacType_),
	reason(reason_),
	challengeData(challengeData_)
{

}

uint32_t Group120Var1::Size() const
{
	return 8 + challengeData.Size();
}

bool Group120Var1::Read(const openpal::ReadBufferView& data, Group120Var1& output)
{
	if (data.Size() < FIXED_BASE_SIZE)
	{		
		return false;
	}
	else
	{
		ReadBufferView copy(data);
		output.challengeSeqNum = UInt32::ReadBuffer(copy);		
		output.userNum = UInt16::ReadBuffer(copy);
		output.hmacType = HMACTypeFromType(UInt8::ReadBuffer(copy));
		output.reason = ChallengeReasonFromType(UInt8::ReadBuffer(copy));
		output.challengeData = copy;
		return true;
	}
}

bool Group120Var1::Write(const Group120Var1& output, openpal::WriteBufferView& buffer)
{
	if (buffer.Size() < output.Size())
	{
		return false;
	}
	else
	{
		UInt32::WriteBuffer(buffer, output.challengeSeqNum);
		UInt16::WriteBuffer(buffer, output.userNum);
		UInt8::WriteBuffer(buffer, HMACTypeToType(output.hmacType));
		UInt8::WriteBuffer(buffer, ChallengeReasonToType(output.reason));
		buffer.ReadFrom(output.challengeData);		
		return true;
	}
}

}
