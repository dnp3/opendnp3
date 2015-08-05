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
#ifndef SECAUTH_PENDING_USER_STATUS_CHANGE_H
#define SECAUTH_PENDING_USER_STATUS_CHANGE_H

#include "opendnp3/gen/KeyChangeMethod.h"
#include "opendnp3/gen/UserOperation.h"
#include "opendnp3/gen/UserRole.h"

#include <openpal/executor/MonotonicTimestamp.h>
#include <openpal/util/Uncopyable.h>

#include <string>
#include <map>

namespace secauth {


class ChangeData
{

public:

	ChangeData() :
		keyChangeMethod(opendnp3::KeyChangeMethod::UNDEFINED),
		userRole(opendnp3::UserRole::UNDEFINED),
		expiration(openpal::MonotonicTimestamp::Min())
	{}

	ChangeData(
		opendnp3::KeyChangeMethod keyChangeMethod,
		opendnp3::UserRole userRole,
		openpal::MonotonicTimestamp expiration
	);

	opendnp3::KeyChangeMethod keyChangeMethod;
	opendnp3::UserRole userRole;
	openpal::MonotonicTimestamp expiration;
};

class PendingUserStatusChanges : private openpal::Uncopyable
{

public:	

	

	void QueueChange(const std::string& userName, const ChangeData& data);

	bool PopChange(const std::string& userName, ChangeData& data);

private:

	typedef std::map<std::string, ChangeData> ChangeMap;

	ChangeMap changeMap;
	  
};


}

#endif
