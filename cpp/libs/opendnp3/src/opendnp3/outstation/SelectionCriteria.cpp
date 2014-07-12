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

SelectionCriteria::SelectionCriteria() : numClass1(0), numClass2(0), numClass3(0)
{

}

SelectionCriteria::SelectionCriteria(const ClassField& field) 	
{
	
}

IINField SelectionCriteria::RecordAllObjects(GroupVariation enumeration)
{
	switch (enumeration)
	{
		case(GroupVariation::Group60Var2):
			return RecordClass(numClass1);

		case(GroupVariation::Group60Var3):
			return RecordClass(numClass2);

		case(GroupVariation::Group60Var4):
			return RecordClass(numClass3);

		default:
			return IINField(IINBit::FUNC_NOT_SUPPORTED);
	}
}

IINField SelectionCriteria::RecordClass(uint32_t& count)
{
	return RecordClass(count, openpal::MaxValue<uint32_t>());
}

IINField SelectionCriteria::RecordClass(uint32_t& count, uint32_t value)
{
	if (count > 0)
	{
		return IINField(IINBit::PARAM_ERROR);
	}
	else
	{
		count = value;
		return IINField::Empty;
	}
}

void SelectionCriteria::Clear()
{
	numClass1 = numClass2 = numClass3 = 0;
}

bool SelectionCriteria::HasSelection() const
{
	return (numClass1 | numClass2 | numClass3) > 0;
}

void SelectionCriteria::RecordAsWritten(EventClass clazz, EventType type)
{
	switch (clazz)
	{
		case(EventClass::EC1) :
			--numClass1;
			break;
		case(EventClass::EC2) :
			--numClass2;
			break;
		case(EventClass::EC3) :
			--numClass3;
			break;	
	}
}

EventWriteOperation SelectionCriteria::GetWriteOperationFor(const EventResponseConfig& defaults, EventClass clazz, EventType type)
{
	EventWriteLimits limits;
	limits.numClass1 = numClass1;
	limits.numClass2 = numClass2;
	limits.numClass3 = numClass3;
	limits.numOfType = openpal::MaxValue<uint32_t>();

	auto pFunction = GetDefaultWriteFunction(defaults, type);

	switch(clazz)
	{
		case(EventClass::EC1) :
			return (numClass1 > 0) ? EventWriteOperation(pFunction, limits) : EventWriteOperation();
		case(EventClass::EC2):
			return (numClass2 > 0) ? EventWriteOperation(pFunction, limits) : EventWriteOperation();
		case(EventClass::EC3):
			return (numClass3 > 0) ? EventWriteOperation(pFunction, limits) : EventWriteOperation();
		default:
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
