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

void Database::Configure(MeasurementType aType, size_t aNumPoints)
{
	switch(aType) {
	case(MeasurementType::BINARY):
		this->mBinaryVec.resize(aNumPoints);
		this->AssignIndices(mBinaryVec);
		break;
	case(MeasurementType::ANALOG):
		this->mAnalogVec.resize(aNumPoints);
		this->AssignIndices(mAnalogVec);		
		break;
	case(MeasurementType::COUNTER):
		this->mCounterVec.resize(aNumPoints);
		this->AssignIndices(mCounterVec);
		break;
	case(MeasurementType::CONTROL_STATUS):
		this->mControlStatusVec.resize(aNumPoints);
		this->AssignIndices(mControlStatusVec);
		break;
	case(MeasurementType::SETPOINT_STATUS):
		this->mSetpointStatusVec.resize(aNumPoints);
		this->AssignIndices(mSetpointStatusVec);
		break;
	}
}

void Database::Configure(const DeviceTemplate& arTmp)
{
	size_t numBinary = arTmp.mBinary.size();
	size_t numAnalog = arTmp.mAnalog.size();
	size_t numCounter = arTmp.mCounter.size();
	size_t numControlStatus = arTmp.mControlStatus.size();
	size_t numSetpointStatus = arTmp.mSetpointStatus.size();

	//configure the database for these objects
	this->Configure(MeasurementType::BINARY, numBinary);
	this->Configure(MeasurementType::ANALOG, numAnalog);
	this->Configure(MeasurementType::COUNTER, numCounter);
	this->Configure(MeasurementType::CONTROL_STATUS, numControlStatus);
	this->Configure(MeasurementType::SETPOINT_STATUS, numSetpointStatus);

	for(size_t i = 0; i < arTmp.mBinary.size(); ++i) 
	{
		this->SetClass(MeasurementType::BINARY, i, arTmp.mBinary[i].EventClass);
	}

	for(size_t i = 0; i < arTmp.mCounter.size(); ++i)
	{
		this->SetClass(MeasurementType::COUNTER, i, arTmp.mCounter[i].EventClass);
	}

	for(size_t i = 0; i < arTmp.mAnalog.size(); ++i) 
	{
		this->SetClass(MeasurementType::ANALOG, i, arTmp.mAnalog[i].EventClass);
		this->SetDeadband(MeasurementType::ANALOG, i, arTmp.mAnalog[i].Deadband);
	}
}

void Database::SetClass(MeasurementType aType, PointClass aClass)
{
	switch(aType) {
		case(MeasurementType::BINARY):
			for(auto& m: mBinaryVec) m.clazz = aClass;		
			break;
		case(MeasurementType::ANALOG):
			for(auto& m: mAnalogVec) m.clazz = aClass;			
			break;
		case(MeasurementType::COUNTER):
			for(auto& m: mCounterVec) m.clazz = aClass;			
			break;		
		default:		
			break;
	}
}

bool Database::SetClass(MeasurementType aType, size_t aIndex, PointClass aClass)
{
	switch(aType) {
		case(MeasurementType::BINARY):
			if(aIndex >= mBinaryVec.size()) return false;
			mBinaryVec[aIndex].clazz = aClass;
			return true;
		case(MeasurementType::ANALOG):
			if(aIndex >= mAnalogVec.size()) return false;
			mAnalogVec[aIndex].clazz = aClass;
			return true;
		case(MeasurementType::COUNTER):
			if(aIndex >= mCounterVec.size()) return false;
			mCounterVec[aIndex].clazz = aClass;
			return true;		
		default:
			return false;
	}	
}

bool Database::SetDeadband(MeasurementType aType, size_t aIndex, double aDeadband)
{
	switch(aType) {
		case(MeasurementType::ANALOG):
			if(aIndex >= mAnalogVec.size()) return false;
			mAnalogVec[aIndex].deadband = aDeadband;
			return true;
		case(MeasurementType::COUNTER):
			if(aIndex >= mCounterVec.size()) return false;
			mCounterVec[aIndex].deadband = aDeadband;
			return true;
		default:
			return false;
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

void Database::Update(const Binary& aValue, size_t aIndex)
{
	if(aIndex < mBinaryVec.size())	
	{	
		auto& record = mBinaryVec[aIndex];		
		if(record.Load(aValue) && record.HasEventClass() && mpEventBuffer)
		{				
			mpEventBuffer->Update(record);			
		}		
	}	
}

void Database::Update(const Analog& aValue, size_t aIndex)
{
	if(aIndex < mAnalogVec.size())	
	{	
		auto& record = mAnalogVec[aIndex];		
		if(record.Load(aValue) && record.HasEventClass() && mpEventBuffer)
		{			
			mpEventBuffer->Update(record);
		}		
	}	
}

void Database::Update(const Counter& aValue, size_t aIndex)
{
	if(aIndex < mCounterVec.size())	
	{	
		auto& record = mCounterVec[aIndex];		
		if(record.Load(aValue) && record.HasEventClass() && mpEventBuffer)
		{			
			mpEventBuffer->Update(record);
		}		
	}
}

void Database::Update(const ControlStatus& aValue, size_t aIndex)
{
	if(aIndex < mControlStatusVec.size()) mControlStatusVec[aIndex].value = aValue;	
}

void Database::Update(const SetpointStatus& aValue, size_t aIndex)
{
	if(aIndex < mSetpointStatusVec.size()) mSetpointStatusVec[aIndex].value = aValue;
}

////////////////////////////////////////////////////
// misc public functions
////////////////////////////////////////////////////

uint32_t Database::NumType(MeasurementType aType)
{
	switch(aType) {
		case(MeasurementType::BINARY):
			return mBinaryVec.size();
		case(MeasurementType::ANALOG):
			return mAnalogVec.size();
		case(MeasurementType::COUNTER):
			return mCounterVec.size();
		case(MeasurementType::CONTROL_STATUS):
			return mControlStatusVec.size();
		case(MeasurementType::SETPOINT_STATUS):
			return mSetpointStatusVec.size();
		default:
			return 0;
	}	
}

}

