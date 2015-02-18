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
#ifndef SECAUTHV5_IUPDATEKEYSTORE_H
#define SECAUTHV5_IUPDATEKEYSTORE_H

#include "secauthv5/User.h"
#include "UpdateKeyType.h"

#include <openpal/container/ReadBufferView.h>

namespace secauthv5
{

/** 
	An interface for retrieving update keys

	This interface may be given out to multiple outstation instances on multiple threads, 
	and therefore should be thread-safe (or immutable).
*/
class IUpdateKeyStore
{
	public:

		virtual bool GetUpdateKeyType(const User& user, UpdateKeyType& type) const = 0;
		
		virtual bool GetUpdateKey(const User& user, UpdateKeyType& type, openpal::ReadBufferView& key) const = 0;
};

}

#endif

