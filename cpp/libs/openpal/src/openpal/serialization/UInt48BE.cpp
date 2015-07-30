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

#include "UInt48BE.h"

namespace openpal
{

	UInt48Type UInt48BE::Read(const uint8_t* pStart)
	{
		uint64_t ret = 0;

		ret |= static_cast<int64_t>(*(pStart)) << 40;
		ret |= static_cast<int64_t>(*(++pStart)) << 32;
		ret |= static_cast<int64_t>(*(++pStart)) << 24;
		ret |= static_cast<int64_t>(*(++pStart)) << 16;
		ret |= static_cast<int64_t>(*(++pStart)) << 8;
		ret |= static_cast<int64_t>(*(++pStart));

		return UInt48Type(ret);
	}

	void UInt48BE::Write(uint8_t* pStart, UInt48Type value)
	{
		if (value > MAX)
		{
			value = UInt48Type(MAX);
		}

		*(pStart) = static_cast<uint8_t>((value >> 40) & 0xFF);
		*(++pStart) = static_cast<uint8_t>((value >> 32) & 0xFF);
		*(++pStart) = static_cast<uint8_t>((value >> 24) & 0xFF);
		*(++pStart) = static_cast<uint8_t>((value >> 16) & 0xFF);
		*(++pStart) = static_cast<uint8_t>((value >> 8) & 0xFF);
		*(++pStart) = static_cast<uint8_t>(value & 0xFF);									
	}

}