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

SelectionWriter::SelectionWriter(OutstationEventBuffer& buffer) : pBuffer(&buffer) {}

bool SelectionWriter::WriteAllEvents(const EventResponseConfig& config, SelectionCriteria& criteria, ObjectWriter& writer)
{	
	auto apduHasSpace = true;
	auto iterator = pBuffer->facade.sequenceOfEvents.Iterate();
			
	while (apduHasSpace && iterator.HasNext() && criteria.HasSelection())
	{
		auto pNode = SeekNextWriteableNode(iterator);		

		if (pNode)
		{
			auto operation = criteria.GetWriteOperationFor(config, pNode->value.clazz, pNode->value.type);
			if (operation.IsDefined())
			{
				apduHasSpace = operation.Invoke(writer, criteria);
			}						
		}

	} 
	
	return apduHasSpace;
}

openpal::ListNode<SOERecord>* SelectionWriter::SeekNextWriteableNode(openpal::LinkedListIterator<SOERecord>& iterator)
{
	while (iterator.HasNext())
	{
		auto pNode = iterator.Next();
		
		if (pNode && !pNode->value.selected)
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
*/



}
