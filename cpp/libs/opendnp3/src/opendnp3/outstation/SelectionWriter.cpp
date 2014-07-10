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

#include "SelectionWriter.h"

#include "OutstationEventBuffer.h"

using namespace openpal;

namespace opendnp3
{

SelectionWriter::SelectionWriter(
	OutstationEventBuffer& buffer,	
	const openpal::LinkedListAdapter<SequenceRecord, uint16_t>::Iterator& iterator_) :
	pBuffer(&buffer),	
	pCurrent(nullptr),
	iterator(iterator_)
{
	
}

bool SelectionWriter::WriteEvents(SelectionCriteria& criteria, ObjectWriter& writer)
{
	return true;
}

openpal::ListNode<SequenceRecord>* SelectionWriter::SeekNextNode(SelectionCriteria& criteria)
{
	while (iterator.HasNext())
	{
		auto pNode = iterator.Next();
		if (pNode && !pNode->value.selected && criteria.IsMatch(pNode->value.clazz, pNode->value.type))
		{
			return pNode;
		}
	}

	return nullptr;
}

/*
void SelectionWriter::SelectCurrent()
{
	assert(pCurrent);
	pCurrent->value.selected = true;
	pBuffer->selectedTracker.Increment(pCurrent->value.clazz);
	pBuffer->facade.selectedEvents.Push(pCurrent);
	pCurrent = nullptr;
}

bool SelectionWriter::Read(Event<Binary>& evt)
{
	return ReadAny<Binary>(evt, EventType::Binary, pBuffer->facade.binaryEvents);
}

bool SelectionWriter::Read(Event<DoubleBitBinary>& evt)
{
	return ReadAny<DoubleBitBinary>(evt, EventType::DoubleBitBinary, pBuffer->facade.doubleBinaryEvents);
}

bool SelectionWriter::Read(Event<Counter>& evt)
{
	return ReadAny<Counter>(evt, EventType::Counter, pBuffer->facade.counterEvents);
}

bool SelectionWriter::Read(Event<Analog>& evt)
{
	return ReadAny<Analog>(evt, EventType::Analog, pBuffer->facade.analogEvents);
}

bool SelectionWriter::Read(Event<FrozenCounter>& evt)
{
	return ReadAny<FrozenCounter>(evt, EventType::FrozenCounter, pBuffer->facade.frozenCounterEvents);
}

bool SelectionWriter::Read(Event<BinaryOutputStatus>& evt)
{
	return ReadAny<BinaryOutputStatus>(evt, EventType::BinaryOutputStatus, pBuffer->facade.binaryOutputStatusEvents);
}

bool SelectionWriter::Read(Event<AnalogOutputStatus>& evt)
{
	return ReadAny<AnalogOutputStatus>(evt, EventType::AnalogOutputStatus, pBuffer->facade.analogOutputStatusEvents);
}
*/



}
