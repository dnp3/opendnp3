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

#ifndef __STATICALLY_ALLOCATED_EVENT_BUFFER_
#define __STATICALLY_ALLOCATED_EVENT_BUFFER_

#include "EventBufferFacade.h"

#include <openpal/RandomInsertAdapter.h>
#include <openpal/StaticArray.h>
#include <openpal/StaticArray.h>

namespace opendnp3
{

template <uint32_t N>
class StaticallyAllocatedEventBuffer
{
public:

	EventBufferFacade GetFacade()
	{
		openpal::RandomInsertAdapter<Event<Binary>, uint16_t> binaryAdapter(binaryArray.ToIndexable(), binaryStack.ToIndexable());
		openpal::RandomInsertAdapter<Event<DoubleBitBinary>, uint16_t> doubleBinaryAdapter(doubleBinaryArray.ToIndexable(), doubleBinaryStack.ToIndexable());
		openpal::RandomInsertAdapter<Event<Analog>, uint16_t> analogAdapter(analogArray.ToIndexable(), analogStack.ToIndexable());
		openpal::RandomInsertAdapter<Event<Counter>, uint16_t> counterAdapter(counterArray.ToIndexable(), counterStack.ToIndexable());
		openpal::RandomInsertAdapter<Event<FrozenCounter>, uint16_t> frozenCounterAdapter(frozenCounterArray.ToIndexable(), frozenCounterStack.ToIndexable());
		openpal::RandomInsertAdapter<Event<BinaryOutputStatus>, uint16_t> binaryOutputStatusAdapter(binaryOutputStatusArray.ToIndexable(), binaryOutputStatusStack.ToIndexable());
		openpal::RandomInsertAdapter<Event<AnalogOutputStatus>, uint16_t> analogOutputStatusAdapter(analogOutputStatusArray.ToIndexable(), analogOutputStatusStack.ToIndexable());

		openpal::LinkedListAdapter<SequenceRecord, uint16_t> soeAdapter(sequenceOfEvents.ToIndexable());
		openpal::StackAdapter<openpal::ListNode<SequenceRecord>*, uint16_t> selectionAdapter(selectedEvents.ToIndexable());

		return EventBufferFacade(
			binaryAdapter,
			doubleBinaryAdapter,
			analogAdapter,
			counterAdapter,
			frozenCounterAdapter,
			binaryOutputStatusAdapter,
			analogOutputStatusAdapter,
			soeAdapter,
			selectionAdapter
		);
	}

private:

	openpal::StaticArray<uint16_t, uint16_t, N> binaryStack;
	openpal::StaticArray<Event<Binary>, uint16_t, N> binaryArray;

	openpal::StaticArray<uint16_t, uint16_t, N> doubleBinaryStack;
	openpal::StaticArray<Event<DoubleBitBinary>, uint16_t, N> doubleBinaryArray;

	openpal::StaticArray<uint16_t, uint16_t, N> analogStack;
	openpal::StaticArray<Event<Analog>, uint16_t, N> analogArray;

	openpal::StaticArray<uint16_t, uint16_t, N> counterStack;
	openpal::StaticArray<Event<Counter>, uint16_t, N> counterArray;
	
	openpal::StaticArray<uint16_t, uint16_t, N> frozenCounterStack;
	openpal::StaticArray<Event<FrozenCounter>, uint16_t, N> frozenCounterArray;
	
	openpal::StaticArray<uint16_t, uint16_t, N> binaryOutputStatusStack;
	openpal::StaticArray<Event<BinaryOutputStatus>, uint16_t, N> binaryOutputStatusArray;
	
	openpal::StaticArray<uint16_t, uint16_t, N> analogOutputStatusStack;
	openpal::StaticArray<Event<AnalogOutputStatus>, uint16_t, N> analogOutputStatusArray;

	openpal::StaticArray < openpal::ListNode<SequenceRecord>, uint16_t, 7*N> sequenceOfEvents;
	openpal::StaticArray < openpal::ListNode<SequenceRecord>*, uint16_t, 7*N> selectedEvents;
};

}

#endif
