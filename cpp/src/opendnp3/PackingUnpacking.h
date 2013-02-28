
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

#include <opendnp3/Types.h>
#include <opendnp3/Visibility.h>

#include <stddef.h>
#include <limits>
#include <string.h>

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

namespace opendnp3
{
class DLL_LOCAL UInt8
{
public:
	static uint8_t Read(const uint8_t* apStart) {
		return (*apStart);
	}
	static void Write(uint8_t* apStart, uint8_t aValue) {
		*(apStart) = aValue;
	}

	const static size_t Size = 1;
	const static uint8_t Max;
	const static uint8_t Min;

	typedef uint8_t Type;
};

template <class T>
class DLL_LOCAL BytesBE
{
public:
	static T Read( const uint8_t* apStart ) {
		T ret = 0;
		for ( size_t i = 0; i < Size; i++ ) {
			ret |= static_cast<T>(apStart[i]) << (8 * (Size - 1 - i));
		}
		return ret;
	}
	static void Write( uint8_t* apStart, T aValue ) {
		for ( size_t i = 0; i < Size; i++ ) {
			apStart[i] = static_cast<uint8_t>((aValue >> 8 * (Size - 1 - i)) & 0xFF);
		}
	}

	typedef T Type;
	const static size_t Size = sizeof(T);
	const static T Max;
	const static T Min;
};

template <class T>
const T BytesBE<T>::Max = std::numeric_limits<T>::max();

template <class T>
const T BytesBE<T>::Min = std::numeric_limits<T>::min();

template <class T>
class DLL_LOCAL Bit16LE
{
public:
	static T Read(const uint8_t* apStart) {
		T ret = *(apStart);
		ret |= *(++apStart) << 8;
		return ret;
	}

	static void Write(uint8_t* apStart, T aValue) {
		*(apStart) = static_cast<uint8_t>(aValue & 0xFF);
		*(++apStart) = static_cast<uint8_t>((aValue >> 8) & 0xFF);
	}

	typedef T Type;

	const static size_t Size = 2;
	const static T Max;
	const static T Min;
};

template <class T>
const T Bit16LE<T>::Max = std::numeric_limits<T>::max();

template <class T>
const T Bit16LE<T>::Min = std::numeric_limits<T>::min();

template <class T>
class DLL_LOCAL Bit32LE
{
public:
	static T Read(const uint8_t* apStart) {
		T  ret = *(apStart);
		ret |= *(++apStart) << 8;
		ret |= *(++apStart) << 16;
		ret |= *(++apStart) << 24;
		return ret;
	}
	static void Write(uint8_t* apStart, T aValue) {
		*(apStart) = static_cast<uint8_t>(aValue & 0xFF);
		*(++apStart) = static_cast<uint8_t>((aValue >> 8) & 0xFF);
		*(++apStart) = static_cast<uint8_t>((aValue >> 16) & 0xFF);
		*(++apStart) = static_cast<uint8_t>((aValue >> 24) & 0xFF);
	}

	typedef T Type;

	const static size_t Size = 4;
	const static T Max;
	const static T Min;
};

template <class T>
const T Bit32LE<T>::Max = std::numeric_limits<T>::max();

template <class T>
const T Bit32LE<T>::Min = std::numeric_limits<T>::min();



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

class DLL_LOCAL UInt48LE
{
public:
	static int64_t Read(const uint8_t* apStart) {

		int64_t  ret = *(apStart);
		ret |= static_cast<int64_t>(*(++apStart)) << 8;
		ret |= static_cast<int64_t>(*(++apStart)) << 16;
		ret |= static_cast<int64_t>(*(++apStart)) << 24;
		ret |= static_cast<int64_t>(*(++apStart)) << 32;
		ret |= static_cast<int64_t>(*(++apStart)) << 40;

		return ret;
	}

	static void Write(uint8_t* apStart, int64_t aValue) {
		if(aValue > MAX) aValue = MAX;

		*(apStart) = static_cast<uint8_t>(aValue & 0xFF);
		*(++apStart) = static_cast<uint8_t>((aValue >> 8) & 0xFF);
		*(++apStart) = static_cast<uint8_t>((aValue >> 16) & 0xFF);
		*(++apStart) = static_cast<uint8_t>((aValue >> 24) & 0xFF);
		*(++apStart) = static_cast<uint8_t>((aValue >> 32) & 0xFF);
		*(++apStart) = static_cast<uint8_t>((aValue >> 40) & 0xFF);
	}

	const static int64_t MAX = 281474976710655ULL; // 2^48 -1

	const static size_t Size = 6;

	typedef int64_t Type;
};

template <class T>
class DLL_LOCAL Float
{
public:
	typedef T Type;

	const static size_t Size = sizeof(T);
	const static T Max;
	const static T Min;

protected:

	static T SafeRead(const uint8_t* apStart) {		
		T d;
		memcpy(&d, apStart, sizeof(T));
		return d;
	}


	static void SafeWrite(uint8_t* apStart, T aValue) {
		memcpy(apStart, &aValue, sizeof(T));
	}
};

template <class T>
const T Float<T>::Max = std::numeric_limits<T>::max();

template <class T>
const T Float<T>::Min = -std::numeric_limits<T>::max();

class DLL_LOCAL SingleFloat : public Float<float>
{
public:
	static float Read(const uint8_t* apStart);

	static void Write(uint8_t* apStart, float aValue);
};

class DLL_LOCAL DoubleFloat : public Float<double>
{
public:
	static double Read(const uint8_t* apStart);
	static void Write(uint8_t* apStart, double aValue);

private:

#ifdef OPENDNP3_ARM_FLOAT_WORKAROUND
	static double FlipWord32(double aValue);
#endif
};
}

#endif
