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

#include "openpal/container/ReadBufferView.h"
#include "openpal/container/WriteBufferView.h"

#include "openpal/util/Limits.h"
#include "openpal/util/Uncopyable.h"

namespace openpal
{

template <class T>
class Float : private PureStatic
{
public:
	typedef T Type;

	const static size_t SIZE = sizeof(T);
	const static T Max;
	const static T Min;

	inline static T ReadBuffer(ReadBufferView& buffer)
	{
		auto ret = Read(buffer);
		buffer.Advance(SIZE);
		return ret;
	}

	inline static void WriteBuffer(WriteBufferView& buffer, T value)
	{
		Write(buffer, value);
		buffer.Advance(SIZE);
	}

	// Some platforms like ARM have WORD alignment issue when using reinterpret cast.
	// The float/double read routines use intermediate buffer that the compiler word aligns
	inline static T Read(const uint8_t* pStart)
	{
		T d;
		memcpy(&d, pStart, SIZE);
		return d;
	}

	inline static void Write(uint8_t* pStart, T value)
	{
		memcpy(pStart, &value, SIZE);
	}
};

template <class T>
const T Float<T>::Max = openpal::MaxValue<T>();

template <class T>
const T Float<T>::Min = openpal::MinValue<T>();

}

#endif
