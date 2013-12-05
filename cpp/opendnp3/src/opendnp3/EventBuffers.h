#ifndef __EVENT_BUFFERS_H_
#define __EVENT_BUFFERS_H_

#include "BufferSetTypes.h"
#include "EventBufferBase.h"

#include <openpal/Visibility.h>

#include <map>
#include <set>
#include <vector>
#include <limits>

namespace opendnp3
{

/** Event buffer that only stores one event per Index:

	Note: EventType must have the public property mIndex.
	*/
template <class EventType>
class DLL_LOCAL SingleEventBuffer : public EventBufferBase<EventType, IndexSet< EventType > >
{
public:

	SingleEventBuffer(size_t aMaxEvents);

	void _Update(const EventType& arEvent);
};

/** Event buffer that stores all changes to all points in the order. */
template <class EventType>
class DLL_LOCAL TimeOrderedEventBuffer : public EventBufferBase<EventType, TimeMultiSet< EventType > >
{
public:

	TimeOrderedEventBuffer(size_t aMaxEvents);
};

/** Event buffer that stores all changes to all points in the order. */
template <class EventType>
class DLL_LOCAL InsertionOrderedEventBuffer : public EventBufferBase<EventType, InsertionOrderSet< EventType > >
{
public:

	InsertionOrderedEventBuffer(size_t aMaxEvents);
};

template <class EventType>
SingleEventBuffer<EventType> :: SingleEventBuffer(size_t aMaxEvents) :
	EventBufferBase< EventType, IndexSet< EventType > >(aMaxEvents)
{}

template <class EventType>
TimeOrderedEventBuffer<EventType> :: TimeOrderedEventBuffer(size_t aMaxEvents) :
	EventBufferBase <EventType, TimeMultiSet< EventType > >(aMaxEvents)
{}

template <class EventType>
InsertionOrderedEventBuffer<EventType> :: InsertionOrderedEventBuffer(size_t aMaxEvents) :
	EventBufferBase<EventType, InsertionOrderSet< EventType > >(aMaxEvents)
{}

template <class EventType>
void SingleEventBuffer<EventType> :: _Update(const EventType& arEvent)
{
	typename IndexSet< EventType >::Type::iterator i = this->mEventSet.find(arEvent);

	if(i != this->mEventSet.end() ) {
		if(arEvent.mValue.GetTime() >= i->mValue.GetTime()) {
			this->mEventSet.erase(i);
			this->mEventSet.insert(arEvent); //new event
		}
	}
	else {
		this->mEventSet.insert(arEvent); //new event
		this->mCounter.IncrCount(arEvent.mClass);
	}
}

} //end NS

#endif
