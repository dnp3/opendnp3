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

#ifndef __OUTSTATION_EVENT_BUFFER_H_
#define __OUTSTATION_EVENT_BUFFER_H_

#include "IEventBuffer.h"
#include "EventBufferFacade.h"
#include "SelectionCriteria.h"

#include <openpal/ListAdapter.h>

namespace opendnp3
{

// The event buffer doesn't actually own the buffers, it just creates
// collection facades around the indexable buffers it's given
class OutstationEventBuffer : public IEventBuffer
{

	public:
		OutstationEventBuffer(const EventBufferFacade&);

		void Update(const Event<Binary>& aEvent) final;
		void Update(const Event<Analog>& aEvent) final;
		void Update(const Event<Counter>& aEvent) final;
		
		void Reset(); // called when a transmission fails
		void Clear(); // called when a transmission succeeds

		uint32_t SelectEvents(const SelectionCriteria&, IEventWriter* pWriter);
		
	private:

		template <class T, class EnumType>
		bool InsertEvent(const T& aEvent, EnumType eventType, openpal::RandomInsertAdapter<T>& buffer);

		bool ApplyEvent(IEventWriter* pWriter, SequenceRecord& record);

		bool overflow;

		EventBufferFacade facade;
};

template <class T, class EnumType>
bool OutstationEventBuffer::InsertEvent(const T& aEvent, EnumType eventType, openpal::RandomInsertAdapter<T>& buffer)
{
	if(buffer.IsFull() || facade.sequenceOfEvents.IsFull()) return false;	
	else 
	{		
		auto index = buffer.Add(aEvent);
		SequenceRecord record = { eventType, index, aEvent.clazz, false};
		facade.sequenceOfEvents.Add(record);
		return true;
	}
}

}

#endif

