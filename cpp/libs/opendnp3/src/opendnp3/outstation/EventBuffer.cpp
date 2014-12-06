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

#include "EventBuffer.h"

namespace opendnp3
{

EventBuffer::EventBuffer(const EventBufferConfig& config_) :
	overflow(false),
	config(config_),
	events(config_.TotalEvents())	
{

}

void EventBuffer::Unselect()
{
	auto unselect = [this](SOERecord& record) 
	{
		if (record.selected)
		{			
			selectedCounts.Decrement(record.clazz, record.type);
			record.selected = false;
		}

		if (record.written)
		{
			writtenCounts.Decrement(record.clazz, record.type);
			record.written = false;
		}

		return this->selectedCounts.TotatCount() > 0;
	};

	events.While(unselect);
}

IINField EventBuffer::SelectAll(GroupVariation gv)
{
	return SelectCount(gv, openpal::MaxValue<uint32_t>());
}

IINField EventBuffer::SelectCount(GroupVariation gv, uint16_t count)
{
	switch (gv)
	{
		case(GroupVariation::Group60Var2) :
			this->SelectByClass(EventClass::EC1, count);
			return IINField();
		case(GroupVariation::Group60Var3):
			this->SelectByClass(EventClass::EC2, count);
			return IINField();
		case(GroupVariation::Group60Var4):
			this->SelectByClass(EventClass::EC3, count);
			return IINField();
		default:
			return IINBit::FUNC_NOT_SUPPORTED;
	}
}

ClassField EventBuffer::UnwrittenClassField() const
{
	return totalCounts.Subtract(writtenCounts);
}

bool EventBuffer::IsOverflown()
{
	if (overflow && HasEnoughSpaceToClearOverflow())
	{
		overflow = false;
	}

	return overflow;
}

uint32_t EventBuffer::SelectByClass(EventClass ec, uint32_t max)
{
	uint32_t num = 0;
	auto iter = events.Iterate();
	const uint32_t remaining = totalCounts.NumOfClass(ec) - selectedCounts.NumOfClass(ec);

	while (iter.HasNext() && (num < remaining) && (num < max))
	{
		auto pNode = iter.Next();
		if (pNode->value.clazz == ec)
		{
			pNode->value.SelectDefault();			
			selectedCounts.Increment(pNode->value.clazz, pNode->value.type);
			++num;
		}
	}	

	return num;
}

void EventBuffer::RemoveFromCounts(const SOERecord& record)
{
	totalCounts.Decrement(record.clazz, record.type);

	if (record.selected)
	{
		selectedCounts.Decrement(record.clazz, record.type);
	}

	if (record.written)
	{
		writtenCounts.Decrement(record.clazz, record.type);
	}
}

bool EventBuffer::RemoveOldestEventOfType(EventType type)
{
	// find the first event of this type in the SOE, and discard it
	auto isMatch = [type](const SOERecord& rec) { return rec.type == type; };
	auto pNode = events.RemoveFirst(isMatch);

	if (pNode)
	{		
		this->RemoveFromCounts(pNode->value);
		pNode->value.Reset();	
		return true;
	}
	else
	{
		return false;
	}
}

void EventBuffer::ClearWritten()
{
	auto written = [this](const SOERecord& record) 
	{ 
		if (record.written)
		{
			this->RemoveFromCounts(record);
			return true;
		}
		else
		{
			return false;
		}		
	};	

	events.RemoveAll(written);
}

bool EventBuffer::IsTypeOverflown(EventType type) const
{	
	auto max = config.GetMaxEventsForType(type);	
	return  (max > 0) ? (totalCounts.NumOfType(type) >= max) : false;	
}

bool EventBuffer::IsAnyTypeOverflown() const
{
	return	IsTypeOverflown(EventType::Binary) ||
			IsTypeOverflown(EventType::DoubleBitBinary) ||
			IsTypeOverflown(EventType::BinaryOutputStatus) ||
			IsTypeOverflown(EventType::Counter) ||
			IsTypeOverflown(EventType::FrozenCounter) ||
			IsTypeOverflown(EventType::Analog) ||
			IsTypeOverflown(EventType::AnalogOutputStatus);
}

bool EventBuffer::HasEnoughSpaceToClearOverflow() const
{	
	return !(events.IsFull() || IsAnyTypeOverflown());	
}

}
