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

#ifndef OPENDNP3_EVENTCOUNT_H
#define OPENDNP3_EVENTCOUNT_H

#include <cstdint>

#include <openpal/util/Uncopyable.h>

#include "opendnp3/app/ClassField.h"
#include "opendnp3/app/EventType.h"

namespace opendnp3
{

class EventCount : openpal::Uncopyable
{
	static const uint16_t NUM_CLASSES = 3;
	static const uint16_t NUM_TYPES = 7;
	
public:

	EventCount();	

	ClassField ToClassField() const;

	uint32_t NumOfClass(EventClass clazz) const;
	uint32_t NumOfType(EventType type) const;

	ClassField Subtract(const EventCount& rhs) const;

	void Increment(EventClass clazz, EventType type, uint32_t count = 1);
	void Decrement(EventClass clazz, EventType type, uint32_t count = 1);
	
	bool IsEmpty() const;	

	void Clear();	

private:

	uint16_t numOfTypeAndClass[NUM_CLASSES][NUM_TYPES];	
};

}

#endif
