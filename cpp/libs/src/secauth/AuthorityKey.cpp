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

#include "secauth/AuthorityKey.h"

namespace secauth
{
AuthorityKey::AuthorityKey() : m_valid(false)
{}

AuthorityKey::AuthorityKey(uint8_t repeat) : m_valid(true)
{
	this->m_buffer.GetWSlice().SetAllTo(repeat);
}

AuthorityKey::AuthorityKey(const openpal::RSlice& key) : m_valid(false)
{
	this->Initialize(key);
}

openpal::RSlice AuthorityKey::GetKeyView() const
{
	return m_buffer.ToRSlice();
}

bool AuthorityKey::IsValid() const
{
	return m_valid;
}

bool AuthorityKey::Initialize(const openpal::RSlice& key)
{
	if (!(key.Size() == KEY_SIZE_256))
	{
		return false;
	}

	auto dest = this->m_buffer.GetWSlice();
	key.CopyTo(dest);
	return true;
}



}



