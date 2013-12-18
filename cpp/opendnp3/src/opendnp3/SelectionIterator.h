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

#include "EventBufferFacade.h"

namespace opendnp3
{

struct SelectionCriteria
{
	bool binary;
	bool analog;	
	bool counter;
	bool class1;
	bool class2;
	bool class3;

	inline bool IsMatch(PointClass clazz, EventType type) const
	{
		return IsMatch(clazz) && IsMatch(type);
	}

	private:

	inline bool IsMatch(PointClass clazz) const
	{
		switch(clazz)
		{
			case(PointClass::PC_CLASS_1): return class1;
			case(PointClass::PC_CLASS_2): return class2;
			case(PointClass::PC_CLASS_3): return class3;
			default:
				return false;
		}
	}

	inline bool IsMatch(EventType type) const
	{
		switch(type)
		{
			case(EventType::Binary): return binary;
			case(EventType::Analog): return analog;
			case(EventType::Counter): return counter;
			default:
				return false;
		}
	}
};

class IEventWriter
{
	public:
		
		// returns true of their was space in the fragment and the event should be added to selection buffer
		virtual bool Write(const Event<Binary>& evt) = 0;
		virtual bool Write(const Event<Analog>& evt) = 0;
		virtual bool Write(const Event<Counter>& evt) = 0;
};



// The event buffer doesn't actually own the buffers, it just creates
// collection facades around the indexable buffers it's given
class SelectionIterator
{
	public:
		SelectionIterator(EventBufferFacade* apFacade, SelectionCriteria aCriteria);

		// keep applying values meeting the criteria until the event writer returns false
		void Apply(IEventWriter* pWriter);

	private:

		bool ApplyEvent(IEventWriter* pWriter, SequenceRecord& record);		

		EventBufferFacade* pFacade;
		SelectionCriteria criteria;
};

}

#endif
