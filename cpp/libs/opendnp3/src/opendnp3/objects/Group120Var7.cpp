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

#include "Group120Var7.h"

#include <openpal/serialization/Serialization.h>

using namespace openpal;

namespace opendnp3 {

	Group120Var7::Group120Var7() :
		seqNum(0),
		userNum(0),
		associationID(0),
		errorCode(AuthErrorCode::UNKNOWN),
		timeOfError(0)		
	{
	
	}
	
	Group120Var7::Group120Var7(
		uint32_t seqNum_,
		uint16_t userNum_,
		uint16_t associationID_,
		AuthErrorCode errorCode_,
		uint64_t timeOfError_,
		openpal::ReadBufferView errorText_
		) :
		seqNum(seqNum_),
		userNum(userNum_),
		associationID(associationID_),
		errorCode(errorCode_),
		timeOfError(timeOfError_),
		errorText(errorText_)
	{}

	uint32_t Group120Var7::Size() const
	{
		return FIXED_BASE_SIZE + errorText.Size();
	}
	
	bool Group120Var7::Read(const ReadBufferView& buffer, Group120Var7& output)
	{
		if (buffer.Size() < output.Size())
		{
			return false;
		}
		else
		{
			ReadBufferView copy(buffer);
			output.seqNum = UInt32::ReadBuffer(copy);
			output.userNum = UInt16::ReadBuffer(copy);
			output.associationID = UInt16::ReadBuffer(copy);
			output.errorCode = AuthErrorCodeFromType(UInt8::ReadBuffer(copy));
			output.timeOfError = UInt48::ReadBuffer(copy);
			output.errorText = copy; //whatever is left over
			return true;
		}
	}

	bool Group120Var7::Write(const Group120Var7& output, openpal::WriteBufferView& buffer)
	{
		if (buffer.Size() < output.Size())
		{
			return false;
		}
		else
		{			
			UInt32::WriteBuffer(buffer, output.seqNum);
			UInt16::WriteBuffer(buffer, output.userNum);
			UInt16::WriteBuffer(buffer, output.associationID);
			UInt8::WriteBuffer(buffer, AuthErrorCodeToType(output.errorCode));
			UInt48::WriteBuffer(buffer, output.timeOfError);
			buffer.ReadFrom(output.errorText);							
			return true;
		}
	}
}


