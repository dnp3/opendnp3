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
#ifndef __BUFFER_HELPERS_H_
#define __BUFFER_HELPERS_H_

#include <APL/CopyableBuffer.h>
#include <string>

namespace apl
{


class ByteStr : public CopyableBuffer
{

public:
	ByteStr(size_t aLength);
	ByteStr(size_t aLength, uint8_t aSeed);
	ByteStr(const uint8_t* apData, size_t aLength);
	ByteStr(const std::string& aChars);
	bool operator==(const ByteStr& arRHS) const;
};

/**
 * A sequence of hex values in the form "01 02 03 04" that
 * are stored as a ByteStr.
 *
 */
class HexSequence : public ByteStr
{
public:
	HexSequence(const std::string& aSequence);

private:
	std::string RemoveSpaces(const std::string& aSequence);
	void RemoveSpacesInPlace(std::string& aSequence);
	static size_t Validate(const std::string& aSequence);
};


}

#endif
