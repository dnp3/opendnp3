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
#include "ReadBufferView.h"

#include "openpal/util/Comparisons.h"

#include "WriteBufferView.h"

#include <cstring>

namespace openpal
{

ReadBufferView ReadBufferView::Empty()
{
	return ReadBufferView();
}

ReadBufferView::ReadBufferView(): HasSize(0), pBuffer(nullptr)
{}

ReadBufferView::ReadBufferView(uint8_t const* pBuffer, uint32_t size) :
	HasSize(size),
	pBuffer(pBuffer)
{}

ReadBufferView ReadBufferView::CopyTo(WriteBufferView& dest) const
{
	if (dest.Size() < size)
	{
		return ReadBufferView::Empty();
	}
	else
	{
		memcpy(dest, pBuffer, size);
		return ReadBufferView(dest, size);
	}
}

ReadBufferView ReadBufferView::Take(uint32_t count) const
{	
	return ReadBufferView(pBuffer, openpal::Min(size, count));
}

ReadBufferView ReadBufferView::Skip(uint32_t count) const
{
	auto num = openpal::Min(size, count);
	return ReadBufferView(pBuffer + num, size - num);
}

void ReadBufferView::Clear()
{
	pBuffer = nullptr;
	size = 0;
}

bool ReadBufferView::Equals(const ReadBufferView& rhs) const
{
	if (this->Size() == rhs.Size())
	{
		return memcmp(pBuffer, rhs.pBuffer, Size()) == 0;
	}
	else
	{
		return false;
	}
}

void ReadBufferView::Advance(uint32_t count)
{
	auto num = openpal::Min(size, count);
	pBuffer += num;
	size -= num;
}

}
