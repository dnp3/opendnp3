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
#ifndef OPENPAL_FLOATSERIALIZATIONTEMPLATES_H
#define OPENPAL_FLOATSERIALIZATIONTEMPLATES_H

#include <cstdint>
#include <cstring>

#include "openpal/container/ReadOnlyBuffer.h"
#include "openpal/container/WriteBuffer.h"

#include "openpal/util/Limits.h"

namespace openpal
{

template <class T>
class Float
{
public:
	typedef T Type;

	const static size_t Size = sizeof(T);
	const static T Max;
	const static T Min;

	inline static T ReadBuffer(ReadOnlyBuffer& arBuffer)
	{
		auto ret = Read(arBuffer);
		arBuffer.Advance(Size);
		return ret;
	}

	static void WriteBuffer(WriteBuffer& buffer, T aValue)
	{
		Write(buffer, aValue);
		buffer.Advance(Size);
	}

	// Some platforms like ARM have WORD alignment issue when using reinterpret cast.
	// The float/double read routines use intermediate buffer that the compiler word aligns
	inline static T Read(const uint8_t* apStart)
	{
		T d;
		memcpy(&d, apStart, Size);
		return d;
	}

	inline static void Write(uint8_t* apStart, T aValue)
	{
		memcpy(apStart, &aValue, Size);
	}
};

template <class T>
const T Float<T>::Max = openpal::MaxValue<T>();

template <class T>
const T Float<T>::Min = openpal::MinValue<T>();

}

#endif
