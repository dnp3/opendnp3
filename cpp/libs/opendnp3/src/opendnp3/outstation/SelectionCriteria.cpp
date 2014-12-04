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
	auto max = openpal::MaxValue<uint32_t>();

	if (field.HasClass1())
	{
		this->RecordClass(EventClass::EC1, max);
	}
	
	if (field.HasClass2())
	{
		this->RecordClass(EventClass::EC2, max);
	}

	if (field.HasClass3())
	{
		this->RecordClass(EventClass::EC3, max);
	}
}

IINField SelectionCriteria::RecordAllObjects(GroupVariation enumeration)
{
	return RecordCountOfObjects(enumeration, openpal::MaxValue<uint32_t>());
}

IINField SelectionCriteria::RecordCountOfObjects(GroupVariation enumeration, uint32_t count)
{
	switch (enumeration)
	{
		/// --- class polls ------

		case(GroupVariation::Group60Var2):
			return RecordClass(EventClass::EC1, count);

		case(GroupVariation::Group60Var3):
			return RecordClass(EventClass::EC2, count);

		case(GroupVariation::Group60Var4):
			return RecordClass(EventClass::EC3, count);


		/// --- defaults ------
		
		case(GroupVariation::Group2Var0) :
			return RecordType<Binary>(defaults.binary, count);
		case(GroupVariation::Group4Var0) :
			return RecordType<DoubleBitBinary>(defaults.doubleBinary, count);
		case(GroupVariation::Group11Var0) :
			return RecordType<BinaryOutputStatus>(defaults.binaryOutputStatus, count);
		case(GroupVariation::Group22Var0) :
			return RecordType<Counter>(defaults.counter, count);
		case(GroupVariation::Group23Var0) :
			return RecordType<FrozenCounter>(defaults.frozenCounter, count);
		case(GroupVariation::Group32Var0) :
			return RecordType<Analog>(defaults.analog, count);
		case(GroupVariation::Group42Var0):
			return RecordType<AnalogOutputStatus>(defaults.analogOutputStatus, count);

		/// --- specific binary variations ------

		case(GroupVariation::Group2Var1) :
			return RecordType<Binary>(EventBinaryVariation::Group2Var1, count);
		case(GroupVariation::Group2Var2) :
			return RecordType<Binary>(EventBinaryVariation::Group2Var2, count);
		case(GroupVariation::Group2Var3) :
			return RecordType<Binary>(EventBinaryVariation::Group2Var3, count);

		/// --- specific double binary variations ------
		
		case(GroupVariation::Group4Var1) :
			return RecordType<DoubleBitBinary>(EventDoubleBinaryVariation::Group4Var1, count);
		case(GroupVariation::Group4Var2) :
			return RecordType<DoubleBitBinary>(EventDoubleBinaryVariation::Group4Var2, count);
		case(GroupVariation::Group4Var3) :
			return RecordType<DoubleBitBinary>(EventDoubleBinaryVariation::Group4Var3, count);

		/// --- specific binary output status variations ------

		case(GroupVariation::Group11Var1) :
			return RecordType<BinaryOutputStatus>(EventBinaryOutputStatusVariation::Group11Var1, count);
		case(GroupVariation::Group11Var2) :
			return RecordType<BinaryOutputStatus>(EventBinaryOutputStatusVariation::Group11Var2, count);

		/// --- specific counter variations ------

		case(GroupVariation::Group22Var1) :
			return RecordType<Counter>(EventCounterVariation::Group22Var1, count);
		case(GroupVariation::Group22Var2) :
			return RecordType<Counter>(EventCounterVariation::Group22Var2, count);
		case(GroupVariation::Group22Var5) :
			return RecordType<Counter>(EventCounterVariation::Group22Var5, count);
		case(GroupVariation::Group22Var6) :
			return RecordType<Counter>(EventCounterVariation::Group22Var6, count);

		/// --- specific frozen counter variations ------
		
		case(GroupVariation::Group23Var1) :
			return RecordType<FrozenCounter>(EventFrozenCounterVariation::Group23Var1, count);
		case(GroupVariation::Group23Var2) :
			return RecordType<FrozenCounter>(EventFrozenCounterVariation::Group23Var2, count);
		case(GroupVariation::Group23Var5) :
			return RecordType<FrozenCounter>(EventFrozenCounterVariation::Group23Var5, count);
		case(GroupVariation::Group23Var6) :
			return RecordType<FrozenCounter>(EventFrozenCounterVariation::Group23Var6, count);

		/// --- specific analog variations ------
		
		case(GroupVariation::Group32Var1) :
			return RecordType<Analog>(EventAnalogVariation::Group32Var1, count);
		case(GroupVariation::Group32Var2) :
			return RecordType<Analog>(EventAnalogVariation::Group32Var2, count);
		case(GroupVariation::Group32Var3) :
			return RecordType<Analog>(EventAnalogVariation::Group32Var3, count);
		case(GroupVariation::Group32Var4) :
			return RecordType<Analog>(EventAnalogVariation::Group32Var4, count);
		case(GroupVariation::Group32Var5) :
			return RecordType<Analog>(EventAnalogVariation::Group32Var5, count);
		case(GroupVariation::Group32Var6) :
			return RecordType<Analog>(EventAnalogVariation::Group32Var6, count);
		case(GroupVariation::Group32Var7) :
			return RecordType<Analog>(EventAnalogVariation::Group32Var7, count);
		case(GroupVariation::Group32Var8) :
			return RecordType<Analog>(EventAnalogVariation::Group32Var8, count);

		/// --- specific analog output status variations ------

		case(GroupVariation::Group42Var1) :
			return RecordType<AnalogOutputStatus>(EventAnalogOutputStatusVariation::Group42Var1, count);
		case(GroupVariation::Group42Var2) :
			return RecordType<AnalogOutputStatus>(EventAnalogOutputStatusVariation::Group42Var2, count);
		case(GroupVariation::Group42Var3) :
			return RecordType<AnalogOutputStatus>(EventAnalogOutputStatusVariation::Group42Var3, count);
		case(GroupVariation::Group42Var4) :
			return RecordType<AnalogOutputStatus>(EventAnalogOutputStatusVariation::Group42Var4, count);
		case(GroupVariation::Group42Var5) :
			return RecordType<AnalogOutputStatus>(EventAnalogOutputStatusVariation::Group42Var5, count);
		case(GroupVariation::Group42Var6) :
			return RecordType<AnalogOutputStatus>(EventAnalogOutputStatusVariation::Group42Var6, count);
		case(GroupVariation::Group42Var7) :
			return RecordType<AnalogOutputStatus>(EventAnalogOutputStatusVariation::Group42Var7, count);
		case(GroupVariation::Group42Var8) :
			return RecordType<AnalogOutputStatus>(EventAnalogOutputStatusVariation::Group42Var8, count);

		default:
			return IINField(IINBit::FUNC_NOT_SUPPORTED);
	}
}

IINField SelectionCriteria::RecordEventType(EventType type, EventHeaderWriteFunc function, uint32_t count)
{
	if (mode == Mode::TYPE_BASED || mode == Mode::UNDEFINED)
	{		
		auto& record = typeSelections[static_cast<int>(type)];

		if (record.IsDefined())
		{
			mode = Mode::FAILED;
			return IINField(IINBit::PARAM_ERROR);
		}
		else
		{
			mode = Mode::TYPE_BASED;
			record = TypeSelection(count, function);
			return IINField::Empty();
		}
	}
	else
	{
		mode = Mode::FAILED;
		return IINField(IINBit::PARAM_ERROR);
	}
}

IINField SelectionCriteria::RecordClass(EventClass ec, uint32_t value)
{
	auto& count = classCounts[static_cast<int>(ec)];

	if (mode == Mode::CLASS_BASED || mode == Mode::UNDEFINED)
	{
		if (count > 0)
		{
			mode = Mode::FAILED;
			return IINField(IINBit::PARAM_ERROR);
		}
		else
		{
			mode = Mode::CLASS_BASED;
			count = value;			
			return IINField::Empty();
		}		
	}
	else
	{
		mode = Mode::FAILED;
		return IINField(IINBit::PARAM_ERROR);
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

