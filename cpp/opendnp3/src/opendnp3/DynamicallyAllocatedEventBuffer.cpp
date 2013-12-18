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

DynamicallyAllocatedEventBuffer::DynamicallyAllocatedEventBuffer(uint32_t aMaxBinary, uint32_t aMaxAnalog, uint32_t aMaxCounter) :
	binaryStack(aMaxBinary),
	binaryArray(aMaxBinary),
	analogStack(aMaxAnalog),
	analogArray(aMaxAnalog),
	counterStack(aMaxCounter),
	counterArray(aMaxCounter),
	sequenceOfEvents(aMaxBinary + aMaxAnalog + aMaxCounter),
	selectedEvents(aMaxBinary + aMaxAnalog + aMaxCounter)
{}

EventBufferFacade DynamicallyAllocatedEventBuffer::GetFacade()
{
	RandomInsertAdapter<Event<Binary>> binaryAdapter(binaryArray.ToIndexable(), binaryStack.ToIndexable());
	RandomInsertAdapter<Event<Analog>> analogAdapter(analogArray.ToIndexable(), analogStack.ToIndexable());
	RandomInsertAdapter<Event<Counter>> counterAdapter(counterArray.ToIndexable(), counterStack.ToIndexable());
	DoublyLinkedListAdapter<SequenceRecord> soeAdapter(sequenceOfEvents.ToIndexable());
	StackAdapter<DoubleListNode<SequenceRecord>*> selectionAdapter(selectedEvents.ToIndexable());

	return EventBufferFacade(binaryAdapter, analogAdapter, counterAdapter, soeAdapter, selectionAdapter);
}

}
