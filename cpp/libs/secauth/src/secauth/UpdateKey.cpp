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

#include "UpdateKey.h"

using namespace opendnp3;

namespace secauth
{

UpdateKey::UpdateKey() :
	isValid(false),
	updateKeyMode(UpdateKeyMode::AES128),			
	buffer(0xFF)	
{
	
}

UpdateKey::UpdateKey(const openpal::ReadBufferView& key) : UpdateKey()
{
	this->Initialize(key);
}

bool UpdateKey::IsValid() const
{
	return isValid;
}

openpal::ReadBufferView UpdateKey::GetKeyView() const
{
	const auto SIZE = (updateKeyMode == UpdateKeyMode::AES128) ? UPDATE_KEY_SIZE_128 : UPDATE_KEY_SIZE_256;	
	return buffer.ToReadOnly().Take(SIZE);	
}

opendnp3::UpdateKeyMode UpdateKey::GetKeyMode() const
{
	return updateKeyMode;
}
				
bool UpdateKey::Initialize(const openpal::ReadBufferView& key)
{
	switch (key.Size())
	{
	case(UPDATE_KEY_SIZE_128) :
		this->Initialize(key, opendnp3::UpdateKeyMode::AES128);
		return true;
	case(UPDATE_KEY_SIZE_256):
		this->Initialize(key, opendnp3::UpdateKeyMode::AES256);
		return true;
	default:		
		return false;
	}
}

void UpdateKey::Initialize(const openpal::ReadBufferView& key, opendnp3::UpdateKeyMode mode)
{
  this->isValid = true;
  this->updateKeyMode = mode;
  auto dest = buffer.GetWriteBuffer();
  key.CopyTo(dest);  
}

}



