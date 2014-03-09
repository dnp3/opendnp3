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

void ClassCount::Increment(EventType type)
{
	switch(type)
	{
	case(EventType::Binary):
		++numBinary;
		break;
	case(EventType::Analog):
		++numAnalog;
		break;
	case(EventType::Counter):
		++numCounter;
		break;
	}
}

ClassCount ClassCount::Subtract(const ClassCount& rhs) const
{
	ClassCount ret;
	ret.numBinary = this->numBinary - rhs.numBinary;
	ret.numAnalog = this->numAnalog - rhs.numAnalog;
	ret.numCounter = this->numCounter - rhs.numCounter;
	return ret;
}

void ClassCount::Clear()
{
	numBinary = 0;
	numAnalog = 0;
	numCounter = 0;
}

bool ClassCount::IsEmpty() const
{
	return (numBinary == 0) && (numAnalog == 0) && (numCounter) == 0;
}

uint32_t ClassCount::Total() const
{
	return numBinary + numAnalog + numCounter;
}

uint32_t ClassCount::CountOf(uint8_t eventTypeMask) const
{
	uint32_t count = 0;
	if(eventTypeMask & EventTypeMasks::BINARY) count += numBinary;
	if(eventTypeMask & EventTypeMasks::ANALOG) count += numAnalog;
	if(eventTypeMask & EventTypeMasks::COUNTER) count += numCounter;
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
