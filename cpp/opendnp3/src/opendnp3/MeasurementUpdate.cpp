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
#include <opendnp3/MeasurementUpdate.h>

#include <vector>

#include <iostream>

namespace opendnp3 
{
 
MeasurementUpdate::MeasurementUpdate()
{}

MeasurementUpdate::MeasurementUpdate(MeasurementUpdate&& arOther) :
	mBinaryVec(std::move(arOther.mBinaryVec)),
	mAnalogVec(std::move(arOther.mAnalogVec)),
	mCounterVec(std::move(arOther.mCounterVec)),
	mControlStatusVec(std::move(arOther.mControlStatusVec)),
	mSetpointStatusVec(std::move(arOther.mSetpointStatusVec)),
	mOctetStringVec(std::move(arOther.mOctetStringVec))
{
	
}

bool MeasurementUpdate::HasUpdates()
{
	return !(mBinaryVec.empty() && 
			mAnalogVec.empty() && 
			mCounterVec.empty() && 
			mControlStatusVec.empty() && 
			mSetpointStatusVec.empty() &&
			mOctetStringVec.empty());
}

void MeasurementUpdate::Add(const Binary& arValue, uint32_t aIndex)
{
	mBinaryVec.push_back(IndexedValue<Binary>(arValue, aIndex));
}

void MeasurementUpdate::Add(const Analog& arValue, uint32_t aIndex)
{
	mAnalogVec.push_back(IndexedValue<Analog>(arValue, aIndex));
}

void MeasurementUpdate::Add(const Counter& arValue, uint32_t aIndex)
{
	mCounterVec.push_back(IndexedValue<Counter>(arValue, aIndex));
}

void MeasurementUpdate::Add(const ControlStatus& arValue, uint32_t aIndex)
{
	mControlStatusVec.push_back(IndexedValue<ControlStatus>(arValue, aIndex));
}

void MeasurementUpdate::Add(const SetpointStatus& arValue, uint32_t aIndex)
{
	mSetpointStatusVec.push_back(IndexedValue<SetpointStatus>(arValue, aIndex));
}

void MeasurementUpdate::Add(const OctetString& arValue, uint32_t aIndex)
{
	mOctetStringVec.push_back(IndexedValue<OctetString>(arValue, aIndex));
}

const IMeasurementUpdate::BinaryContainer& MeasurementUpdate::BinaryUpdates() const
{
	return mBinaryVec;
}

const IMeasurementUpdate::AnalogContainer& MeasurementUpdate::AnalogUpdates() const 
{
	return mAnalogVec;
}

const IMeasurementUpdate::CounterContainer& MeasurementUpdate::CounterUpdates() const 
{
	return mCounterVec;
}

const IMeasurementUpdate::ControlStatusContainer& MeasurementUpdate::ControlStatusUpdates() const 
{
	return mControlStatusVec;
}

const IMeasurementUpdate::SetpointStatusContainer& MeasurementUpdate::SetpointStatusUpdates() const 
{
	return mSetpointStatusVec;
}

const IMeasurementUpdate::OctetStringContainer& MeasurementUpdate::OctetStringUpdates() const 
{
	return mOctetStringVec;
}

}


