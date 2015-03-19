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

#include "Group120Var5.h"

#include <openpal/serialization/Serialization.h>

using namespace openpal;

namespace opendnp3 {

Group120Var5Def::Group120Var5Def() :
	keyChangeSeqNum(0),
	userNum(0),
	keywrapAlgorithm(KeyWrapAlgorithm::UNDEFINED),
	keyStatus(KeyStatus::UNDEFINED),
	hmacType(HMACType::UNKNOWN)
{
	
}

Group120Var5Def::Group120Var5Def(
		uint32_t keyChangeSeqNum_,
		uint16_t userNum_,
		KeyWrapAlgorithm keywrapAlgorithm_,
		KeyStatus keyStatus_,
		HMACType hmacType_,
		openpal::ReadBufferView challengeData_,
		openpal::ReadBufferView hmacValue_
	) :
	keyChangeSeqNum(keyChangeSeqNum_),
	userNum(userNum_),
	keywrapAlgorithm(keywrapAlgorithm_),
	keyStatus(keyStatus_),
	hmacType(hmacType_),
	challengeData(challengeData_),
	hmacValue(hmacValue_)
{
	
}

uint32_t Group120Var5Def::Size() const
{
	return FIXED_BASE_SIZE + challengeData.Size() + hmacValue.Size();
}
	
	
bool Group120Var5Def::Read(const openpal::ReadBufferView& buffer, Group120Var5Def& output)
{
	if (buffer.Size() < FIXED_BASE_SIZE)
	{
		return false;
	}
	else
	{
		// we know there's enough to read the fixed size fields, so just advance the copy of the buffer
		ReadBufferView copy(buffer);

		output.keyChangeSeqNum = UInt32::ReadBuffer(copy);
		output.userNum = UInt16::ReadBuffer(copy);
		output.keywrapAlgorithm = KeyWrapAlgorithmFromType(UInt8::ReadBuffer(copy));
		output.keyStatus = KeyStatusFromType(UInt8::ReadBuffer(copy));
		output.hmacType = HMACTypeFromType(UInt8::ReadBuffer(copy));

		uint16_t challengeLength = UInt16::ReadBuffer(copy);

		if (copy.Size() >= challengeLength)
		{
			output.challengeData = copy.Take(challengeLength);				
			output.hmacValue = copy.Skip(challengeLength);
			return true;
		}
		else
		{
			// not enough bytes for challenge data
			return false;
		}
	}
}

bool Group120Var5Def::Write(const Group120Var5Def& output, openpal::WriteBufferView& buffer)
{
	if (	output.challengeData.Size() <= openpal::MaxValue<uint16_t>() &&
			output.hmacValue.Size() <= openpal::MaxValue<uint16_t>() &&
			output.Size() <= buffer.Size())	
	{
		UInt32::WriteBuffer(buffer, output.keyChangeSeqNum);
		UInt16::WriteBuffer(buffer, output.userNum);
		UInt8::WriteBuffer(buffer, KeyWrapAlgorithmToType(output.keywrapAlgorithm));
		UInt8::WriteBuffer(buffer, KeyStatusToType(output.keyStatus));
		UInt8::WriteBuffer(buffer, HMACTypeToType(output.hmacType));
		
		// we known this cast is correct because of the pre-condition in the "if" above
		uint16_t challengeDataSize = static_cast<uint16_t>(output.challengeData.Size());
		UInt16::WriteBuffer(buffer, challengeDataSize);
		
		// these will always work b/c of the total size check in the "if" above
		output.challengeData.CopyTo(buffer);
		output.hmacValue.CopyTo(buffer);

		return true;
	}
	else
	{
		return false;
	}
}

}

