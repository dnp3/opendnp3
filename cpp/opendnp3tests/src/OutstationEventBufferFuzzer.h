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

#ifndef __OUTSTATION_EVENT_BUFFER_FUZZER_H_
#define __OUTSTATION_EVENT_BUFFER_FUZZER_H_

#include <opendnp3/outstation/OutstationEventBuffer.h>

#include <iostream>
#include <random>

using namespace std;
using namespace opendnp3;

enum class Action
{
    Update,
    Select,
    Clear,
    Reset
};

class EventBufferFuzzer
{
public:

	EventBufferFuzzer() : gen(), actionDist(1, 100), classDist(0, 2), boolDist(0, 1)
	{
		gen.seed(1);
	}

	bool GetBool()
	{
		return boolDist(gen) == 0;
	}

	uint32_t PerformAction(OutstationEventBuffer& buffer, IEventWriter& writer)
	{
		switch(GetAction())
		{
		case(Action::Clear):
			this->Clear(buffer);
			return 0;
		case(Action::Reset):
			this->Reset(buffer);
			return 0;
		case(Action::Select):
			return this->Select(buffer, writer);
		case(Action::Update):
			this->Update(buffer);
			return 0;
		default:
			return 0;
		}
	}

	Action GetAction()
	{
		auto probability = actionDist(gen);
		if(probability > 10) // 90%
		{
			return Action::Update;
		}
		else if(probability > 7) return Action::Select; // 3 %
		else if(probability > 4) return Action::Clear; // 3 %
		else return Action::Reset; // 4 %
	}

	EventClass GetEventClass()
	{
		switch(classDist(gen))
		{
		case(0): return EventClass::EC1;
		case(1): return EventClass::EC2;
		case(2): return EventClass::EC3;
		default:
			return EventClass::EC1;
		}
	}

	SelectionCriteria GetSelectionCriteria()
	{
		SelectionCriteria criteria;
		criteria.class1 = GetEventTypeMask();
		criteria.class2 = GetEventTypeMask();
		criteria.class3 = GetEventTypeMask();
		return criteria;
	}

	uint8_t GetEventTypeMask()
	{
		uint8_t ret = 0;
		if(GetBool()) ret |= EventTypeMasks::BINARY;
		if(GetBool()) ret |= EventTypeMasks::ANALOG;
		if(GetBool()) ret |= EventTypeMasks::COUNTER;
		return ret;
	}

	void Clear(OutstationEventBuffer& buffer)
	{
		buffer.Clear();
	}

	void Reset(OutstationEventBuffer& buffer)
	{
		buffer.Reset();
	}

	void Update(OutstationEventBuffer& buffer)
	{
		switch(classDist(gen))
		{
		case(0):
			buffer.Update(Event<Binary>(false, 0, GetEventClass()));
			break;
		case(1):
			buffer.Update(Event<Counter>(0, 0, GetEventClass()));
			break;
		case(2):
			buffer.Update(Event<Analog>(0, 0, GetEventClass()));
			break;
		}
	}

	uint32_t Select(OutstationEventBuffer& buffer, IEventWriter& writer)
	{
		return buffer.SelectEvents(GetSelectionCriteria(), writer);
	}

	std::mt19937 gen;
	std::uniform_int_distribution<size_t> actionDist;
	std::uniform_int_distribution<size_t> classDist;
	std::uniform_int_distribution<size_t> boolDist;
};

#endif
