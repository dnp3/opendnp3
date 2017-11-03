/*
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
#include "BufferTestObject.h"

#include <testlib/BufferHelpers.h>

#include <memory>
#include <stdexcept>

#include <testlib/HexConversions.h>

#include <openpal/util/ToHex.h>

using namespace openpal;

namespace testlib
{

void BufferTestObject::ClearBuffer()
{
	numWrites = 0;
	buffer.clear();
}

bool BufferTestObject::BufferEquals(const openpal::RSlice& data) const
{
	return BufferEquals(data, data.Size());
}

bool BufferTestObject::BufferEquals(const uint8_t* data, size_t count) const
{

	if(count != this->buffer.size()) return false;

	for (size_t i = 0; i < count; i++) {
		if (data[i] != this->buffer[i])
		{
			return false;
		}
	}
	return true;
}

bool BufferTestObject::BufferContains(const std::string& text) const
{
	std::string s;
	for(size_t i = 0; i < this->buffer.size(); ++i)
	{
		std::string c(1, static_cast<char>(buffer[i]));
		s.append(c);
	}
	return s.find(text) != std::string::npos;
}

std::string BufferTestObject::AsHex(bool spaced) const
{	
	CopyableBuffer temp(static_cast<uint32_t>(this->buffer.size()));
	for(size_t i = 0; i < this->buffer.size(); ++i) temp[i] = this->buffer[i];
	return ToHex(temp.ToRSlice(), spaced);
}


bool BufferTestObject::BufferEqualsHex(const std::string& hex) const
{
	HexSequence hs(hex);
	return BufferEquals(hs, hs.Size());
}

void BufferTestObject::WriteToBuffer(const RSlice& data)
{	
	++(this->numWrites);
	for(size_t i = 0; i < data.Size(); ++i)
	{
		this->buffer.push_back(data[i]);
	}	
}

} //end namespace

