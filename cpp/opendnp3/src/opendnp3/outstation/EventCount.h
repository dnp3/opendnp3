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

#ifndef __EVENT_COUNT_H_
#define __EVENT_COUNT_H_

#include <cstdint>
#include "EventType.h"

namespace opendnp3
{

class ClassCount
{
	public:
	
		ClassCount() : 
		numAnalog(0), 
		numBinary(0), 
		numCounter(0)
	{}

	ClassCount Subtract(const ClassCount& rhs) const;

	void Increment(EventType type);		

	bool IsEmpty() const;
	uint32_t Total() const;

	uint32_t CountOf(uint8_t eventTypeMask) const;

	void Clear();

	uint32_t numAnalog;
	uint32_t numBinary;
	uint32_t numCounter;
};


class EventTracker
{
	public:	

	void Increment(EventType type, EventClass clazz);
	void Decrement(EventType type, EventClass clazz);

	EventTracker Subtract(const EventTracker& rhs) const;

	bool IsEmpty() const;
	uint32_t Total() const;

	void Clear();

	ClassCount class1;
	ClassCount class2;
	ClassCount class3;
};

}

#endif
