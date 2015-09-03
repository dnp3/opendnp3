/*
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
#include "secauth/UpdateKey.h"

using namespace opendnp3;

namespace secauth
{

UpdateKey::UpdateKey() : m_algorithm (KeyWrapAlgorithm::UNDEFINED), m_buffer(0xFF)
{

}

UpdateKey::UpdateKey(uint8_t repeat, KeyWrapAlgorithm algorithm) : m_algorithm(algorithm), m_buffer(repeat)
{
}

UpdateKey::UpdateKey(const openpal::RSlice& key) : UpdateKey()
{
	this->Initialize(key);
}

UpdateKey::View UpdateKey::GetView() const
{
	return View(m_algorithm, m_buffer.ToRSlice(GetSize(m_algorithm)));
}

uint32_t UpdateKey::GetSize(opendnp3::KeyWrapAlgorithm algorithm)
{
	switch (algorithm)
	{
	case(KeyWrapAlgorithm::AES_128) :
		return UPDATE_KEY_SIZE_128;
	case(KeyWrapAlgorithm::AES_256) :
		return UPDATE_KEY_SIZE_256;
	default:
		return 0;
	}
}

opendnp3::KeyWrapAlgorithm UpdateKey::GetKeyWrapAlgorithm(uint32_t size)
{
	switch (size)
	{
	case(UPDATE_KEY_SIZE_128) :
		return KeyWrapAlgorithm::AES_128;
	case(UPDATE_KEY_SIZE_256) :
		return KeyWrapAlgorithm::AES_256;
	default:
		return KeyWrapAlgorithm::UNDEFINED;
	}
}


bool UpdateKey::Initialize(const openpal::RSlice& key)
{
	switch (key.Size())
	{
	case(UPDATE_KEY_SIZE_128) :
	case(UPDATE_KEY_SIZE_256) :
		this->Copy(key);
		return true;
	default:
		return false;
	}
}

void UpdateKey::Copy(const openpal::RSlice& key)
{
	auto dest = m_buffer.GetWSlice();
	key.CopyTo(dest);
	this->m_algorithm = GetKeyWrapAlgorithm(key.Size());
}

}



