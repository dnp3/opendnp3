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
#ifndef __BUFFER_TEST_OBJECT_H_
#define __BUFFER_TEST_OBJECT_H_

#include <string>
#include <vector>
#include <cstdint>

#include <openpal/container/RSlice.h>

namespace testlib
{

class BufferTestObject
{	
public:

	BufferTestObject() = default;	

	bool BufferEquals(const openpal::RSlice& data) const;
	bool BufferEquals(const uint8_t*, size_t) const;
	bool BufferEqualsHex(const std::string& hex) const;	
	bool BufferContains(const std::string& text) const;

	std::string GetBufferAsHexString(bool spaced = true) const;

	bool IsBufferEmpty() const
	{
		return buffer.size() == 0;
	}
	
	void ClearBuffer();
	
	size_t Size() const
	{
		return buffer.size();
	}

	bool SizeEquals(size_t num)
	{
		return num == Size();
	}

	size_t NumWrites() const
	{
		return numWrites;
	}

protected:
	void WriteToBuffer(const openpal::RSlice& data);

private:
	size_t numWrites = 0;
	std::vector<uint8_t> buffer;
};

}

#endif

