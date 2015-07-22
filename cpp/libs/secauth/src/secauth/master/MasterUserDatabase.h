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
#ifndef SECAUTH_MASTERUSERDATABASE_H
#define SECAUTH_MASTERUSERDATABASE_H

#include "IMasterUserDatabase.h"
#include "secauth/UpdateKey.h"

#include <openpal/container/Buffer.h>

#include <map>
#include <memory>

namespace secauth
{

/**
	A very simple update key store for the default user
*/
class MasterUserDatabase : public IMasterUserDatabase
{

public:

	virtual void EnumerateUsers(const std::function<void(const opendnp3::User)>& fun) const override final;

	virtual bool GetUpdateKey(const opendnp3::User& user, opendnp3::UpdateKeyMode& type, openpal::ReadBufferView& key) const override final;	

	virtual bool UserExists(const opendnp3::User& user) const override final;

	// copies the update key into the key store permanently
	// fails if the update key is invalid
	bool AddUser(const opendnp3::User& user, const UpdateKey& key);

private:

	std::map<uint16_t, std::unique_ptr<UpdateKey>> userMap;
		
};

}

#endif

