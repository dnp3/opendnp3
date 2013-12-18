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

#include "EventBufferFacade.h"

namespace opendnp3
{

SelectionIterator::SelectionIterator(EventBufferFacade* apFacade, SelectionCriteria aCriteria) :
	pFacade(apFacade),
	criteria(aCriteria)
{

}

void SelectionIterator::Apply(IEventWriter* pWriter)
{	
	auto& soe = pFacade->sequenceOfEvents;
	for(uint32_t pos = 0; pos < soe.Size(); ++pos)
	{
		auto& record = soe[pos];
		if(criteria.IsMatch(record.clazz, record.type)) 
		{
			if(!ApplyEvent(pWriter, soe[pos])) return;
		}				
	}
}

bool SelectionIterator::ApplyEvent(IEventWriter* pWriter, SequenceRecord& record)
{
	switch(record.type)
	{
		case(EventType::Binary):
		{
			auto& value = pFacade->binaryEvents[record.index];
			if(pWriter->Write(value))
			{
				pFacade->selectedEvents.Push(record.index);
				return true;
			}
			return false;
		}
		case(EventType::Analog):
		{
			auto& value = pFacade->analogEvents[record.index];
			if(pWriter->Write(value))
			{
				pFacade->selectedEvents.Push(record.index);
				return true;
			}
			return false;
		}
		case(EventType::Counter):
		{
			auto& value = pFacade->counterEvents[record.index];
			if(pWriter->Write(value))
			{
				pFacade->selectedEvents.Push(record.index);
				return true;
			}
			return false;
		}
		default:
			return false;
	}
}

}


