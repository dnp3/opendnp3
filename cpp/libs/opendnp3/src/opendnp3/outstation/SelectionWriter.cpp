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

#include "EventBuffer.h"

using namespace openpal;

namespace opendnp3
{

SelectionWriter::SelectionWriter(EventBuffer& buffer) : 
	pBuffer(&buffer)
	//iterator(buffer.sequenceOfEvents.Iterate()) 
{

}

/*
bool SelectionWriter::WriteAllEvents(SelectionCriteria& criteria, HeaderWriter& writer)
{		
	bool hasSpace = true;

	while (hasSpace && this->SeekNextUnselectedNode(iterator) && criteria.HasSelection())
	{	
		auto pStart = iterator.Current();
		
		auto operation = criteria.GetWriteOperationFor(pStart->value.clazz, pStart->value.type);

		if (operation.IsDefined())
		{					
			auto pCriteria = &criteria;
				
			// callback that tells us this record was written to the apdu
			auto callback = [this, pCriteria](ListNode<SOERecord>* pNode)
			{
				pNode->value.selected = true;
				pBuffer->selectedTracker.Increment(pNode->value.clazz, pNode->value.type);
				pBuffer->selectedEvents.Push(pNode);
				pCriteria->RecordAsWritten(pNode->value.clazz, pNode->value.type);
				this->SeekNextUnselectedNode(iterator);				
				return iterator.Current();
			};

			hasSpace = operation.Invoke(writer, pStart, Function1<ListNode<SOERecord>*, ListNode<SOERecord>*>::Bind(callback));			
		}
		else
		{
			iterator.Next();
		}
	} 
	
	return hasSpace;
}


bool SelectionWriter::SeekNextUnselectedNode(openpal::LinkedListIterator<SOERecord>& iterator)
{
	while (iterator.CurrentValue())
	{				
		if (iterator.CurrentValue()->selected)
		{
			iterator.Next();
		}
		else
		{
			return true;
		}
	} 	

	return false;
}
*/

/*
void SelectionWriter::SelectCurrent()
{
	assert(pCurrent);
	pCurrent->value.selected = true;
	pBuffer->selectedTracker.Increment(pCurrent->value.clazz);
	pBuffer->selectedEvents.Push(pCurrent);
	pCurrent = nullptr;
}
*/



}
