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
#ifndef SECAUTH_USERSTATUSCHANGE_H
#define SECAUTH_USERSTATUSCHANGE_H

#include <opendnp3/gen/KeyChangeMethod.h>
#include <opendnp3/gen/UserOperation.h>
#include <opendnp3/gen/UserRole.h>

#include <openpal/container/Buffer.h>

#include <string>

namespace opendnp3
{
struct Group120Var10;
}

namespace secauth
{

/**
	The object is part of the external master API. It used to initiate the UserStatusChange
	operation for both symmetric and assymetric keys.

	It gets mapped to g120v10 when the master sends it over the wire.
*/
class UserStatusChange
{

public:

	/**
		constructor for symmetric key update
		doesn't include the public key as there
		is no public key in this mode
	*/
	UserStatusChange(
	    opendnp3::KeyChangeMethod keyChangeMethod,
	    opendnp3::UserOperation userOperation,
	    uint32_t statusChangeSeqNum,
	    uint16_t userRole,
	    uint16_t userRoleExpDays,
	    const std::string& userName,
	    openpal::RSlice userPublicKey,
	    openpal::RSlice certificationData
	);

	opendnp3::Group120Var10 Convert() const;

	opendnp3::KeyChangeMethod keyChangeMethod;
	opendnp3::UserOperation userOperation;
	uint32_t statusChangeSeqNum;
	uint16_t userRole;
	uint16_t userRoleExpDays;
	std::string userName;
	openpal::Buffer userPublicKey;
	openpal::Buffer certificationData;

};

}

#endif

