//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or more
// contributor license agreements. See the NOTICE file distributed with this
// work for additional information regarding copyright ownership.  Green Enery
// Corp licenses this file to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance with the
// License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
// WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
// License for the specific language governing permissions and limitations
// under the License.
//

#ifndef __SLAVE_EVENT_BUFFER_H_
#define __SLAVE_EVENT_BUFFER_H_

#include <APL/DataTypes.h>

#include "BufferTypes.h"
#include "DatabaseInterfaces.h"
#include "DNPDatabaseTypes.h"
#include "EventBuffers.h"
#include "SlaveConfig.h"

namespace apl
{
namespace dnp
{

/**
 * Manager for DNP3 data events composed of per-type event buffer
 * implementations. Events are selected based on classification (data type,
 * class) and then read using an iterator system. The behavior is
 * transactional such that failed deliveries put events back into the buffer.
 *
 * All selections can be limited by a desired event count.
 */
class SlaveEventBuffer : public IEventBuffer
{
	friend class Slave;

public:

	/**
	 * Creates a new SlaveEventBuffer instance based on the provided
	 * configuration parameters.
	 *
	 * @param arEventMaxConfig		the configuration parameters for the
	 * 								SlaveEventBuffer instance
	 *
	 * @return						a new SlaveEventBuffer instance
	 */
	SlaveEventBuffer(const EventMaxConfig& arEventMaxConfig);

	/**
	 * Adds an event to the buffer.
	 *
	 * @param arEvent		Event update to add to the buffer
	 * @param aClass		the class of data for the event
	 * @param aIndex		Index of the measurement
	 */
	void Update(const Binary& arEvent, PointClass aClass, size_t aIndex);

	/**
	 * Adds an event to the buffer.
	 *
	 * @param arEvent		Event update to add to the buffer
	 * @param aClass		the class of data for the event
	 * @param aIndex		Index of the measurement
	 */
	void Update(const Analog& arEvent, PointClass aClass, size_t aIndex);

	/**
	 * Adds an event to the buffer.
	 *
	 * @param arEvent		Event update to add to the buffer
	 * @param aClass		the class of data for the event
	 * @param aIndex		Index of the measurement
	 */
	void Update(const Counter& arEvent, PointClass aClass, size_t aIndex);

	/**
	 * Adds an event to the buffer.
	 *
	 * @param arEvent		Event update to add to the buffer
	 */
	void Update(const VtoData& arEvent, PointClass aClass, size_t aIndex);

	size_t NumVtoEventsAvailable();

	/**
	 * Returns the number of events that were previously selected through
	 * SlaveEventBuffer::Select().  The selection behavior is restricted
	 * only to the buffer type given (BT_BINARY, BT_ANALOG, BT_COUNTER,
	 * BT_VTO, etc.)
	 *
	 * @param aType			the type of buffer from which to choose
	 *
	 * @return				the number of events selected
	 */
	size_t NumSelected(BufferTypes aType);

	/**
	 * Returns the number of events that were previously selected through
	 * SlaveEventBuffer::Select().
	 *
	 * @return				the number of events selected
	 */
	size_t NumSelected();

	/**
	 * Returns the number of events of a given type in the buffer.
	 *
	 * @param aType			the type of buffer from which to choose
	 *
	 * @return				the number of events selected
	 */
	size_t NumType(BufferTypes aType);

	/**
	 * Provides an EvtItr iterator object for accessing the data previously
	 * selected through SlaveEventBuffer::Select().
	 */
	void Begin(BinaryEventIter& arIter) {
		arIter = mBinaryEvents.Begin();
	}

	/**
	 * Provides an EvtItr iterator object for accessing the data previously
	 * selected through SlaveEventBuffer::Select().
	 */
	void Begin(AnalogEventIter& arIter) {
		arIter = mAnalogEvents.Begin();
	}

	/**
	 * Provides an EvtItr iterator object for accessing the data previously
	 * selected through SlaveEventBuffer::Select().
	 */
	void Begin(CounterEventIter& arIter) {
		arIter = mCounterEvents.Begin();
	}

	/**
	 * Provides an EvtItr iterator object for accessing the data previously
	 * selected through SlaveEventBuffer::Select().
	 */
	void Begin(VtoDataEventIter& arIter) {
		arIter = mVtoEvents.Begin();
	}

	/**
	 * Returns a flag to indicate whether the buffer has been overflown.
	 *
	 * @return				true if too much data has been written to the
	 * 						buffer, false if not.
	 */
	bool IsOverflow();

	/**
	 * Returns true if the buffer contains any data matching the given
	 * PointClass.
	 *
	 * @param aClass		the class of data to match
	 *
	 * @return				'true' if the buffer contains matching data,
	 * 						'false' if not
	 */
	bool HasClassData(PointClass aClass);

	/**
	 * Returns 'true' if the buffer has any event data stored or 'false'
	 * if not.
	 *
	 * @return				'true' if the buffer has data, 'false' if not
	 */
	bool HasEventData();

	/**
	 * Selects data in the buffer that matches the given PointClass, up to
	 * the defined number of entries.  The selection behavior is
	 * restricted only to the buffer type given (BT_BINARY, BT_ANALOG,
	 * BT_COUNTER, BT_VTO, etc.)
	 *
	 * @param aType			the type of buffer from which to choose
	 * @param aClass		the class of data to match
	 * @param aMaxEvent		maximum number of events to select
	 *
	 * @return				the number of events selected
	 */
	size_t Select(BufferTypes aType, PointClass aClass, size_t aMaxEvent = std::numeric_limits<size_t>::max());

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
	 * Transfers any selected events back into the buffer.
	 */
	size_t Deselect();

	/**
	 * Remove events that have been written (flagged with 'mWritten=true')
	 * from the selection buffer.
	 */
	size_t ClearWritten();

	/**
	 * Returns 'true' if the specified buffer is full, 'false' if it still
	 * has space.
	 *
	 * @param aType			the type of buffer from which to choose
	 *
	 * @return				'true' if the buffer is full, 'false' if not
	 */
	bool IsFull(BufferTypes aType);

protected:

	/**
	 * Returns a pointer to the buffer for VtoEvent objects.  This is used
	 * by the Slave::OnVtoUpdate() function to copy data from the Slave's
	 * VtoWriter instance into the event buffer.
	 *
	 * @return				a pointer to the VtoEvent buffer
	 */
	InsertionOrderedEventBuffer<VtoEvent>* GetVtoEventBuffer() {
		return &this->mVtoEvents;
	}

private:

	/**
	 * A buffer for binary events that require ordering based on the
	 * time of occurrence.
	 */
	TimeOrderedEventBuffer<BinaryEvent> mBinaryEvents;

	/**
	 * A buffer for analog events that require ordering based on the
	 * time of occurrence.
	 */
	TimeOrderedEventBuffer<AnalogEvent> mAnalogEvents;

	/**
	 * A buffer for counter events where a single event is needed per
	 * point and previous events for that point can be overridden.
	 */
	SingleEventBuffer<CounterEvent> mCounterEvents;

	/**
	 * A buffer for VTO data where the queue-style (FCFS) insertion
	 * order is important.
	 */
	InsertionOrderedEventBuffer<VtoEvent> mVtoEvents;

	/**
	 * TODO - I searched the code and found no uses of the mChange
	 * variable.  Perhaps it is deprecated?
	 */
	bool mChange;
};

}
}

/* vim: set ts=4 sw=4: */

#endif
