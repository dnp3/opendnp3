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

#ifndef OPENDNP3_GROUP120VAR5_H
#define OPENDNP3_GROUP120VAR5_H

#include <openpal/container/ReadBufferView.h>
#include <openpal/container/WriteBufferView.h>

#include "opendnp3/gen/HMACType.h"
#include "opendnp3/gen/KeyStatus.h"
#include "opendnp3/gen/ChallengeReason.h"
#include "opendnp3/gen/KeyWrapAlgorithm.h"
#include "opendnp3/app/GroupVariationID.h"


namespace opendnp3 {

struct Group120Var5
{
	Group120Var5();

	Group120Var5(
		uint32_t keyChangeSeqNum,
		uint16_t userNum,
		KeyWrapAlgorithm keywrapAlgorithm,
		KeyStatus  keyStatus,
		HMACType hmacType,
		openpal::ReadBufferView challengeData,
		openpal::ReadBufferView hmacValue
	);

	static GroupVariationID ID() { return GroupVariationID(120,5); }

	uint32_t Size() const;
	
	uint32_t keyChangeSeqNum;
	uint16_t userNum;
	KeyWrapAlgorithm keywrapAlgorithm;
	KeyStatus  keyStatus;	
	HMACType hmacType;
	openpal::ReadBufferView challengeData;
	openpal::ReadBufferView hmacValue;
	
	static bool Read(const openpal::ReadBufferView& buffer, Group120Var5& output);
	static bool Write(const Group120Var5& output, openpal::WriteBufferView& buffer);

	// The size of all the fixed size fields in the serialized object	
	static const uint32_t FIXED_BASE_SIZE = 11;		
};

}

#endif
