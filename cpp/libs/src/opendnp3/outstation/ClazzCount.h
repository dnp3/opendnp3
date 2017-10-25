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
#ifndef OPENDNP3_CLAZZCOUNT_H
#define OPENDNP3_CLAZZCOUNT_H

#include "opendnp3/app/EventType.h"

namespace opendnp3
{
enum class EventState : uint8_t
{
	queued,
	selected,
	written
};

struct ClazzCount
{
public:

	void Increment(EventClass clazz)
	{
		switch (clazz)
		{
		case(EventClass::EC1):
			++num_class_1;
			break;
		case(EventClass::EC2):
			++num_class_2;
			break;
		default:
			++num_class_3;
			break;
		}
	}

	void Decrement(EventClass clazz)
	{
		switch (clazz)
		{
		case(EventClass::EC1):
			--num_class_1;
			break;
		case(EventClass::EC2):
			--num_class_2;
			break;
		default:
			--num_class_3;
			break;
		}
	}

private:

	uint32_t num_class_1 = 0;
	uint32_t num_class_2 = 0;
	uint32_t num_class_3 = 0;
};

class EventClassCounters
{

private:

	ClazzCount total;
	ClazzCount written;

public:

	void Remove(EventClass clazz, EventState state)
	{
		if (state == EventState::written)
		{
			this->written.Decrement(clazz);
		}
		this->total.Decrement(clazz);
	}

	void Add(EventClass clazz)
	{
		this->total.Increment(clazz);
	}

	void Write(EventClass clazz)
	{
		this->written.Increment(clazz);
	}
};
}

#endif

