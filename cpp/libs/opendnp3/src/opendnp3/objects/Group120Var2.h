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

#ifndef OPENDNP3_GROUP120VAR2_H
#define OPENDNP3_GROUP120VAR2_H

#include <openpal/logging/Logger.h>
#include <openpal/container/ReadBufferView.h>
#include <openpal/container/WriteBufferView.h>

#include "opendnp3/app/GroupVariationID.h"

namespace opendnp3 {

struct Group120Var2
{
	Group120Var2();

	Group120Var2(
		uint32_t challengeSeqNum,
		uint16_t userNum,		
		const openpal::ReadBufferView& challengeData
	);

	static GroupVariationID ID() { return GroupVariationID(120,2); }

	uint32_t Size() const;
	
	uint32_t challengeSeqNum;
	uint16_t userNum;	
	openpal::ReadBufferView challengeData;
	
	static bool Read(const openpal::ReadBufferView& buffer, Group120Var2& output, openpal::Logger* pLogger);

	static bool Write(const Group120Var2& output, openpal::WriteBufferView& buffer);

	private:

	static const uint32_t MIN_SIZE = 10;	
};

}

#endif
