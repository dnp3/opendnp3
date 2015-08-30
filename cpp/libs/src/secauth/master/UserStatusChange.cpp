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

#include "secauth/master/UserStatusChange.h"

#include <opendnp3/objects/Group120.h>

#include "secauth/StringConversions.h"

namespace secauth
{


UserStatusChange::UserStatusChange(
    opendnp3::KeyChangeMethod keyChangeMethod_,
    opendnp3::UserOperation userOperation_,
    uint32_t statusChangeSeqNum_,
    uint16_t userRole_,
    uint16_t userRoleExpDays_,
    const std::string& userName_,
    openpal::RSlice userPublicKey_,
    openpal::RSlice certificationData_
) :
	keyChangeMethod(keyChangeMethod_),
	userOperation(userOperation_),
	statusChangeSeqNum(statusChangeSeqNum_),
	userRole(userRole_),
	userRoleExpDays(userRoleExpDays_),
	userName(userName_),
	userPublicKey(userPublicKey_),
	certificationData(certificationData_)
{}

opendnp3::Group120Var10 UserStatusChange::Convert() const
{
	return opendnp3::Group120Var10(
	           keyChangeMethod,
	           userOperation,
	           statusChangeSeqNum,
	           userRole,
	           userRoleExpDays,
	           AsSlice(userName),
	           userPublicKey.ToRSlice(),
	           certificationData.ToRSlice()
	       );
}


}



