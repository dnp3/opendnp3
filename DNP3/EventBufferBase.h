//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one
// or more contributor license agreements. See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  Green Enery Corp licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
//
#ifndef __EVENT_BUFFER_BASE_H_
#define __EVENT_BUFFER_BASE_H_


#include "ClassCounter.h"
#include "EventTypes.h"

namespace apl
{
namespace dnp
{

template <class EventType>
class EventAcceptor
{
public:
	virtual void Update(const typename EventType::MeasType& arVal, PointClass aClass, size_t aIndex) = 0;
};

/**
 * Base class for the EventBuffer classes (with templating and virtual
 * function for Update to alter event storage behavior)
 *
 * Single-threaded for asynchronous/event-based model.
*/
template <class EventType, class SetType>
class EventBufferBase : public EventAcceptor<EventType>
{
public:
	EventBufferBase(size_t aMaxEvents);
	virtual ~EventBufferBase() {}

	/**
	 * Adds an event to the buffer, tracking insertion order. Calls _Update
	 * which can be overriden to do special types of insertion.
	 *
	 * @param arVal			Event update to add to the buffer
	 * @param aClass		Class of the measurement
	 * @param aIndex		Index of the measurement
	 */
	void Update(const typename EventType::MeasType& arVal, PointClass aClass, size_t aIndex);

	/**
	 * Returns true if the buffer contains any data matching the given
	 * PointClass.
	 *
	 * @param aClass		the class of data to match
	 *
	 * @return				'true' if the buffer contains matching data,
	 * 						'false' if not
	 */
	bool HasClassData(PointClass aClass) {
		return mCounter.GetNum(aClass) > 0;
	}

	/**
	 * Selects data in the buffer that matches the given PointClass, up to
	 * the defined number of entries.
	 *
	 * @param aClass		the class of data to match
	 * @param aMaxEvent		maximum number of events to select
	 *
	 * @return				the number of events selected
	 */
	size_t Select(PointClass aClass, size_t aMaxEvent = std::numeric_limits<size_t>::max());

	/**
	 * Transfers any unwritten events back into the event set.
	 *
	 * @return				the number of events returned to the event set
	 */
	size_t Deselect();


	/**
	 * Remove events that have been written (flagged with 'mWritten=true')
	 * from the selection buffer.
	 *
	 * @return				the number of events removed
	 */
	size_t ClearWrittenEvents();

	/**
	 * Returns an EvtItr iterator object for accessing the data previously
	 * selected through EventBufferBase::Select().
	 *
	 * @return				the iterator object to the data from Select()
	 */
	typename EvtItr< EventType >::Type Begin();

	/**
	 * Returns the number of events that were previously selected through
	 * EventBufferBase::Select().
	 *
	 * @return				the number of events selected
	 */
	size_t NumSelected() {
		return mSelectedEvents.size();
	}

	/**
	 * Returns the number of events that were omitted from the selection
	 * during the last call of EventBufferBase::Select().
	 *
	 * @return				the number of events not selected
	 */
	size_t NumUnselected() {
		return mEventSet.size();
	}

	/**
	 * Returns the total number of events (selected and unselected) being
	 * managed by the EventBufferBase.
	 *
	 * @return				the number of events
	 */
	size_t Size() {
		return mSelectedEvents.size() + mEventSet.size();
	}

	/**
	 * @return the number of events before the buffer will overflow
	 *
	 */
	size_t NumAvailable() {
		return M_MAX_EVENTS - this->Size();
	}

	/**
	 * Returns a flag to indicate whether the buffer has been overflown.
	 *
	 * @return				true if too much data has been written to the
	 * 						buffer, false if not.
	 */
	bool IsOverflown();

	/**
	 * Returns a flag to indicate whether the buffer is full.  A subsequent
	 * write the buffer at this point would lead to IsOverflown() returning
	 * true.
	 *
	 * @return				true if the buffer is full, false if the buffer
	 * 						still has space
	 */
	bool IsFull() {
		return NumUnselected() >= M_MAX_EVENTS;
	}

protected:

	/**
	 * Adds an event to the buffer, tracking insertion order. Calls _Update
	 * which can be overriden to do special types of insertion.
	 *
	 * @param arEvent		Event update to add to the buffer
	 * @param aNewValue		a new sequence value will be assigned to the
	 * 						arEvent if the value is true, otherwise the
	 * 						existing arEvent sequence number will be used
	 */
	void Update(EventType& arEvent, bool aNewValue = true);

