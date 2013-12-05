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

#include <openpal/Exception.h>
#include <openpal/Location.h>

using namespace openpal;

namespace opendnp3
{

SlaveEventBuffer::SlaveEventBuffer(const EventMaxConfig& arEventMaxConfig) :
	mBinaryEvents(arEventMaxConfig.mMaxBinaryEvents),
	mAnalogEvents(arEventMaxConfig.mMaxAnalogEvents),
	mCounterEvents(arEventMaxConfig.mMaxCounterEvents),
	mVtoEvents(arEventMaxConfig.mMaxVtoEvents)
{}

void SlaveEventBuffer::Update(const Binary& arEvent, PointClass aClass, size_t aIndex)
{
	mBinaryEvents.Update(arEvent, aClass, aIndex);
}

void SlaveEventBuffer::Update(const Analog& arEvent, PointClass aClass, size_t aIndex)
{
	mAnalogEvents.Update(arEvent, aClass, aIndex);
}

void SlaveEventBuffer::Update(const Counter& arEvent, PointClass aClass, size_t aIndex)
{
	mCounterEvents.Update(arEvent, aClass, aIndex);
}

void SlaveEventBuffer::Update(const VtoData& arEvent, PointClass aClass, size_t aIndex)
{
	mVtoEvents.Update(arEvent, aClass, aIndex);
}

size_t SlaveEventBuffer::NumVtoEventsAvailable()
{
	return mVtoEvents.NumAvailable();
}

size_t SlaveEventBuffer::NumSelected(BufferTypes aType)
{
	switch(aType) {
	case BT_BINARY:
		return mBinaryEvents.NumSelected();
	case BT_ANALOG:
		return mAnalogEvents.NumSelected();
	case BT_COUNTER:
		return mCounterEvents.NumSelected();
	case BT_VTO:
		return mVtoEvents.NumSelected();
	default:
		MACRO_THROW_EXCEPTION(ArgumentException, "Invalid BufferType");
	}
}

size_t SlaveEventBuffer::NumType(BufferTypes aType)
{
	switch(aType) {
	case BT_BINARY:
		return mBinaryEvents.Size();
	case BT_ANALOG:
		return mAnalogEvents.Size();
	case BT_COUNTER:
		return mCounterEvents.Size();
	case BT_VTO:
		return mVtoEvents.Size();
	default:
		MACRO_THROW_EXCEPTION(ArgumentException, "Invalid BufferType");
	}
}

size_t SlaveEventBuffer::NumSelected()
{
	size_t num = 0;

	num += mBinaryEvents.NumSelected();
	num += mAnalogEvents.NumSelected();
	num += mCounterEvents.NumSelected();
	num += mVtoEvents.NumSelected();

	return num;
}

bool SlaveEventBuffer::IsOverflow()
{
	return	mBinaryEvents.IsOverflown()
	        || mAnalogEvents.IsOverflown()
	        || mCounterEvents.IsOverflown()
	        || mVtoEvents.IsOverflown();
}

bool SlaveEventBuffer::HasEventData()
{
	return mBinaryEvents.NumUnselected() > 0
	       || mAnalogEvents.NumUnselected() > 0
	       || mCounterEvents.NumUnselected() > 0
	       || mVtoEvents.NumUnselected() > 0;
}

bool SlaveEventBuffer::HasClassData(PointClass aClass)
{
	return mBinaryEvents.HasClassData(aClass)
	       || mAnalogEvents.HasClassData(aClass)
	       || mCounterEvents.HasClassData(aClass)
	       || mVtoEvents.HasClassData(aClass);
}

size_t SlaveEventBuffer::Select(BufferTypes aType, PointClass aClass, size_t aMaxEvent)
{
	switch(aType) {
	case BT_BINARY:
		return mBinaryEvents.Select(aClass, aMaxEvent);
	case BT_ANALOG:
		return mAnalogEvents.Select(aClass, aMaxEvent);
	case BT_COUNTER:
		return mCounterEvents.Select(aClass, aMaxEvent);
	case BT_VTO:
		return mVtoEvents.Select(aClass, aMaxEvent);
	default:
		MACRO_THROW_EXCEPTION(ArgumentException, "Invalid BufferType");
	}
}

size_t SlaveEventBuffer::Select(PointClass aClass, size_t aMaxEvent)
{
	size_t left = aMaxEvent;

	/*
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
	if (left > 0) left -= mVtoEvents.Select(aClass, left);

	return aMaxEvent - left;
}

size_t SlaveEventBuffer::ClearWritten()
{
	size_t sum = 0;
	sum += mBinaryEvents.ClearWrittenEvents();
	sum += mAnalogEvents.ClearWrittenEvents();
	sum += mCounterEvents.ClearWrittenEvents();
	sum += mVtoEvents.ClearWrittenEvents();
	return sum;
}

size_t SlaveEventBuffer::Deselect()
{
	size_t sum = 0;
	sum += mBinaryEvents.Deselect();
	sum += mAnalogEvents.Deselect();
	sum += mCounterEvents.Deselect();
	sum += mVtoEvents.Deselect();
	return sum;
}

bool SlaveEventBuffer::IsFull(BufferTypes aType)
{
	switch (aType) {
	case BT_BINARY:
		return mBinaryEvents.IsFull();
	case BT_ANALOG:
		return mAnalogEvents.IsFull();
	case BT_COUNTER:
		return mCounterEvents.IsFull();
	case BT_VTO:
		return mVtoEvents.IsFull();
	default:
		MACRO_THROW_EXCEPTION(ArgumentException, "Invalid BufferType");
	}
}

} //end NS


/* vim: set ts=4 sw=4: */
