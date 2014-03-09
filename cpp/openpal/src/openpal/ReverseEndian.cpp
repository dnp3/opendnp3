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
#include <openpal/ReverseEndian.h>

#include <cstring>

namespace openpal
{

const uint8_t UInt8Simple::Max = std::numeric_limits<uint8_t>::max();
const uint8_t UInt8Simple::Min = std::numeric_limits<uint8_t>::min();

int64_t UInt48LE::Read(const uint8_t* apStart)
{

	int64_t  ret = *(apStart);
	ret |= static_cast<int64_t>(*(++apStart)) << 8;
	ret |= static_cast<int64_t>(*(++apStart)) << 16;
	ret |= static_cast<int64_t>(*(++apStart)) << 24;
	ret |= static_cast<int64_t>(*(++apStart)) << 32;
	ret |= static_cast<int64_t>(*(++apStart)) << 40;

	return ret;
}

void UInt48LE::Write(uint8_t* apStart, int64_t aValue)
{
	if(aValue > MAX) aValue = MAX;

	*(apStart) = static_cast<uint8_t>(aValue & 0xFF);
	*(++apStart) = static_cast<uint8_t>((aValue >> 8) & 0xFF);
	*(++apStart) = static_cast<uint8_t>((aValue >> 16) & 0xFF);
	*(++apStart) = static_cast<uint8_t>((aValue >> 24) & 0xFF);
	*(++apStart) = static_cast<uint8_t>((aValue >> 32) & 0xFF);
	*(++apStart) = static_cast<uint8_t>((aValue >> 40) & 0xFF);
}

}
