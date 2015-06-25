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
#ifndef OPENPAL_STATICBUFFER_H
#define OPENPAL_STATICBUFFER_H

#include "openpal/container/WriteBufferView.h"
#include "openpal/container/ReadBufferView.h"
#include "openpal/util/Comparisons.h"

#include <cstdint>

namespace openpal
{

template <uint32_t SIZE>
class StaticBuffer
{

public:	

	StaticBuffer()
	{}	

	virtual ~StaticBuffer() {}

	ReadBufferView ToReadOnly() const
	{
		return ReadBufferView(buffer, SIZE);
	}

	ReadBufferView ToReadOnly(uint32_t maxSize) const
	{
		return ReadBufferView(buffer, openpal::Min(SIZE, maxSize));
	}

	WriteBufferView GetWriteBuffer()
	{
		return WriteBufferView(buffer, SIZE);
	}

	WriteBufferView GetWriteBuffer(uint32_t maxSize)
	{
		return WriteBufferView(buffer, openpal::Min(SIZE, maxSize));
	}

	const uint8_t* operator()() const { return buffer; }

	uint8_t* operator()() { return buffer; }

	uint32_t Size() const { return SIZE; }

private:
	uint8_t buffer[SIZE];
};

}

#endif
