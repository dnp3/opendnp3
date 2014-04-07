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

#include "EventCount.h"

namespace opendnp3
{

uint32_t ClassCount::IndexOf(EventType type)
{
	switch (type)
	{
	case(EventType::Binary) :
		return 0;
	case(EventType::DoubleBitBinary) :
		return 1;
	case(EventType::Analog) :
		return 2;
	case(EventType::Counter) :
		return 3;
	case(EventType::FrozenCounter) :
		return 4;
	case(EventType::BinaryOutputStatus) :
		return 5;
	case(EventType::AnalogOutputStatus) :
		return 6;
	default:
		return 0;
	}
}

uint32_t ClassCount::MaskForIndex(uint32_t index)
{
	switch(index)
	{
	case(0) :
		return events::BINARY;
	case(1) :
		return events::DOUBLE_BIT_BINARY;
	case(2) :
		return events::ANALOG;
	case(3) :
		return events::COUNTER;
	case(4) :
		return events::FROZEN_COUNTER;
	case(5) :
		return events::BINARY_OUTPUT_STATUS;
	case(6) :
		return events::ANALOG_OUTPUT_STATUS;
	default:
		return 0;
	}
}

void ClassCount::Increment(EventType type)
{
	auto index = IndexOf(type);
	++counts[index];
}

ClassCount ClassCount::Subtract(const ClassCount& rhs) const
{
	ClassCount ret;
	for (uint8_t i = 0; i < NUM_TYPES; ++i)
	{
		ret.counts[i] = counts[i] - rhs.counts[i];
	}
	return ret;
}

void ClassCount::Clear()
{
	for (uint8_t i = 0; i < NUM_TYPES; ++i)
	{
		counts[i] = 0;
	}
}

bool ClassCount::HasEvents() const
{
	return !IsEmpty();
}

bool ClassCount::IsEmpty() const
{
	for (uint32_t i = 0; i < NUM_TYPES; ++i)
	{
		if (counts[i] > 0)
		{
			return false;
		}
	}

	return true;
}

uint32_t ClassCount::Total() const
{
	uint32_t sum = 0;
	for (uint32_t i = 0; i < NUM_TYPES; ++i)
	{
		sum += counts[i];
	}
	return sum;
}

uint32_t ClassCount::CountOf(uint32_t eventTypeMask) const
{
	uint32_t count = 0;
	for (uint32_t i = 0; i < NUM_TYPES; ++i)
	{
		auto mask = MaskForIndex(i);
		if (mask & eventTypeMask)
		{
			count += counts[i];
		}
	}
	return count;
}

void EventTracker::Clear()
{
	this->class1.Clear();
	this->class2.Clear();
	this->class3.Clear();
}

EventTracker EventTracker::Subtract(const EventTracker& rhs) const
{
	EventTracker ret;
	ret.class1 = class1.Subtract(rhs.class1);
	ret.class2 = class2.Subtract(rhs.class2);
	ret.class3 = class3.Subtract(rhs.class3);
	return ret;
}

bool EventTracker::IsEmpty() const
{
	return class1.IsEmpty() && class2.IsEmpty() && class3.IsEmpty();
}

uint32_t EventTracker::Total() const
{
	return class1.Total() + class2.Total() + class3.Total();
}

void EventTracker::Increment(EventType type, EventClass clazz)
{
	switch(clazz)
	{
	case(EventClass::EC1):
		class1.Increment(type);
		break;
	case(EventClass::EC2):
		class2.Increment(type);
		break;
	case(EventClass::EC3):
		class3.Increment(type);
		break;
	}
}

}
