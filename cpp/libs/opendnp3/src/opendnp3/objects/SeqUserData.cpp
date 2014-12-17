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

#include "SeqUserData.h"

#include <openpal/serialization/Serialization.h>

using namespace openpal;

namespace opendnp3 {

SeqUserData::SeqUserData() : seq(0), user(0)
{}

SeqUserData::SeqUserData(
	uint32_t seq_,
	uint16_t user_,
	const openpal::ReadBufferView& data_
	) :
	seq(seq_),
	user(user_),
	data(data_)
{}

uint32_t SeqUserData::Size() const
{
	return MIN_SIZE + data.Size();
}
	
bool SeqUserData::Read(const openpal::ReadBufferView& buffer, SeqUserData& output)
{
	if (buffer.Size() < MIN_SIZE)
	{		
		return false;
	}
	else
	{
		ReadBufferView copy(buffer);
		output.seq = UInt32::ReadBuffer(copy);
		output.user = UInt16::ReadBuffer(copy);
		output.data = copy;
		return true;
	}
}

bool SeqUserData::Write(const SeqUserData& output, openpal::WriteBufferView& buffer)
{
	if (buffer.Size() < output.Size())
	{
		return false;
	}
	else
	{
		UInt32::WriteBuffer(buffer, output.seq);
		UInt16::WriteBuffer(buffer, output.user);
		buffer.Advance(output.data.CopyTo(buffer).Size());
		return true;
	}
}
	
}

