
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
#ifndef __PACKING_UNPACKING_H_
#define __PACKING_UNPACKING_H_

#include "SerializationTemplates.h"

namespace opendnp3
{

class UInt8
{
public:

	inline static uint8_t Read(const uint8_t* apStart) 
	{
		return (*apStart);
	}

	inline static void Write(uint8_t* apStart, uint8_t aValue) 
	{
		*(apStart) = aValue;
	}

	const static size_t Size = 1;
	const static uint8_t Max;
	const static uint8_t Min;

	typedef uint8_t Type;
};

typedef Bit16LE<int16_t> Int16LE;
typedef Bit16LE<uint16_t> UInt16LE;
typedef Bit32LE<int32_t> Int32LE;
typedef Bit32LE<uint32_t> UInt32LE;

typedef BytesBE<uint16_t> UInt16BE;
typedef BytesBE<int16_t> Int16BE;
typedef BytesBE<uint32_t> UInt32BE;
typedef BytesBE<int32_t> Int32BE;

typedef BytesBE<uint64_t> UInt64BE;
typedef BytesBE<int64_t> Int64BE;

class UInt48LE
{
public:

	static int64_t Read(const uint8_t* apStart);	
	static void Write(uint8_t* apStart, int64_t aValue);

	const static int64_t MAX = 281474976710655ULL; // 2^48 -1
	const static size_t Size = 6;
	typedef int64_t Type;
};

class SingleFloat : public Float<float>
{
public:
	static float Read(const uint8_t* apStart);
	static void Write(uint8_t* apStart, float aValue);
};

class DoubleFloat : public Float<double>
{
public:
	static double Read(const uint8_t* apStart);
	static void Write(uint8_t* apStart, double aValue);
};

}

#endif
