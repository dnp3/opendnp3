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
#include "Database.h"

#include <openpal/logging/LogMacros.h>

#include "opendnp3/ErrorCodes.h"

#include <assert.h>

using namespace openpal;

namespace opendnp3
{

Database::Database(const DatabaseTemplate& dbTemplate, IEventReceiver& eventReceiver, IndexMode indexMode_, StaticTypeBitField allowedClass0Types) :
	buffers(dbTemplate, allowedClass0Types, indexMode_),
	pEventReceiver(&eventReceiver),
	indexMode(indexMode_)
{

}

bool Database::Update(const Binary& value, uint16_t index, EventMode mode)
{
	return this->UpdateEvent(value, index, mode);
}

bool Database::Update(const DoubleBitBinary& value, uint16_t index, EventMode mode)
{
	return this->UpdateEvent(value, index, mode);
}

bool Database::Update(const Analog& value, uint16_t index, EventMode mode)
{
	return this->UpdateEvent(value, index, mode);
}

bool Database::Update(const Counter& value, uint16_t index, EventMode mode)
{
	return this->UpdateEvent(value, index, mode);
}

bool Database::Update(const FrozenCounter& value, uint16_t index, EventMode mode)
{
	return this->UpdateEvent(value, index, mode);
}

bool Database::Update(const BinaryOutputStatus& value, uint16_t index, EventMode mode)
{
	return this->UpdateEvent(value, index, mode);
}

bool Database::Update(const AnalogOutputStatus& value, uint16_t index, EventMode mode)
{
	return this->UpdateEvent(value, index, mode);
}

bool Database::Update(const TimeAndInterval& value, uint16_t index)
{		
	auto rawIndex = GetRawIndex<TimeAndInterval>(index);
	auto view = buffers.buffers.GetArrayView<TimeAndInterval>();	

	if (view.Contains(rawIndex))
	{		
		view[rawIndex].value = value;
		return true;
	}
	else
	{
		return false;
	}
}

bool Database::Update(const SecurityStat& value, uint16_t index)
{
	return this->UpdateEvent(value, index, EventMode::Detect);
}

bool Database::Modify(const openpal::Function1<const Binary&, Binary>& modify, uint16_t index, EventMode mode)
{
	return this->ModifyEvent(modify, index, mode);
}

bool Database::Modify(const openpal::Function1<const DoubleBitBinary&, DoubleBitBinary>& modify, uint16_t index, EventMode mode)
{
	return this->ModifyEvent(modify, index, mode);
}

bool Database::Modify(const openpal::Function1<const Analog&, Analog>& modify, uint16_t index, EventMode mode)
{
	return this->ModifyEvent(modify, index, mode);
}

bool Database::Modify(const openpal::Function1<const Counter&, Counter>& modify, uint16_t index, EventMode mode)
{
	return this->ModifyEvent(modify, index, mode);
}

bool Database::Modify(const openpal::Function1<const FrozenCounter&, FrozenCounter>& modify, uint16_t index, EventMode mode)
{
	return this->ModifyEvent(modify, index, mode);
}

bool Database::Modify(const openpal::Function1<const BinaryOutputStatus&, BinaryOutputStatus>& modify, uint16_t index, EventMode mode)
{
	return this->ModifyEvent(modify, index, mode);
}

bool Database::Modify(const openpal::Function1<const AnalogOutputStatus&, AnalogOutputStatus>& modify, uint16_t index, EventMode mode)
{
	return this->ModifyEvent(modify, index, mode);
}

bool Database::Modify(const openpal::Function1<const TimeAndInterval&, TimeAndInterval>& modify, uint16_t index)
{
	auto rawIndex = GetRawIndex<TimeAndInterval>(index);
	
	auto view = buffers.buffers.GetArrayView<TimeAndInterval>();

	if (view.Contains(rawIndex))
	{
		view[rawIndex].value = modify.Apply(view[rawIndex].value);
		return true;
	}
	else
	{
		return false;
	}
}

bool Database::ConvertToEventClass(PointClass pc, EventClass& ec)
{
	switch (pc)
	{
	case(PointClass::Class1) :
		ec = EventClass::EC1;
		return true;
	case(PointClass::Class2) :
		ec = EventClass::EC2;
		return true;
	case(PointClass::Class3) :
		ec = EventClass::EC3;
		return true;
	default:
		return false;
	}
}

}

