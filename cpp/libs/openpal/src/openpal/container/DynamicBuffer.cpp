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

#include "DynamicBuffer.h"

namespace openpal
{
	DynamicBuffer::DynamicBuffer(uint32_t size) : DynamicArray<uint8_t, uint32_t>(size)
	{}

	DynamicBuffer::DynamicBuffer(const ReadBufferView& input) : DynamicArray<uint8_t, uint32_t>(input.Size())
	{
		input.CopyTo(this->GetWriteBufferView());
	}

	ReadBufferView DynamicBuffer::ToReadOnly() const
	{
		return ReadBufferView(this->buffer, this->size);
	}

	WriteBufferView DynamicBuffer::GetWriteBufferView()
	{
		return WriteBufferView(this->buffer, this->Size());
	}

	WriteBufferView DynamicBuffer::GetWriteBufferView(uint32_t maxSize)
	{		
		if (maxSize <= this->Size())
		{
			return WriteBufferView(this->buffer, maxSize);
		}
		else
		{
			return GetWriteBufferView();
		}
	}
}

