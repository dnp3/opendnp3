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
#ifndef SECAUTH_FINISH_UPDATE_KEY_CHANGE_ARGS_H
#define SECAUTH_FINISH_UPDATE_KEY_CHANGE_ARGS_H

#include <opendnp3/gen/TaskCompletion.h>
#include <opendnp3/app/User.h>

#include <openpal/container/Buffer.h>

#include "secauth/UpdateKey.h"

#include <string>

namespace secauth
{

/**
	Arguments required for completing an update key change on the master
*/
class FinishUpdateKeyChangeArgs
{	

public:
	
	FinishUpdateKeyChangeArgs(
		const std::string& username,
		const std::string& outstationName,
		opendnp3::User user,
		uint32_t keyChangeSequenceNumber,
		const openpal::RSlice& masterChallengeData,
		const openpal::RSlice& outstationChallengeData,		
		const openpal::RSlice& encryptedKeyData,
		const UpdateKey& key
	);
				
	/// The UTF-8 username shared by the authority and outstation
	std::string username;

	/// Organizationally unique name of the outstation
	std::string outstationName;

	/// The user # assigned by the outstation associated w/ the UTF-8 encoded name
	opendnp3::User user;	

	/// The KSQ specified by the outstation
	uint32_t keyChangeSequenceNum;

	/// The challenge data that the master chose when it initiated the request
	openpal::Buffer masterChallengeData;

	/// The challenge data that the outstation provided in its response
	openpal::Buffer outstationChallengeData;

	/// The encrypted key data provided by the authority using the authority symmetric key
	openpal::Buffer encryptedKeyData;	

	/// the plaintext update key that was derived by the master and signed by the authority
	UpdateKey updateKey;
};

}

#endif

