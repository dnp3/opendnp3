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
		case(GroupVariation::Group2Var0) :		
			return this->SelectByType<Binary>(count);			
		case(GroupVariation::Group2Var1) :
			return this->SelectByType<Binary>(count, EventBinaryVariation::Group2Var1);			
		case(GroupVariation::Group2Var2) :
			return this->SelectByType<Binary>(count, EventBinaryVariation::Group2Var2);			
		case(GroupVariation::Group2Var3) :
			return this->SelectByType<Binary>(count, EventBinaryVariation::Group2Var3);
			
		case(GroupVariation::Group4Var0) :
			return this->SelectByType<DoubleBitBinary>(count);			
		case(GroupVariation::Group4Var1) :
			return this->SelectByType<DoubleBitBinary>(count, EventDoubleBinaryVariation::Group4Var1);			
		case(GroupVariation::Group4Var2) :
			return this->SelectByType<DoubleBitBinary>(count, EventDoubleBinaryVariation::Group4Var2);			
		case(GroupVariation::Group4Var3) :
			return this->SelectByType<DoubleBitBinary>(count, EventDoubleBinaryVariation::Group4Var3);			

		case(GroupVariation::Group11Var0) :
			return this->SelectByType<BinaryOutputStatus>(count);			
		case(GroupVariation::Group11Var1) :
			return this->SelectByType<BinaryOutputStatus>(count, EventBinaryOutputStatusVariation::Group11Var1);			
		case(GroupVariation::Group11Var2) :
			return this->SelectByType<BinaryOutputStatus>(count, EventBinaryOutputStatusVariation::Group11Var2);			
		
		case(GroupVariation::Group22Var0) :
			return this->SelectByType<Counter>(count);			
		case(GroupVariation::Group22Var1) :
			return this->SelectByType<Counter>(count, EventCounterVariation::Group22Var1);			
		case(GroupVariation::Group22Var2) :
			return this->SelectByType<Counter>(count, EventCounterVariation::Group22Var2);			
		case(GroupVariation::Group22Var5) :
			return this->SelectByType<Counter>(count, EventCounterVariation::Group22Var5);			
		case(GroupVariation::Group22Var6) :
			return this->SelectByType<Counter>(count, EventCounterVariation::Group22Var6);			

		case(GroupVariation::Group23Var0) :
			return this->SelectByType<FrozenCounter>(count);			
		case(GroupVariation::Group23Var1) :
			return this->SelectByType<FrozenCounter>(count, EventFrozenCounterVariation::Group23Var1);			
		case(GroupVariation::Group23Var2) :
			return this->SelectByType<FrozenCounter>(count, EventFrozenCounterVariation::Group23Var2);			
		case(GroupVariation::Group23Var5) :
			return this->SelectByType<FrozenCounter>(count, EventFrozenCounterVariation::Group23Var5);			
		case(GroupVariation::Group23Var6) :
			return this->SelectByType<FrozenCounter>(count, EventFrozenCounterVariation::Group23Var6);			
			
		case(GroupVariation::Group32Var0) :
			return this->SelectByType<Analog>(count);			
		case(GroupVariation::Group32Var1) :
			return this->SelectByType<Analog>(count, EventAnalogVariation::Group32Var1);
		case(GroupVariation::Group32Var2) :
			return this->SelectByType<Analog>(count, EventAnalogVariation::Group32Var2);
		case(GroupVariation::Group32Var3) :
			return this->SelectByType<Analog>(count, EventAnalogVariation::Group32Var3);
		case(GroupVariation::Group32Var4) :
			return this->SelectByType<Analog>(count, EventAnalogVariation::Group32Var4);
		case(GroupVariation::Group32Var5) :
			return this->SelectByType<Analog>(count, EventAnalogVariation::Group32Var5);
		case(GroupVariation::Group32Var6) :
			return this->SelectByType<Analog>(count, EventAnalogVariation::Group32Var6);
		case(GroupVariation::Group32Var7) :
			return this->SelectByType<Analog>(count, EventAnalogVariation::Group32Var7);
		case(GroupVariation::Group32Var8) :
			return this->SelectByType<Analog>(count, EventAnalogVariation::Group32Var8);

		case(GroupVariation::Group42Var0) :
			return this->SelectByType<AnalogOutputStatus>(count);
		case(GroupVariation::Group42Var1) :
			return this->SelectByType<AnalogOutputStatus>(count, EventAnalogOutputStatusVariation::Group42Var1);
		case(GroupVariation::Group42Var2) :
			return this->SelectByType<AnalogOutputStatus>(count, EventAnalogOutputStatusVariation::Group42Var2);
		case(GroupVariation::Group42Var3) :
			return this->SelectByType<AnalogOutputStatus>(count, EventAnalogOutputStatusVariation::Group42Var3);
		case(GroupVariation::Group42Var4) :
			return this->SelectByType<AnalogOutputStatus>(count, EventAnalogOutputStatusVariation::Group42Var4);
		case(GroupVariation::Group42Var5) :
			return this->SelectByType<AnalogOutputStatus>(count, EventAnalogOutputStatusVariation::Group42Var5);
		case(GroupVariation::Group42Var6) :
			return this->SelectByType<AnalogOutputStatus>(count, EventAnalogOutputStatusVariation::Group42Var6);
		case(GroupVariation::Group42Var7) :
			return this->SelectByType<AnalogOutputStatus>(count, EventAnalogOutputStatusVariation::Group42Var7);
		case(GroupVariation::Group42Var8) :
			return this->SelectByType<AnalogOutputStatus>(count, EventAnalogOutputStatusVariation::Group42Var8);


		case(GroupVariation::Group60Var2) :
			return this->SelectByClass(EventClass::EC1, count);			
		case(GroupVariation::Group60Var3):
			return this->SelectByClass(EventClass::EC2, count);			
		case(GroupVariation::Group60Var4):
			return this->SelectByClass(EventClass::EC3, count);			
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

IINField EventBuffer::SelectByClass(EventClass ec, uint32_t max)
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

	return IINField();
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
