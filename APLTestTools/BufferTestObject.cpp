//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one
// or more contributor license agreements. See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  Green Enery Corp licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
//
#include "BufferTestObject.h"

#include "BufferHelpers.h"

#include <memory>
#include <iostream>

#include <APL/Exception.h>
#include <APL/Location.h>

namespace apl
{

BufferTestObject::BufferTestObject() :
	mNumWrites(0)
{

}

BufferTestObject::~BufferTestObject()
{

}

void BufferTestObject::ClearBuffer()
{
	mNumWrites = 0;
	mBuffer.clear();
}

bool BufferTestObject::BufferEquals(const uint8_t* apData, size_t aNumBytes)
{

	if(aNumBytes != mBuffer.size()) return false;
	for(size_t i = 0; i < aNumBytes; i++)
		if(apData[i] != mBuffer[i]) {
			return false;
		}
	return true;
}

bool BufferTestObject::BufferContains(const std::string& arPattern)
{
	std::string s;
	for(size_t i = 0; i < mBuffer.size(); ++i) {
		std::string c(1, static_cast<char>(mBuffer[i]));
		s.append(c);
	}
	return s.find(arPattern) != std::string::npos;
}

bool BufferTestObject::BufferEqualsHex(const std::string& arData)
{
	HexSequence hs(arData);
	return BufferEquals(hs.Buffer(), hs.Size());
}

bool BufferTestObject::BufferEqualsString(const std::string& arData)
{
	if(arData.size() != mBuffer.size()) return false;
	for(size_t i = 0; i < mBuffer.size(); i++)
		if(arData[i] != mBuffer[i]) {
			return false;
		}
	return true;
}

void BufferTestObject::WriteToBuffer(const uint8_t* apData, size_t aNumBytes)
{
	if( (mBuffer.size() + aNumBytes) > MAX_SIZE ) throw Exception(LOCATION, "Max size exceeded");
	++mNumWrites;
	for(size_t i = 0; i < aNumBytes; ++i) mBuffer.push_back(apData[i]);
}

} //end namespace

