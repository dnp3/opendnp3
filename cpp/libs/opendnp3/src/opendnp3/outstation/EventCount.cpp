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

#include "opendnp3/gen/PointClass.h"

namespace opendnp3
{

EventCount::EventCount()
{
	this->Clear();
}

void EventCount::Clear()
{
	for (uint16_t clazz = 0; clazz < NUM_CLASSES; ++clazz)
	{
		for (uint16_t type = 0; type < NUM_TYPES; ++type)
		{
			numOfTypeAndClass[clazz][type] = 0;
		}
	}
}

ClassField EventCount::Subtract(const EventCount& rhs) const
{	
	EventCount count;

	for (auto clazz = 0; clazz < NUM_CLASSES; ++clazz)
	{
		for (auto type = 0; type < NUM_TYPES; ++type)
		{
			count.numOfTypeAndClass[clazz][type] = numOfTypeAndClass[clazz][type] - rhs.numOfTypeAndClass[clazz][type];
		}
	}

	return count.ToClassField();
}

ClassField EventCount::ToClassField() const
{
	bool class1 = this->NumOfClass(EventClass::EC1) > 0;
	bool class2 = this->NumOfClass(EventClass::EC2) > 0;
	bool class3 = this->NumOfClass(EventClass::EC3) > 0;

	return ClassField(false, class1, class2, class3);
}

uint32_t EventCount::NumOfClass(EventClass clazz) const
{
	uint32_t total = 0;

	for (auto type = 0; type < NUM_TYPES; ++type)
	{
		total += numOfTypeAndClass[static_cast<int>(clazz)][type];
	}

	return total;
}

uint32_t EventCount::NumOfType(EventType type) const
{
	uint32_t total = 0;

	for (auto clazz = 0; clazz < NUM_CLASSES; ++clazz)
	{
		total += numOfTypeAndClass[clazz][static_cast<int>(type)];
	}

	return total;
}

bool EventCount::IsEmpty() const
{
	return ToClassField().IsEmpty();	
}

void EventCount::Increment(EventClass clazz, EventType type, uint32_t count)
{
	numOfTypeAndClass[static_cast<int>(clazz)][static_cast<int>(type)] += count;
}

void EventCount::Decrement(EventClass clazz, EventType type, uint32_t count)
{
	numOfTypeAndClass[static_cast<int>(clazz)][static_cast<int>(type)] -= count;
}

}
