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

#include "Group120Var2.h"

#include <openpal/logging/LogMacros.h>
#include <openpal/serialization/Serialization.h>

#include <opendnp3/LogLevels.h>

using namespace openpal;

namespace opendnp3 {

Group120Var2::Group120Var2() : 
	challengeSeqNum(0),
	userNum(0)	
{
		
}

Group120Var2::Group120Var2(
	uint32_t challengeSeqNum_,
	uint16_t userNum_,	
	const openpal::ReadBufferView& challengeData_
	) : 
	challengeSeqNum(challengeSeqNum_),
	userNum(userNum_),	
	challengeData(challengeData_)
{

}

uint32_t Group120Var2::Size() const
{
	return 6 + challengeData.Size();
}

bool Group120Var2::Read(const openpal::ReadBufferView& data, Group120Var2& output, openpal::Logger* pLogger)
{
	if (data.Size() < MIN_SIZE)
	{
		SIMPLE_LOGGER_BLOCK(pLogger, flags::WARN, "Not enough data for g120v1 object");
		return false;
	}
	else
	{
		ReadBufferView copy(data);
		output.challengeSeqNum = UInt32::ReadBuffer(copy);		
		output.userNum = UInt16::ReadBuffer(copy);		
		output.challengeData = copy;
		return true;
	}
}

bool Group120Var2::Write(const Group120Var2& output, openpal::WriteBufferView& buffer)
{
	if (buffer.Size() < output.Size())
	{
		return false;
	}
	else
	{
		UInt32::WriteBuffer(buffer, output.challengeSeqNum);
		UInt16::WriteBuffer(buffer, output.userNum);		
		buffer.Advance(output.challengeData.CopyTo(buffer).Size());
		return true;
	}
}

}
