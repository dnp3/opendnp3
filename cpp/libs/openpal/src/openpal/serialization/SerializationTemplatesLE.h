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
#ifndef OPENPAL_SERIALIZATIONTEMPLATESLE_H
#define OPENPAL_SERIALIZATIONTEMPLATESLE_H

#include <cstdint>
#include <cstring>

#include "openpal/container/ReadOnlyBuffer.h"
#include "openpal/container/WriteBuffer.h"

#include "openpal/util/Limits.h"

namespace openpal
{

template <class T>
class Bit16LE
{
public:

	static T Read(const uint8_t* apStart)
	{
		T ret = *(apStart);
		ret |= *(++apStart) << 8;
		return ret;
	}

	static void Write(uint8_t* apStart, T aValue)
	{
		*(apStart) = static_cast<uint8_t>(aValue & 0xFF);
		*(++apStart) = static_cast<uint8_t>((aValue >> 8) & 0xFF);
	}

	static void WriteBuffer(WriteBuffer& buffer, T aValue)
	{
		Write(buffer, aValue);
		buffer.Advance(Size);
	}

	inline static T ReadBuffer(ReadOnlyBuffer& arBuffer)
	{
		auto ret = Read(arBuffer);
		arBuffer.Advance(Size);
		return ret;
	}

	typedef T Type;

	const static size_t Size = sizeof(T);
	const static T Max;
	const static T Min;
};

template <class T>
const T Bit16LE<T>::Max = openpal::MaxValue<T>();

template <class T>
const T Bit16LE<T>::Min = openpal::MinValue<T>();

template <class T>
class Bit32LE
{
public:

	static T Read(const uint8_t* apStart)
	{
		T  ret = *(apStart);
		ret |= static_cast<T>(*(++apStart)) << 8;
		ret |= static_cast<T>(*(++apStart)) << 16;
		ret |= static_cast<T>(*(++apStart)) << 24;
		return ret;
	}
	static void Write(uint8_t* apStart, T aValue)
	{
		*(apStart) = static_cast<uint8_t>(aValue & 0xFF);
		*(++apStart) = static_cast<uint8_t>((aValue >> 8) & 0xFF);
		*(++apStart) = static_cast<uint8_t>((aValue >> 16) & 0xFF);
		*(++apStart) = static_cast<uint8_t>((aValue >> 24) & 0xFF);
	}

	static void WriteBuffer(WriteBuffer& buffer, T aValue)
	{
		Write(buffer, aValue);
		buffer.Advance(Size);
	}

	inline static T ReadBuffer(ReadOnlyBuffer& arBuffer)
	{
		auto ret = Read(arBuffer);
		arBuffer.Advance(Size);
		return ret;
	}

	typedef T Type;

	const static size_t Size = sizeof(T);
	const static T Max;
	const static T Min;
};

template <class T>
const T Bit32LE<T>::Max = openpal::MaxValue<T>();

template <class T>
const T Bit32LE<T>::Min = openpal::MinValue<T>();

}

#endif
