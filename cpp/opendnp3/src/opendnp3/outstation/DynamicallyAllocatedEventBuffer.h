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

#ifndef __DYNAMICALLY_ALLOCATED_EVENT_BUFFER_
#define __DYNAMICALLY_ALLOCATED_EVENT_BUFFER_

#include "EventBufferFacade.h"
#include <openpal/DynamicArray.h>

namespace opendnp3
{

class DynamicallyAllocatedEventBuffer
{
	public:
		DynamicallyAllocatedEventBuffer(uint16_t aMaxBinary, uint16_t aMaxAnalog, uint16_t aMaxCounter, uint16_t aMaxFrozenCounter);

		EventBufferFacade GetFacade();

	private:
		openpal::DynamicArray<uint16_t, uint16_t> binaryStack;
		openpal::DynamicArray<Event<Binary>, uint16_t> binaryArray;

		openpal::DynamicArray<uint16_t, uint16_t> analogStack;
		openpal::DynamicArray<Event<Analog>, uint16_t> analogArray;

		openpal::DynamicArray<uint16_t, uint16_t> counterStack;
		openpal::DynamicArray<Event<Counter>, uint16_t> counterArray;

		openpal::DynamicArray<uint16_t, uint16_t> frozenCounterStack;
		openpal::DynamicArray<Event<FrozenCounter>, uint16_t> frozenCounterArray;

		openpal::DynamicArray<openpal::DoubleListNode<SequenceRecord>, uint16_t> sequenceOfEvents;
		openpal::DynamicArray<openpal::DoubleListNode<SequenceRecord>*, uint16_t> selectedEvents;		
};

}

#endif
