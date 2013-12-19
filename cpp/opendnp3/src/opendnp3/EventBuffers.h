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
#ifndef __EVENT_BUFFERS_H_
#define __EVENT_BUFFERS_H_

#include "BufferSetTypes.h"
#include "EventBufferBase.h"



#include <map>
#include <set>
#include <vector>
#include <limits>

namespace opendnp3
{

/** Event buffer that only stores one event per Index:

	Note: EventType must have the public property mIndex.
	*/
template <class T>
class SingleEventBuffer : public EventBufferBase<T, IndexSet< EventInfo<T> > >
{
public:

	SingleEventBuffer(size_t aMaxEvents);

	void _Update(const EventInfo<T>& arEvent);
};

/** Event buffer that stores all changes to all points in the order. */
template <class T>
class TimeOrderedEventBuffer : public EventBufferBase<T, TimeMultiSet<EventInfo<T> > >
{
public:

	TimeOrderedEventBuffer(size_t aMaxEvents);
};

/** Event buffer that stores all changes to all points in the order. */
template <class T>
class InsertionOrderedEventBuffer : public EventBufferBase<T, InsertionOrderSet<EventInfo<T> > >
{
public:

	InsertionOrderedEventBuffer(size_t aMaxEvents);
};

template <class T>
SingleEventBuffer<T> :: SingleEventBuffer(size_t aMaxEvents) :
	EventBufferBase< T, IndexSet< EventInfo<T> > >(aMaxEvents)
{}

template <class T>
TimeOrderedEventBuffer<T> :: TimeOrderedEventBuffer(size_t aMaxEvents) :
	EventBufferBase <T, TimeMultiSet< EventInfo<T> > >(aMaxEvents)
{}

template <class T>
InsertionOrderedEventBuffer<T> :: InsertionOrderedEventBuffer(size_t aMaxEvents) :
	EventBufferBase<T, InsertionOrderSet< EventInfo<T> > >(aMaxEvents)
{}

template <class T>
void SingleEventBuffer<T> :: _Update(const EventInfo<T>& aEvent)
{
	auto i = this->mEventSet.find(aEvent);

	if(i != this->mEventSet.end() ) {
		if(aEvent.value.GetTime() >= i->value.GetTime()) {
			this->mEventSet.erase(i);
			this->mEventSet.insert(aEvent); //new event
		}
	}
	else {
		this->mEventSet.insert(aEvent); //new event
		//this->mCounter.IncrCount(aEvent.clazz);
	}
}

} //end NS

#endif
