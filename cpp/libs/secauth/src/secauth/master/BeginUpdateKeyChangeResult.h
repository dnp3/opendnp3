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
#ifndef SECAUTH_BEGIN_UPDATE_KEY_CHANGE_RESULT_H
#define SECAUTH_BEGIN_UPDATE_KEY_CHANGE_RESULT_H

#include <opendnp3/gen/KeyChangeMethod.h>
#include <opendnp3/gen/TaskCompletion.h>
#include <opendnp3/app/User.h>
#include <openpal/container/ReadBufferView.h>

#include <string>

namespace secauth
{

/**
	The result of the first step in an update key change sequence 
*/
class BeginUpdateKeyChangeResult
{	

public:

	// failure constructor
	BeginUpdateKeyChangeResult(opendnp3::TaskCompletion result);
		
	// success constructor
	BeginUpdateKeyChangeResult(
		opendnp3::User user,
		uint32_t keyChangeSequenceNum,
		const openpal::ReadBufferView& masterChallengeData,
		const openpal::ReadBufferView& outstationChallengeData
	);
	
	/// The success or failure type of the overall operation
	/// the other fields are only valid if this value is 'SUCCESS'
	opendnp3::TaskCompletion result;

	/// The user # assigned by the outstation to the specified UTF-8 encoded name
	opendnp3::User user;
	/// The KSQ specified by the outstation
	uint32_t keyChangeSequenceNum;
	/// The challenge data that the master chose when it initiated the request
	openpal::ReadBufferView masterChallengeData;
	/// The challenge data that the outstation provided in its response
	openpal::ReadBufferView outstationChallengeData;
};

}

#endif

