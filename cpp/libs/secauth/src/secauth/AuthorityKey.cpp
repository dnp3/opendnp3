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

#include "AuthorityKey.h"

namespace secauth
{
	uint32_t AuthorityKey::NumBytes(KeySize size)
	{
		return (size == KeySize::S128) ? MIN_KEY_SIZE_128 : MAX_KEY_SIZE_256;
	}

	bool AuthorityKey::IsValidKeySize(uint32_t count)
	{
		switch (count)
		{
		case(MIN_KEY_SIZE_128) :			
			return true;
		case(MAX_KEY_SIZE_256) :			
			return true;
		default:
			return false;
		}
	}
		
	AuthorityKey::AuthorityKey() : m_size(0)
	{}
	
	AuthorityKey::AuthorityKey(uint8_t repeat, KeySize size) : m_size(NumBytes(size))
	{
		this->m_buffer.GetWriteBuffer(m_size).SetAllTo(repeat);
	}
	
	AuthorityKey::AuthorityKey(const openpal::ReadBufferView& key) : m_size(0)
	{
		this->Initialize(key);
	}

	openpal::ReadBufferView AuthorityKey::GetKeyView() const
	{
		return m_buffer.ToReadOnly(m_size);
	}

	bool AuthorityKey::IsValid() const
	{		
		return IsValidKeySize(m_size);
	}

	bool AuthorityKey::Initialize(const openpal::ReadBufferView& key)
	{		
		if (!IsValidKeySize(key.Size()))
		{
			return false;
		}

		this->m_size = key.Size();
		key.CopyTo(this->m_buffer.GetWriteBuffer());
		return true;
	}



}



