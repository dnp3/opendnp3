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

#include "DynamicallyAllocatedEventBuffer.h"

using namespace openpal;

namespace opendnp3
{

DynamicallyAllocatedEventBuffer::DynamicallyAllocatedEventBuffer(const EventBufferConfig& config) :
	binaryStack(config.maxBinaryEvents),
	binaryArray(config.maxBinaryEvents),
	doubleBinaryStack(config.maxDoubleBinaryEvents),
	doubleBinaryArray(config.maxDoubleBinaryEvents),
	analogStack(config.maxAnalogEvents),
	analogArray(config.maxAnalogEvents),
	counterStack(config.maxCounterEvents),
	counterArray(config.maxCounterEvents),
	frozenCounterStack(config.maxFrozenCounterEvents),
	frozenCounterArray(config.maxFrozenCounterEvents),
	binaryOutputStatusStack(config.maxBinaryOutputStatusEvents),
	binaryOutputStatusArray(config.maxBinaryOutputStatusEvents),
	analogOutputStatusStack(config.maxAnalogOutputStatusEvents),
	analogOutputStatusArray(config.maxAnalogOutputStatusEvents),
	sequenceOfEvents(config.TotalEvents()),
	selectedEvents(config.TotalEvents())
{}

EventBufferFacade DynamicallyAllocatedEventBuffer::GetFacade()
{
	RandomInsertAdapter<Event<Binary>, uint16_t> binaryAdapter(binaryArray.ToIndexable(), binaryStack.ToIndexable());
	RandomInsertAdapter<Event<DoubleBitBinary>, uint16_t> doubleBinaryAdapter(doubleBinaryArray.ToIndexable(), doubleBinaryStack.ToIndexable());
	RandomInsertAdapter<Event<Analog>, uint16_t> analogAdapter(analogArray.ToIndexable(), analogStack.ToIndexable());
	RandomInsertAdapter<Event<Counter>, uint16_t> counterAdapter(counterArray.ToIndexable(), counterStack.ToIndexable());
	RandomInsertAdapter<Event<FrozenCounter>, uint16_t> frozenCounterAdapter(frozenCounterArray.ToIndexable(), frozenCounterStack.ToIndexable());
	RandomInsertAdapter<Event<BinaryOutputStatus>, uint16_t> binaryOutputStatusAdapter(binaryOutputStatusArray.ToIndexable(), binaryOutputStatusStack.ToIndexable());
	RandomInsertAdapter<Event<AnalogOutputStatus>, uint16_t> analogOutputStatusAdapter(analogOutputStatusArray.ToIndexable(), analogOutputStatusStack.ToIndexable());

	LinkedListAdapter<SequenceRecord, uint16_t> soeAdapter(sequenceOfEvents.ToIndexable());
	StackAdapter<ListNode<SequenceRecord>*, uint16_t> selectionAdapter(selectedEvents.ToIndexable());

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

}
