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
#ifndef SECAUTH_IOUTSTATIONAPPLICATIONSA_H
#define SECAUTH_IOUTSTATIONAPPLICATIONSA_H

#include <opendnp3/outstation/IOutstationApplication.h>
#include <openpal/executor/IUTCTimeSource.h>

#include "secauth/outstation/OutstationUserInfo.h"

#include <string>

namespace secauth
{

/**
	Extends the outstation application interface for vanilla DNP3 with additional methods required for secure authentication.

	A UTC time source is required so that the outstation can timestamp authentication error objects internally, whereas in vanilla DNP3
	the application code sets the timestamps on all measurement values before loading.
*/
class IOutstationApplicationSA : public opendnp3::IOutstationApplication, public openpal::IUTCTimeSource
{
public:

	/**
	*	Called when a fully authenticated user status change message is received w/ a new SCSN
	*
	*	Application code is expected to persist the increment SCSN to non-volatile storage to be restored on reboot
	*/
	virtual void OnNewSCSN(uint32_t userStatusSeqNum) {}

	/**
	*	Callback received when an authenticated user status change is received with a DELETE for an existing user
	*
	*	The user's update keys are automatically deleted and existing sessions are invalidated.
	*/
	virtual void OnDeleteUser(const std::string& userName, const opendnp3::User& user) {}

	/**
	*	Called when a fully authenticated Update Key Change is processed
	*
	*	Application code is expected to persist this data to secure non-volatile memory so that it can be loaded on the next reboot.
	*/
	virtual void AddOrUpdateUser(const OutstationUserInfo& info) {}

};

}

#endif