	/**
	 * Overridable NVII function called by Update.  The default
	 * implementation does a simple insert into mEventSet.
	 *
	 * @param arEvent		Event update to add to the buffer
	 */
	virtual void _Update(const EventType& arEvent);

	ClassCounter mCounter;		// counter for class events
	const size_t M_MAX_EVENTS;	// max number of events to accept before setting overflow
	size_t mSequence;			// used to track the insertion order of events into the buffer
	bool mIsOverflown;			// flag that tracks when an overflow occurs

	// vector to hold all selected events until they are cleared or failed back into mEventSet
	typename std::vector< EventType > mSelectedEvents;

	// store to keep and order incoming events
	typename SetType::Type mEventSet;
};

template <class EventType, class SetType>
EventBufferBase <EventType, SetType> :: EventBufferBase(size_t aMaxEvents) :
	M_MAX_EVENTS(aMaxEvents),
	mSequence(0),
	mIsOverflown(false)
{}

template <class EventType, class SetType>
void EventBufferBase<EventType, SetType> :: Update(const typename EventType::MeasType& arVal, PointClass aClass, size_t aIndex)
{
	EventType evt(arVal, aClass, aIndex);
	this->Update(evt, true);

	if(this->NumUnselected() > M_MAX_EVENTS) { //we've overflown and we've got to drop an event
		mIsOverflown = true;
		typename SetType::Type::iterator itr = mEventSet.begin();
		this->mCounter.DecrCount(itr->mClass);
		mEventSet.erase(itr);
	}
}

template <class EventType, class SetType>
void EventBufferBase<EventType, SetType> :: Update(EventType& arEvent, bool aNewValue)
{
	// prevents numerical overflow of the increasing sequence number
	if(this->Size() == 0) mSequence = 0;

	if(aNewValue) arEvent.mSequence = mSequence++;

	this->_Update(arEvent); // call the overridable NVII function
}

template <class EventType, class SetType>
void EventBufferBase<EventType, SetType> :: _Update(const EventType& arEvent)
{
	this->mCounter.IncrCount(arEvent.mClass);
	this->mEventSet.insert(arEvent);
}

template <class EventType, class SetType>
size_t EventBufferBase<EventType, SetType> :: Deselect()
{
	size_t num = mSelectedEvents.size();

	// put selected events back into the event buffer
	for(size_t i = 0; i < num; i++) this->Update(mSelectedEvents[i], false);

	mSelectedEvents.clear();

	return num;
}

template <class EventType, class SetType>
bool EventBufferBase <EventType, SetType> :: IsOverflown()
{
	// if the buffer previously overflowed, but is no longer full, reset the flag
	if(mIsOverflown && this->Size() < M_MAX_EVENTS) mIsOverflown = false;

	return mIsOverflown;
}

template <class EventType, class SetType>
size_t EventBufferBase<EventType, SetType> :: ClearWrittenEvents()
{
	typename std::vector<EventType>::iterator itr = this->mSelectedEvents.begin();

	size_t num = 0;
	while(itr != this->mSelectedEvents.end() && itr->mWritten) {
		++itr;
		++num;
	}

	this->mSelectedEvents.erase(this->mSelectedEvents.begin(), itr);
	return num;
}

template <class EventType, class SetType>
typename EvtItr< EventType >::Type EventBufferBase<EventType, SetType> :: Begin()
{
	return mSelectedEvents.begin();
}

template <class EventType, class SetType>
size_t EventBufferBase <EventType, SetType> :: Select(PointClass aClass, size_t aMaxEvent)
{
	typename SetType::Type::iterator i = mEventSet.begin();

	size_t count = 0;

	while( i != mEventSet.end() && count < aMaxEvent) {
		if( ( i->mClass & aClass) != 0 ) {
			mCounter.DecrCount(i->mClass);
			mSelectedEvents.push_back(*i);
			mEventSet.erase(i++);
			++count;
			mSelectedEvents.back().mWritten = false;
		}
		else ++i;
	}

	return count;
}

}
} //end NS

/* vim: set ts=4 sw=4: */

#endif
