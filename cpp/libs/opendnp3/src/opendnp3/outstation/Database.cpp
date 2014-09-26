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

Database::Database(const DatabaseTemplate& temp, openpal::IMutex* pMutex_) :
	buffers(temp),
	pEventBuffer(nullptr),
	pMutex(pMutex_),
	transactionHasEvents(false)
{

}

void Database::Start()
{
	openpal::CriticalSection::Lock(pMutex);	
}

void Database::End()
{
	if (transactionHasEvents)
	{
		transactionHasEvents = false;
		onEventAction.Foreach([](const Action0& runnable) { runnable.Apply(); });
	}
	openpal::CriticalSection::Unlock(pMutex);
}

void Database::SetEventBuffer(IEventBuffer& eventBuffer)
{
	pEventBuffer = &eventBuffer;
}

void Database::DoubleBuffer()
{
	FreezeCollection(buffers.binaries.values);
	FreezeCollection(buffers.doubleBinaries.values);
	FreezeCollection(buffers.analogs.values);
	FreezeCollection(buffers.counters.values);
	FreezeCollection(buffers.frozenCounters.values);
	FreezeCollection(buffers.binaryOutputStatii.values);
	FreezeCollection(buffers.analogOutputStatii.values);
}

////////////////////////////////////////////////////
// IDatabase* interface
////////////////////////////////////////////////////

void Database::Update(const Binary& value, uint16_t index)
{
	this->UpdateEvent(value, index, buffers.binaries);
}

void Database::Update(const DoubleBitBinary& value, uint16_t index)
{
	this->UpdateEvent(value, index, buffers.doubleBinaries);
}

void Database::Update(const Analog& value, uint16_t index)
{
	this->UpdateEvent(value, index, buffers.analogs);
}

void Database::Update(const Counter& value, uint16_t index)
{
	this->UpdateEvent(value, index, buffers.counters);
}

void Database::Update(const FrozenCounter& value, uint16_t index)
{
	this->UpdateEvent(value, index, buffers.frozenCounters);
}

void Database::Update(const BinaryOutputStatus& value, uint16_t index)
{
	this->UpdateEvent(value, index, buffers.binaryOutputStatii);
}

void Database::Update(const AnalogOutputStatus& value, uint16_t index)
{
	this->UpdateEvent(value, index, buffers.analogOutputStatii);
}

template <>
openpal::Indexable<DualValue<Binary>, uint16_t>& Database::Values<Binary>()
{
	return buffers.binaries.values;
}

template <>
openpal::Indexable<DualValue<DoubleBitBinary>, uint16_t>& Database::Values<DoubleBitBinary>()
{
	return  buffers.doubleBinaries.values;
}

template <>
openpal::Indexable<DualValue<Analog>, uint16_t>& Database::Values<Analog>()
{
	return buffers.analogs.values;
}

template <>
openpal::Indexable<DualValue<Counter>, uint16_t>& Database::Values<Counter>()
{
	return buffers.counters.values;
}

template <>
openpal::Indexable<DualValue<FrozenCounter>, uint16_t>& Database::Values<FrozenCounter>()
{
	return buffers.frozenCounters.values;
}

template <>
openpal::Indexable<DualValue<BinaryOutputStatus>, uint16_t>& Database::Values<BinaryOutputStatus>()
{
	return buffers.binaryOutputStatii.values;
}

template <>
openpal::Indexable<DualValue<AnalogOutputStatus>, uint16_t>& Database::Values<AnalogOutputStatus>()
{
	return buffers.analogOutputStatii.values;
}

template <>
uint16_t Database::NumValues<Binary>() const
{
	return buffers.binaries.values.Size();
}

template <>
uint16_t Database::NumValues<DoubleBitBinary>() const
{
	return buffers.doubleBinaries.values.Size();
}

template <>
uint16_t Database::NumValues<Analog>() const
{
	return buffers.analogs.values.Size();
}

template <>
uint16_t Database::NumValues<Counter>() const
{
	return buffers.counters.values.Size();
}

template <>
uint16_t Database::NumValues<FrozenCounter>() const
{
	return buffers.frozenCounters.values.Size();
}

template <>
uint16_t Database::NumValues<BinaryOutputStatus>() const
{
	return buffers.binaryOutputStatii.values.Size();
}

template <>
uint16_t Database::NumValues<AnalogOutputStatus>() const
{
	return buffers.analogOutputStatii.values.Size();
}

bool Database::AssignClass(AssignClassType type, PointClass clazz, const StaticRange& range)
{
	switch (type)
	{	
		case(AssignClassType::BinaryInput) :
			return AssignClassTo(buffers.binaries.metadata, clazz, range);
		default:
			return false;
	}
}

void Database::SetEventHandler(const Action0& callback)
{
	onEventAction.Set(callback);
}


}

