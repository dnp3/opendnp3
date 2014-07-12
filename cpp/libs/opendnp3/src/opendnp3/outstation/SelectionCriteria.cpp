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

#include "SelectionCriteria.h"

#include "opendnp3/outstation/EventResponseTypes.h"

using namespace openpal;

namespace opendnp3
{

SelectionCriteria::TypeSelection::TypeSelection() : maximum(0), function(nullptr)
{}

SelectionCriteria::TypeSelection::TypeSelection(uint32_t maximum_, EventHeaderWriteFunc function_) : maximum(maximum_), function(function_)
{}

bool SelectionCriteria::TypeSelection::IsDefined() const
{
	return function && (maximum > 0);
}

void SelectionCriteria::TypeSelection::Clear()
{
	maximum = 0;
	function = nullptr;
}

SelectionCriteria::SelectionCriteria(const EventResponseConfig& config) : mode(Mode::UNDEFINED), defaults(config)
{
	this->Clear();	
}

void SelectionCriteria::RecordViaClassField(const ClassField& field)
{	
	if (field.HasClass1())
	{
		this->RecordClassMaxValue(EventClass::EC1);
	}
	
	if (field.HasClass2())
	{
		this->RecordClassMaxValue(EventClass::EC2);
	}

	if (field.HasClass3())
	{
		this->RecordClassMaxValue(EventClass::EC3);
	}
}

IINField SelectionCriteria::RecordAllObjects(GroupVariation enumeration)
{
	switch (enumeration)
	{
		case(GroupVariation::Group60Var2):
			return RecordClassMaxValue(EventClass::EC1);

		case(GroupVariation::Group60Var3):
			return RecordClassMaxValue(EventClass::EC2);

		case(GroupVariation::Group60Var4):
			return RecordClassMaxValue(EventClass::EC3);
			
		case(GroupVariation::Group2Var0) :
			return RecordTypeMaxValue(EventType::Binary, GetDefaultWriteFunction(defaults, EventType::Binary));

		case(GroupVariation::Group22Var0) :
			return RecordTypeMaxValue(EventType::Counter, GetDefaultWriteFunction(defaults, EventType::Counter));

		case(GroupVariation::Group32Var0) :
			return RecordTypeMaxValue(EventType::Analog, GetDefaultWriteFunction(defaults, EventType::Analog));

		default:
			return IINField(IINBit::FUNC_NOT_SUPPORTED);
	}
}

IINField SelectionCriteria::RecordTypeMaxValue(EventType type, EventHeaderWriteFunc function)
{
	if (mode == Mode::CLASS_BASED)
	{
		return IINField(IINBit::PARAM_ERROR);
	}
	else
	{
		mode = Mode::TYPE_BASED;
		auto& record = typeSelections[static_cast<int>(type)];
		if (record.IsDefined())
		{
			return IINField(IINBit::PARAM_ERROR);
		}
		else
		{
			record = TypeSelection(MaxValue<uint32_t>(), function);
			return IINField::Empty;
		}
	}
}

IINField SelectionCriteria::RecordClassMaxValue(EventClass ec)
{
	return RecordClass(ec, openpal::MaxValue<uint32_t>());
}

IINField SelectionCriteria::RecordClass(EventClass ec, uint32_t value)
{
	auto& count = classCounts[static_cast<int>(ec)];

	if (mode == Mode::TYPE_BASED)
	{
		return IINField(IINBit::PARAM_ERROR);
	}
	else
	{
		if (count > 0)
		{
			return IINField(IINBit::PARAM_ERROR);
		}
		else
		{
			count = value;
			mode = Mode::CLASS_BASED;
			return IINField::Empty;
		}		
	}	
}

void SelectionCriteria::Clear()
{
	mode = Mode::UNDEFINED;
	this->AssignAllClassCounts(0);
	this->ClearAllTypeSections();
}

void SelectionCriteria::AssignAllClassCounts(uint32_t count)
{
	classCounts[0] = classCounts[1] = classCounts[2] = count;	
}

void SelectionCriteria::ClearAllTypeSections()
{
	for (int i = 0; i < opendnp3::NUM_OUTSTATION_EVENT_TYPES; ++i)
	{
		typeSelections[i].Clear();
	}
}


bool SelectionCriteria::HasSelection() const
{
	return mode != Mode::UNDEFINED;	
}

void SelectionCriteria::RecordAsWritten(EventClass clazz, EventType type)
{
	switch (mode)
	{
		case(Mode::CLASS_BASED) :
			--classCounts[static_cast<int>(clazz)];
			break;
		case(Mode::TYPE_BASED) :
			--typeSelections[static_cast<int>(type)].maximum;
			break;
		default:
			break;
	}	
}

EventWriteOperation SelectionCriteria::GetWriteOperationFor(EventClass clazz, EventType type)
{	
	switch (mode)
	{
		case(Mode::CLASS_BASED) :
			return GetClassBasedWriteOperationFor(clazz, type);
		case(Mode::TYPE_BASED) :
			return GetTypedBasedWriteOperationFor(clazz, type);
		default:
			return EventWriteOperation();
	}
}

EventWriteOperation SelectionCriteria::GetTypedBasedWriteOperationFor(EventClass clazz, EventType type)
{
	auto& record = typeSelections[static_cast<int>(type)];
	if (record.IsDefined())
	{
		EventWriteLimits limits;
		limits.numClass1 = MaxValue<uint32_t>();
		limits.numClass2 = MaxValue<uint32_t>();
		limits.numClass3 = MaxValue<uint32_t>();
		limits.numOfType = record.maximum;

		return EventWriteOperation(record.function, limits);
	}
	else
	{
		return EventWriteOperation();
	}
}

EventWriteOperation SelectionCriteria::GetClassBasedWriteOperationFor(EventClass clazz, EventType type)
{
	auto countOfType = classCounts[static_cast<int>(clazz)];

	if (countOfType > 0)
	{
		EventWriteLimits limits;
		limits.numClass1 = classCounts[0];
		limits.numClass2 = classCounts[1];
		limits.numClass3 = classCounts[2];
		limits.numOfType = openpal::MaxValue<uint32_t>();

		auto pFunction = GetDefaultWriteFunction(defaults, type);

		return EventWriteOperation(pFunction, limits);
	}
	else
	{
		return EventWriteOperation();
	}
}

EventHeaderWriteFunc SelectionCriteria::GetDefaultWriteFunction(const EventResponseConfig& config, EventType type)
{
	switch (type)
	{
		case(EventType::Binary) :
			return EventResponseTypes::Lookup(config.binary);
		case(EventType::DoubleBitBinary) :
			return EventResponseTypes::Lookup(config.doubleBinary);
		case(EventType::BinaryOutputStatus) :
			return EventResponseTypes::Lookup(config.binaryOutputStatus);
		case(EventType::Counter) :
			return EventResponseTypes::Lookup(config.counter);
		case(EventType::FrozenCounter) :
			return EventResponseTypes::Lookup(config.frozenCounter);
		case(EventType::Analog) :
			return EventResponseTypes::Lookup(config.analog);
		case(EventType::AnalogOutputStatus) :
			return EventResponseTypes::Lookup(config.analogOutputStatus);
		default:
			return nullptr;
	}
}

}
