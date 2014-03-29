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

#include "SelectionIterator.h"

#include "OutstationEventBuffer.h"

using namespace openpal;

namespace opendnp3
{

SelectionIterator::SelectionIterator(OutstationEventBuffer* pBuffer_, const SelectionCriteria& criteria_, const openpal::LinkedListAdapter<SequenceRecord, uint16_t>::Iterator& iterator_) : 
	pBuffer(pBuffer_),
	criteria(criteria_),
	pCurrent(nullptr),
	iterator(iterator_)
{}

Option<EventType> SelectionIterator::SeekNext()
{
	while (iterator.HasNext())
	{
		pCurrent = iterator.Next();
		if (!pCurrent->value.selected && criteria.IsMatch(pCurrent->value.clazz, pCurrent->value.type))
		{
			return Option<EventType>::Some(pCurrent->value.type);
		}	
	}

	pCurrent = nullptr;
	return Option<EventType>::None();
}

openpal::Option<EventType> SelectionIterator::Current()
{
	if (pCurrent)
	{
		return Option<EventType>::Some(pCurrent->value.type);
	}
	else
	{
		return Option<EventType>::None();		
	}
}

void SelectionIterator::SelectCurrent()
{
	assert(pCurrent);
	pCurrent->value.selected = true;
	pBuffer->selectedTracker.Increment(pCurrent->value.type, pCurrent->value.clazz);	
	pBuffer->facade.selectedEvents.Push(pCurrent);	
	pCurrent = nullptr;
}

bool SelectionIterator::Read(Event<Binary>& evt)
{
	return ReadAny<Binary>(evt, EventType::Binary, pBuffer->facade.binaryEvents);
}

bool SelectionIterator::Read(Event<Counter>& evt)
{
	return ReadAny<Counter>(evt, EventType::Counter, pBuffer->facade.counterEvents);
}

bool SelectionIterator::Read(Event<Analog>& evt)
{
	return ReadAny<Analog>(evt, EventType::Analog, pBuffer->facade.analogEvents);
}

bool SelectionIterator::Read(Event<FrozenCounter>& evt)
{
	return ReadAny<FrozenCounter>(evt, EventType::FrozenCounter, pBuffer->facade.frozenCounterEvents);
}




}
