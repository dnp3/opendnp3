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

#include "MeasurementHelpers.h"

#include <assert.h>

#include <opendnp3/DNPConstants.h>


#include <openpal/LoggableMacros.h>

using namespace openpal;

namespace opendnp3
{

Database::Database(const StaticDataFacade& aStaticData) : staticData(aStaticData)
{

}

bool Database::AddEventBuffer(IEventBuffer* apEventBuffer)
{
	assert(apEventBuffer != nullptr);
	return eventBuffers.Add(apEventBuffer);
}

openpal::Indexable<Binary, uint16_t> Database::Binaries() { return staticData.binaries.values; }
openpal::Indexable<Analog, uint16_t> Database::Analogs() { return staticData.analogs.values; }
openpal::Indexable<Counter, uint16_t> Database::Counters() { return staticData.counters.values; }
openpal::Indexable<ControlStatus, uint16_t> Database::ControlStatii() { return staticData.controlStatii; }
openpal::Indexable<SetpointStatus, uint16_t> Database::SetpointStatii() { return staticData.setpointStatii; }

////////////////////////////////////////////////////
// IDataObserver interface
////////////////////////////////////////////////////

void Database::Update(const Binary& value, uint16_t index)
{
	this->UpdateEvent(value, index, staticData.binaries);
}

void Database::Update(const Analog& value, uint16_t index)
{
	this->UpdateEvent(value, index, staticData.analogs);	
}

void Database::Update(const Counter& value, uint16_t index)
{
	this->UpdateEvent(value, index, staticData.counters);
}

void Database::Update(const ControlStatus& value, uint16_t index)
{
	if(staticData.controlStatii.Contains(index)) staticData.controlStatii[index] = value;
}

void Database::Update(const SetpointStatus& value, uint16_t index)
{
	if(staticData.setpointStatii.Contains(index)) staticData.setpointStatii[index] = value;
}

template <> 
openpal::Indexable<Binary, uint16_t> Database::Values<Binary>() { return Binaries(); }

template <> 
openpal::Indexable<Analog, uint16_t> Database::Values<Analog>() { return Analogs(); }

template <> 
openpal::Indexable<Counter, uint16_t> Database::Values<Counter>() { return Counters(); }

template <> 
openpal::Indexable<ControlStatus, uint16_t> Database::Values<ControlStatus>() { return ControlStatii(); }

template <> 
openpal::Indexable<SetpointStatus, uint16_t> Database::Values<SetpointStatus> () { return SetpointStatii(); }

template <> 
uint16_t Database::NumValues<Binary>() const { return staticData.binaries.values.Size(); }

template <> 
uint16_t Database::NumValues<Analog>() const { return staticData.analogs.values.Size(); }

template <> 
uint16_t Database::NumValues<Counter>() const { return staticData.counters.values.Size(); }

template <> 
uint16_t Database::NumValues<ControlStatus>() const { return staticData.controlStatii.Size(); }

template <> 
uint16_t Database::NumValues<SetpointStatus>() const { return staticData.setpointStatii.Size(); }



}

