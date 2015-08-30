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
#ifndef SECAUTH_ROLE_BASED_PERMISSIONS_H
#define SECAUTH_ROLE_BASED_PERMISSIONS_H

#include "secauth/outstation/Permissions.h"

#include <opendnp3/gen/UserRole.h>

#include <openpal/util/Uncopyable.h>

namespace secauth
{

/**
*	Creates permission sets based on roles
*
*	This is a VERY crude mapping to DNP3 function codes, but hey, the SA
*	definitions of these roles leaving a lot to the imagination.
*
*   Currently only the VIEWER, OPERATOR, and SINGLE_USER roles have permissions mapped
*
*/
class RoleBasedPermissions : openpal::StaticOnly
{
public:

	static Permissions From(opendnp3::UserRole role);

private:

	static const Permissions OPERATE_CONTROLS;
	static const Permissions MONITOR_DATA;
};

}

#endif

