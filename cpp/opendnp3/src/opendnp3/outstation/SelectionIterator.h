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

#ifndef __SELECTION_ITERATOR_H_
#define __SELECTION_ITERATOR_H_

#include "OutstationEventBuffer.h"

#include <openpal/LinkedListAdapter.h>
#include <openpal/Option.h>

namespace opendnp3
{

class OutstationEventBuffer;

class SelectionIterator
{
	friend class OutstationEventBuffer;	

public:

	openpal::Option<EventType> SeekNext();

	void SelectCurrent();

	void Read(Event<Binary>& evt);
	void Read(Event<Counter>& evt);
	void Read(Event<Analog>& evt);
	void Read(Event<FrozenCounter>& evt);
	/*
	void Read(Event<DoubleBitBinary>& evt);
	void Read(Event<BinaryOutputStatus>& evt);
	void Read(Event<AnalogOutputStatus>& evt);
*/
	

private:

	template <class T>
	void ReadAny(Event<T>& evt, EventType type, const openpal::RandomInsertAdapter<Event<T>, uint16_t>& adapter)
	{
		assert(pCurrent);
		assert(pCurrent->value.type == type);
		evt = adapter[pCurrent->value.index];
	}

	SelectionIterator(OutstationEventBuffer* pBuffer_, 
						const SelectionCriteria& criteria_, 
						const openpal::LinkedListAdapter<SequenceRecord, uint16_t>::Iterator& iterator_);

	OutstationEventBuffer* pBuffer;
	SelectionCriteria criteria;
	openpal::ListNode<SequenceRecord>* pCurrent;
	openpal::LinkedListAdapter<SequenceRecord, uint16_t>::Iterator iterator;
};

}

#endif

