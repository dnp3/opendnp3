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

#include <openpal/LogMacros.h>

#include "opendnp3/app/MeasurementHelpers.h"
#include "opendnp3/LogErrorCodes.h"

#include <assert.h>

using namespace openpal;

namespace opendnp3
{

Database::Database(const StaticDataFacade& staticData_, openpal::IMutex* pMutex_) :
	staticData(staticData_),
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
		onEventAction.Foreach([](const Runnable& runnable) { runnable.Run(); });
	}
	openpal::CriticalSection::Unlock(pMutex);
}

void Database::SetEventBuffer(IEventBuffer& eventBuffer)
{
	pEventBuffer = &eventBuffer;
}

void Database::DoubleBuffer()
{
	FreezeCollection(staticData.binaries.values);
	FreezeCollection(staticData.doubleBinaries.values);
	FreezeCollection(staticData.analogs.values);
	FreezeCollection(staticData.counters.values);
	FreezeCollection(staticData.frozenCounters.values);
	FreezeCollection(staticData.binaryOutputStatii.values);
	FreezeCollection(staticData.analogOutputStatii.values);
}

openpal::Indexable<DualValue<Binary>, uint16_t>& Database::Binaries()
{
	return staticData.binaries.values;
}

openpal::Indexable<DualValue<DoubleBitBinary>, uint16_t>& Database::DoubleBinaries()
{
	return staticData.doubleBinaries.values;
}

openpal::Indexable<DualValue<Analog>, uint16_t>& Database::Analogs()
{
	return staticData.analogs.values;
}
openpal::Indexable<DualValue<Counter>, uint16_t>& Database::Counters()
{
	return staticData.counters.values;
}
openpal::Indexable<DualValue<FrozenCounter>, uint16_t>& Database::FrozenCounters()
{
	return staticData.frozenCounters.values;
}
openpal::Indexable<DualValue<BinaryOutputStatus>, uint16_t>& Database::ControlStatii()
{
	return staticData.binaryOutputStatii.values;
}
openpal::Indexable<DualValue<AnalogOutputStatus>, uint16_t>& Database::SetpointStatii()
{
	return staticData.analogOutputStatii.values;
}

////////////////////////////////////////////////////
// IMeasurementLoader* interface
////////////////////////////////////////////////////

void Database::Update(const Binary& value, uint16_t index)
{
	this->UpdateEvent(value, index, staticData.binaries);
}

void Database::Update(const DoubleBitBinary& value, uint16_t index)
{
	this->UpdateEvent(value, index, staticData.doubleBinaries);
}

void Database::Update(const Analog& value, uint16_t index)
{
	this->UpdateEvent(value, index, staticData.analogs);
}

void Database::Update(const Counter& value, uint16_t index)
{
	this->UpdateEvent(value, index, staticData.counters);
}

void Database::Update(const FrozenCounter& value, uint16_t index)
{
	this->UpdateEvent(value, index, staticData.frozenCounters);
}

void Database::Update(const BinaryOutputStatus& value, uint16_t index)
{
	this->UpdateEvent(value, index, staticData.binaryOutputStatii);
}

void Database::Update(const AnalogOutputStatus& value, uint16_t index)
{
	this->UpdateEvent(value, index, staticData.analogOutputStatii);
}

template <>
openpal::Indexable<DualValue<Binary>, uint16_t>& Database::Values<Binary>()
{
	return Binaries();
}

template <>
openpal::Indexable<DualValue<DoubleBitBinary>, uint16_t>& Database::Values<DoubleBitBinary>()
{
	return DoubleBinaries();
}

template <>
openpal::Indexable<DualValue<Analog>, uint16_t>& Database::Values<Analog>()
{
	return Analogs();
}

template <>
openpal::Indexable<DualValue<Counter>, uint16_t>& Database::Values<Counter>()
{
	return Counters();
}

template <>
openpal::Indexable<DualValue<FrozenCounter>, uint16_t>& Database::Values<FrozenCounter>()
{
	return FrozenCounters();
}

template <>
openpal::Indexable<DualValue<BinaryOutputStatus>, uint16_t>& Database::Values<BinaryOutputStatus>()
{
	return ControlStatii();
}

template <>
openpal::Indexable<DualValue<AnalogOutputStatus>, uint16_t>& Database::Values<AnalogOutputStatus>()
{
	return SetpointStatii();
}

template <>
uint16_t Database::NumValues<Binary>() const
{
	return staticData.binaries.values.Size();
}

template <>
uint16_t Database::NumValues<DoubleBitBinary>() const
{
	return staticData.doubleBinaries.values.Size();
}

template <>
uint16_t Database::NumValues<Analog>() const
{
	return staticData.analogs.values.Size();
}

template <>
uint16_t Database::NumValues<Counter>() const
{
	return staticData.counters.values.Size();
}

template <>
uint16_t Database::NumValues<FrozenCounter>() const
{
	return staticData.frozenCounters.values.Size();
}

template <>
uint16_t Database::NumValues<BinaryOutputStatus>() const
{
	return staticData.binaryOutputStatii.values.Size();
}

template <>
uint16_t Database::NumValues<AnalogOutputStatus>() const
{
	return staticData.analogOutputStatii.values.Size();
}

void Database::SetEventHandler(const Runnable& callback)
{
	onEventAction.Set(callback);
}


}

