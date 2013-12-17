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
#include <opendnp3/DeviceTemplate.h>

#include <openpal/LoggableMacros.h>

using namespace openpal;

namespace opendnp3
{

Database::Database(Logger aLogger) :
	Loggable(aLogger),
	mpEventBuffer(nullptr)
{

}

Database::~Database() {}

////////////////////////////////////////////////////
// Public functions
////////////////////////////////////////////////////

void Database::Configure(MeasurementType aType, size_t numPoints)
{
	switch(aType) 
	{
		case(MeasurementType::BINARY):
			this->Configure(mBinaries, numPoints);		
			break;
		case(MeasurementType::ANALOG):
			this->Configure(mAnalogs, numPoints);	
			break;
		case(MeasurementType::COUNTER):
			this->Configure(mCounters, numPoints);
			break;
		case(MeasurementType::CONTROL_STATUS):
			this->Configure(mControlStatii, numPoints);
			break;
		case(MeasurementType::SETPOINT_STATUS):
			this->Configure(mSetpointStatii, numPoints);
			break;
		default:
			break;
	}
}

void Database::Configure(const DeviceTemplate& devTemplate)
{
	size_t numBinary = devTemplate.mBinary.size();
	size_t numAnalog = devTemplate.mAnalog.size();
	size_t numCounter = devTemplate.mCounter.size();
	size_t numControlStatus = devTemplate.mControlStatus.size();
	size_t numSetpointStatus = devTemplate.mSetpointStatus.size();

	this->Configure(mBinaries, numBinary);				
	this->Configure(mAnalogs, numAnalog);			
	this->Configure(mCounters, numCounter);
	this->Configure(mControlStatii, numControlStatus);
	this->Configure(mSetpointStatii, numSetpointStatus);

	mBinaries.foreachIndex([&](PointInfo<Binary>& record, uint32_t i) { record.clazz = devTemplate.mBinary[i].EventClass; });
	mCounters.foreachIndex([&](PointInfo<Counter>& record, uint32_t i) { record.clazz = devTemplate.mCounter[i].EventClass; });
	
	mAnalogs.foreachIndex([&](PointInfo<Analog>& record, uint32_t i) { 
		record.clazz = devTemplate.mAnalog[i].EventClass; 
		record.deadband =  devTemplate.mAnalog[i].Deadband;
	});	
}

void Database::SetClass(MeasurementType type, PointClass clazz)
{
	switch(type) 
	{
		case(MeasurementType::BINARY):
			mBinaries.foreach([clazz](PointInfo<Binary>& info) { info.clazz = clazz; })	;		
			break;
		case(MeasurementType::ANALOG):
			mAnalogs.foreach([clazz](PointInfo<Analog>& info) { info.clazz = clazz; });
			break;
		case(MeasurementType::COUNTER):
			mCounters.foreach([clazz](PointInfo<Counter>& info) { info.clazz = clazz; });
			break;		
		default:		
			break;
	}
}


void Database::SetEventBuffer(IEventBuffer* apEventBuffer)
{
	assert(apEventBuffer != nullptr);
	assert(mpEventBuffer == nullptr);
	mpEventBuffer = apEventBuffer;
}

////////////////////////////////////////////////////
// IDataObserver interfae - Private NVII functions -
////////////////////////////////////////////////////

void Database::Update(const Binary& value, uint32_t index)
{
	if(mBinaries.Contains(index))
	{	
		auto& record = mBinaries[index];
		if(record.Load(value) && record.HasEventClass() && mpEventBuffer)
		{			
			mpEventBuffer->Update(record);
		}		
	}	
}

void Database::Update(const Analog& value, uint32_t index)
{
	if(mAnalogs.Contains(index))	
	{	
		auto& record = mAnalogs[index];
		if(record.Load(value) && record.HasEventClass() && mpEventBuffer)
		{			
			mpEventBuffer->Update(record);
		}		
	}	
}

void Database::Update(const Counter& value, uint32_t index)
{
	if(mCounters.Contains(index))
	{	
		auto& record = mCounters[index];
		if(record.Load(value) && record.HasEventClass() && mpEventBuffer)
		{			
			mpEventBuffer->Update(record);
		}		
	}
}

void Database::Update(const ControlStatus& value, uint32_t index)
{
	if(mControlStatii.Contains(index)) mControlStatii[index].value = value;
}

void Database::Update(const SetpointStatus& value, uint32_t index)
{
	if(mSetpointStatii.Contains(index)) mSetpointStatii[index].value = value;
}

}

