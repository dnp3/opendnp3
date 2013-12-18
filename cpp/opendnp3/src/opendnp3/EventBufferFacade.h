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

#ifndef __EVENT_BUFFER_FACADE_H_
#define __EVENT_BUFFER_FACADE_H_

#include "Event.h"

#include <openpal/ListAdapter.h>
#include <openpal/RandomInsertAdapter.h>
#include <opendnp3/DataTypes.h>
#include <opendnp3/PointClass.h>

namespace opendnp3
{

enum class EventType: uint8_t
{
	Binary,
	Analog,
	Counter
};

struct SequenceRecord
{
	EventType type;
	uint32_t typeIndex;
	PointClass clazz;
	bool selected;
};

struct EventBufferFacade
{
	public:

	EventBufferFacade(	openpal::RandomInsertAdapter<Event<Binary>> aBinaryEvents,
						openpal::RandomInsertAdapter<Event<Analog>> aAnalogEvents,
						openpal::RandomInsertAdapter<Event<Counter>> aCounterEvents,						
						openpal::ListAdapter<SequenceRecord> aSequenceOfEvents) :

		binaryEvents(aBinaryEvents),
		analogEvents(aAnalogEvents),
		counterEvents(aCounterEvents),		
		sequenceOfEvents(aSequenceOfEvents)
	{
		
	}

	openpal::RandomInsertAdapter<Event<Binary>> binaryEvents;
	openpal::RandomInsertAdapter<Event<Analog>> analogEvents;
	openpal::RandomInsertAdapter<Event<Counter>> counterEvents;	
	openpal::ListAdapter<SequenceRecord> sequenceOfEvents;
};


}

#endif
