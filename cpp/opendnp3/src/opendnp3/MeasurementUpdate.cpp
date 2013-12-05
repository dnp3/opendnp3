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


