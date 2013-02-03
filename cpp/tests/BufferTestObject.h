
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the GNU Affero General Public License Version 3.0
// (the "Additional License"). You may not use these modifications except in
// compliance with the additional license. You may obtain a copy of the Additional
// License at
//
// http://www.gnu.org/licenses/agpl.html
//
// Contact Automatak, LLC for a commercial license to these modifications
//
#ifndef __BUFFER_TEST_OBJECT_H_
#define __BUFFER_TEST_OBJECT_H_

#include <opendnp3/Types.h>
#include <string>
#include <vector>

namespace opendnp3
{
class ByteStr;

class BufferTestObject
{
	static const size_t MAX_SIZE = 1024 * 1024;

public:
	BufferTestObject();
	~BufferTestObject();

	bool BufferEquals(const uint8_t*, size_t);
	bool BufferEqualsHex(const std::string& arData);
	bool BufferEqualsString(const std::string& arData);
	bool BufferContains(const std::string& arData);
	bool IsBufferEmpty() {
		return mBuffer.size() == 0;
	}
	void ClearBuffer();
	size_t Size() {
		return mBuffer.size();
	}
	bool SizeEquals(size_t aNum) {
		return aNum == Size();
	}
	size_t NumWrites() {
		return mNumWrites;
	}

protected:
	void WriteToBuffer(const uint8_t*, size_t);

private:
	size_t mNumWrites;
	std::vector<uint8_t> mBuffer;
};

}

#endif

