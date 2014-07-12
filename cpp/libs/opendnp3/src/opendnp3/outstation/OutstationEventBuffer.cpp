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

#include "OutstationEventBuffer.h"

namespace opendnp3
{

OutstationEventBuffer::OutstationEventBuffer(const EventBufferConfig& config_, const EventBufferFacade& facade_) :
	overflow(false),
	config(config_),
	facade(facade_)
{

}

void OutstationEventBuffer::Reset()
{
	while(facade.selectedEvents.IsNotEmpty())
	{
		auto pNode = facade.selectedEvents.Pop();		
		pNode->value.selected = false;
	}

	selectedTracker.Clear();
}

bool OutstationEventBuffer::IsOverflown()
{
	if (overflow && HasEnoughSpaceToClearOverflow())
	{
		overflow = false;
	}

	return overflow;
}

void OutstationEventBuffer::Clear()
{
	while(facade.selectedEvents.IsNotEmpty())
	{		
		auto pNode = facade.selectedEvents.Pop();
		if (pNode->value.selected) // could have been removed due to buffer overflow
		{			
			facade.sequenceOfEvents.Remove(pNode); // O(1) from SOE
			totalTracker.Decrement(pNode->value.clazz, pNode->value.type);
		}
	}
	
	selectedTracker.Clear();
}

ClassField OutstationEventBuffer::TotalEventMask() const
{
	return totalTracker.ToClassField();
}

ClassField OutstationEventBuffer::UnselectedEventMask() const
{
	return totalTracker.Subtract(selectedTracker);
}

void OutstationEventBuffer::Update(const Event<Binary>& evt)
{
	this->UpdateAny(evt, EventType::Binary);
}

void OutstationEventBuffer::Update(const Event<Analog>& evt)
{
	this->UpdateAny(evt, EventType::Analog);
}

void OutstationEventBuffer::Update(const Event<Counter>& evt)
{
	this->UpdateAny(evt, EventType::Counter);
}

void OutstationEventBuffer::Update(const Event<FrozenCounter>& evt)
{
	this->UpdateAny(evt, EventType::FrozenCounter);
}

void OutstationEventBuffer::Update(const Event<DoubleBitBinary>& evt)
{
	this->UpdateAny(evt, EventType::DoubleBitBinary);
}

void OutstationEventBuffer::Update(const Event<BinaryOutputStatus>& evt)
{
	this->UpdateAny(evt, EventType::BinaryOutputStatus);
}

void OutstationEventBuffer::Update(const Event<AnalogOutputStatus>& evt)
{
	this->UpdateAny(evt, EventType::AnalogOutputStatus);
}

bool OutstationEventBuffer::IsTypeOverflown(EventType type) const
{	
	auto max = config.GetMaxEventsForType(type);

	if (max > 0)
	{
		return  totalTracker.NumOfType(type) < max;
	}
	else
	{
		return false;
	}	
}

bool OutstationEventBuffer::IsAnyTypeOverflown() const
{
	return	IsTypeOverflown(EventType::Binary) ||
			IsTypeOverflown(EventType::DoubleBitBinary) ||
			IsTypeOverflown(EventType::BinaryOutputStatus) ||
			IsTypeOverflown(EventType::Counter) ||
			IsTypeOverflown(EventType::FrozenCounter) ||
			IsTypeOverflown(EventType::Analog) ||
			IsTypeOverflown(EventType::AnalogOutputStatus);
}

bool OutstationEventBuffer::HasEnoughSpaceToClearOverflow() const
{
	auto soeHasSpace = facade.sequenceOfEvents.Size() < facade.sequenceOfEvents.Capacity();

	return soeHasSpace && !IsAnyTypeOverflown();
		
}

SelectionWriter OutstationEventBuffer::Iterate()
{
	return SelectionWriter(*this);
}

}
