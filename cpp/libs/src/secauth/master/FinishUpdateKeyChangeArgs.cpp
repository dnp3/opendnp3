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
#include "secauth/master/FinishUpdateKeyChangeArgs.h"

namespace secauth
{

FinishUpdateKeyChangeArgs::FinishUpdateKeyChangeArgs(
    const std::string& username_,
    const std::string& outstationName_,
    opendnp3::User user_,
    uint32_t keyChangeSequenceNumber_,
    const openpal::RSlice& masterChallengeData_,
    const openpal::RSlice& outstationChallengeData_,
    const openpal::RSlice& encryptedKeyData_,
    const UpdateKey& updateKey_
) :
	username(username_),
	outstationName(outstationName_),
	user(user_),
	keyChangeSequenceNum(keyChangeSequenceNumber_),
	masterChallengeData(masterChallengeData_),
	outstationChallengeData(outstationChallengeData_),
	encryptedKeyData(encryptedKeyData_),
	updateKey(updateKey_)
{

}


}



