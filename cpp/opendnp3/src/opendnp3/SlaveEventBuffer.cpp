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
#include "SlaveEventBuffer.h"

using namespace openpal;

namespace opendnp3
{

SlaveEventBuffer::SlaveEventBuffer(const EventMaxConfig& arEventMaxConfig) :
	mBinaryEvents(arEventMaxConfig.mMaxBinaryEvents),
	mAnalogEvents(arEventMaxConfig.mMaxAnalogEvents),
	mCounterEvents(arEventMaxConfig.mMaxCounterEvents)	
{}

void SlaveEventBuffer::Update(const Binary& aEvent, PointClass aClass, uint32_t aIndex)
{
	Event<Binary> e(aEvent, aIndex, aClass);
	mBinaryEvents.Update(e);
}

void SlaveEventBuffer::Update(const Analog& aEvent, PointClass aClass, uint32_t aIndex)
{
	Event<Analog> e(aEvent, aIndex, aClass);
	mAnalogEvents.Update(e);
}

void SlaveEventBuffer::Update(const Counter& aEvent, PointClass aClass, uint32_t aIndex)
{
	Event<Counter> e(aEvent, aIndex, aClass);
	mCounterEvents.Update(e);
}

size_t SlaveEventBuffer::NumSelected(BufferType aType)
{
	switch(aType) {
		case BufferType::BINARY:
			return mBinaryEvents.NumSelected();
		case BufferType::ANALOG:
			return mAnalogEvents.NumSelected();
		case BufferType::COUNTER:
			return mCounterEvents.NumSelected();
		default:
			return 0;
	}
}

size_t SlaveEventBuffer::NumType(BufferType aType)
{
	switch(aType) {
		case BufferType::BINARY:
			return mBinaryEvents.Size();
		case BufferType::ANALOG:
			return mAnalogEvents.Size();
		case BufferType::COUNTER:
			return mCounterEvents.Size();
		default:
			return 0;
	}
}

size_t SlaveEventBuffer::NumSelected()
{
	size_t num = 0;

	num += mBinaryEvents.NumSelected();
	num += mAnalogEvents.NumSelected();
	num += mCounterEvents.NumSelected();

	return num;
}

bool SlaveEventBuffer::IsOverflow()
{
	return	mBinaryEvents.IsOverflown()
	        || mAnalogEvents.IsOverflown()
	        || mCounterEvents.IsOverflown();	        
}

bool SlaveEventBuffer::HasEventData()
{
	return mBinaryEvents.NumUnselected() > 0
	       || mAnalogEvents.NumUnselected() > 0
	       || mCounterEvents.NumUnselected() > 0;	       
}

bool SlaveEventBuffer::HasClassData(PointClass aClass)
{
	return mBinaryEvents.HasClassData(aClass)
	       || mAnalogEvents.HasClassData(aClass)
	       || mCounterEvents.HasClassData(aClass);	       
}

size_t SlaveEventBuffer::Select(BufferType aType, PointClass aClass, size_t aMaxEvent)
{
	switch(aType) {
	case BufferType::BINARY:
		return mBinaryEvents.Select(aClass, aMaxEvent);
	case BufferType::ANALOG:
		return mAnalogEvents.Select(aClass, aMaxEvent);
	case BufferType::COUNTER:
		return mCounterEvents.Select(aClass, aMaxEvent);
	default:
		return 0;
	}
}

size_t SlaveEventBuffer::Select(PointClass aClass, size_t aMaxEvent)
{
	size_t left = aMaxEvent;

	/* TODO - examine how this is used.
	 * ONLY USED IN TESTS!
	 * The following implies a natural order of importance.  From 'most
	 * important' to 'least important', the order is:
	 *
	 *    1. Binary
	 *    2. Analog
	 *    3. Counter
	 *    4. Virtual Terminal
	 */
	if (left > 0) left -= mBinaryEvents.Select(aClass, left);
	if (left > 0) left -= mAnalogEvents.Select(aClass, left);
	if (left > 0) left -= mCounterEvents.Select(aClass, left);	

	return aMaxEvent - left;
}

size_t SlaveEventBuffer::ClearWritten()
{
	size_t sum = 0;
	sum += mBinaryEvents.ClearWrittenEvents();
	sum += mAnalogEvents.ClearWrittenEvents();
	sum += mCounterEvents.ClearWrittenEvents();	
	return sum;
}

size_t SlaveEventBuffer::Deselect()
{
	size_t sum = 0;
	sum += mBinaryEvents.Deselect();
	sum += mAnalogEvents.Deselect();
	sum += mCounterEvents.Deselect();	
	return sum;
}

bool SlaveEventBuffer::IsFull(BufferType aType)
{
	switch (aType) {
		case BufferType::BINARY:
			return mBinaryEvents.IsFull();
		case BufferType::ANALOG:
			return mAnalogEvents.IsFull();
		case BufferType::COUNTER:
			return mCounterEvents.IsFull();
		default:
			return false;
	}
}

} //end NS


/* vim: set ts=4 sw=4: */
