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
#ifndef SECAUTH_SIMPLEMASTERUSER_H
#define SECAUTH_SIMPLEMASTERUSER_H

#include "IMasterUser.h"

#include <openpal/container/Buffer.h>

#include <map>
#include <memory>

namespace secauth
{

/**
	A very simple update key store for the default user
*/
class SimpleMasterUser : public IMasterUser
{
	public:

		SimpleMasterUser() : updateKeyMode(opendnp3::UpdateKeyMode::AES128), buffer(32)
		{
			buffer.GetWriteBufferView().SetAllTo(0xFF);
			this->updateKey = buffer.ToReadOnly().Take(16);
		}

		virtual opendnp3::UpdateKeyMode GetUpdateKey(openpal::ReadBufferView& key) override final
		{
			key = updateKey;
			return updateKeyMode;
		}
		
		void SetUpdateKey(const openpal::ReadBufferView& key, opendnp3::UpdateKeyMode mode)
		{
			this->updateKeyMode = mode;
			auto dest = buffer.GetWriteBufferView();
			this->updateKey = key.CopyTo(dest);
		}

	private:

		opendnp3::UpdateKeyMode updateKeyMode;
		openpal::ReadBufferView updateKey;
		openpal::Buffer buffer;
};

}

#endif

