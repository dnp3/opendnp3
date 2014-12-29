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
#ifndef OPENDNP3_BINARYCOMMANDEVENT_H
#define OPENDNP3_BINARYCOMMANDEVENT_H

#include "opendnp3/gen/CommandStatus.h"

namespace opendnp3
{

/**
Maps to Group13Var1/2
*/
class BinaryCommandEvent
{
public:

	BinaryCommandEvent();

	BinaryCommandEvent(uint8_t flags);

	BinaryCommandEvent(uint8_t flags, uint64_t aTime);

	BinaryCommandEvent(bool aValue, CommandStatus aStatus);

	BinaryCommandEvent(bool aValue, CommandStatus aStatus, uint64_t aTime);

	bool value;
	CommandStatus status;
	uint64_t time;

	uint8_t GetFlags() const;

	bool operator==(const BinaryCommandEvent& arRHS) const;

private:

	static const uint8_t ValueMask = 0x80;
	static const uint8_t StatusMask = 0x7F;

	static bool GetValueFromFlags(uint8_t flags);
	static CommandStatus GetStatusFromFlags(uint8_t flags);
};

}

#endif
