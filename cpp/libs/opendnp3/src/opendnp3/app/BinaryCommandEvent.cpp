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

#include "BinaryCommandEvent.h"

namespace opendnp3
{

BinaryCommandEvent::BinaryCommandEvent() :
	value(false),
	status(CommandStatus::SUCCESS),
	time(0)
{}

BinaryCommandEvent::BinaryCommandEvent(uint8_t aValue) :
	value(GetValueFromFlags(aValue)),
	status(GetStatusFromFlags(aValue)),
	time(0)
{}

BinaryCommandEvent::BinaryCommandEvent(uint8_t aValue, uint64_t aTime) :
	value(GetValueFromFlags(aValue)),
	status(GetStatusFromFlags(aValue)),
	time(aTime)
{}

BinaryCommandEvent::BinaryCommandEvent(bool aValue, CommandStatus aStatus) :
	value(aValue),
	status(aStatus),
	time(0)
{}

BinaryCommandEvent::BinaryCommandEvent(bool aValue, CommandStatus aStatus, uint64_t aTime) :
	value(aValue),
	status(aStatus),
	time(aTime)
{}

uint8_t BinaryCommandEvent::GetFlags() const
{
	return (static_cast<uint8_t>(value) << 7) | (CommandStatusToType(status));
}

bool BinaryCommandEvent::operator==(const BinaryCommandEvent& arRHS) const
{
	return value == arRHS.value && status == arRHS.status && time == arRHS.time;
}

bool BinaryCommandEvent::GetValueFromFlags(uint8_t flags)
{
	return (flags & ValueMask) == ValueMask;
}

CommandStatus BinaryCommandEvent::GetStatusFromFlags(uint8_t flags)
{
	return CommandStatusFromType(flags & StatusMask);
}

}
