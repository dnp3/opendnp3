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

#include "SecureBuffer.h"

namespace openpal
{

SecureBuffer::SecureBuffer() : pBuffer(nullptr), size(0)
{
	
}

SecureBuffer::SecureBuffer(uint32_t size_, uint8_t initialValue) :
	pBuffer(new uint8_t[size_]), 
	size(size_)
{
	this->GetWriteBufferView().SetAllTo(initialValue);
}

SecureBuffer::SecureBuffer(const SecureBuffer& other) :
	pBuffer(new uint8_t[other.Size()]),
	size(other.Size())
{
	auto dest = this->GetWriteBufferView();
	other.ToReadOnly().CopyTo(dest);
}

SecureBuffer& SecureBuffer::operator=(const SecureBuffer& other)
{
	
	if (this != &other)
	{
		delete[] pBuffer;

		auto view = other.ToReadOnly();
		this->pBuffer = new uint8_t[view.Size()];
		this->size = view.Size();

		auto dest = this->GetWriteBufferView();
		view.CopyTo(dest);
	}	

	return *this;
}

	
SecureBuffer::SecureBuffer(const ReadBufferView& input) : 
	pBuffer(new uint8_t[input.Size()]),
	size(input.Size())
{
	auto dest = this->GetWriteBufferView();
	input.CopyTo(dest);
}

SecureBuffer::~SecureBuffer()
{
	this->GetWriteBufferView().SetAllTo(0x00);
	delete[] pBuffer;
}

ReadBufferView SecureBuffer::ToReadOnly() const
{
	return ReadBufferView(pBuffer, size);
}

WriteBufferView SecureBuffer::GetWriteBufferView()
{
	return WriteBufferView(pBuffer, size);
}


}


