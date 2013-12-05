#ifndef __MOCK_MEASUREMENT_HANDLER_H_
#define __MOCK_MEASUREMENT_HANDLER_H_

#include <opendnp3/IMeasurementHandler.h>
#include <opendnp3/OctetString.h>

#include <openpal/Exception.h>

#include <map>


namespace opendnp3
{



// simple measurement handler for testing purposes
class MockMeasurementHandler : public IMeasurementHandler
{
public:

	MockMeasurementHandler()
	{}
			
	void Load(const IMeasurementUpdate& arUpdate) override
	{
		for(auto v: arUpdate.BinaryUpdates()) mBinaryMap[v.index] = v.value;
		for(auto v: arUpdate.AnalogUpdates()) mAnalogMap[v.index] = v.value;
		for(auto v: arUpdate.CounterUpdates()) mCounterMap[v.index] = v.value;
		for(auto v: arUpdate.ControlStatusUpdates()) mControlStatusMap[v.index] = v.value;
		for(auto v: arUpdate.SetpointStatusUpdates()) mSetpointStatusMap[v.index] = v.value;
		for(auto v: arUpdate.OctetStringUpdates()) mOctetStringMap[v.index] = v.value;
	}

	void Clear()
	{
		mBinaryMap.clear();
		mAnalogMap.clear();
		mCounterMap.clear();
		mControlStatusMap.clear();
		mSetpointStatusMap.clear();
		mOctetStringMap.clear();
	}

	Binary GetBinary(uint32_t aIndex) { return GetAny<Binary>(aIndex, mBinaryMap); }
	Analog GetAnalog(uint32_t aIndex) { return GetAny<Analog>(aIndex, mAnalogMap); }
	Counter GetCounter(uint32_t aIndex) { return GetAny<Counter>(aIndex, mCounterMap); }
	ControlStatus GetControlStatus(uint32_t aIndex) { return GetAny<ControlStatus>(aIndex, mControlStatusMap); }
	SetpointStatus GetSetpointStatus(uint32_t aIndex) { return GetAny<SetpointStatus>(aIndex, mSetpointStatusMap); }
	OctetString GetOctetString(uint32_t aIndex) { return GetAny<OctetString>(aIndex, mOctetStringMap); }

	

private:

	template <typename T>
	struct PointMap {
		typedef std::map<uint32_t, T> Type;
	};

	template <class T>
	T GetAny(uint32_t aIndex, const typename PointMap<T>::Type& arMap)
	{
		auto iter = arMap.find(aIndex);
		if(iter == arMap.end()) throw openpal::Exception(LOCATION, "Index not found in map");
		else return iter->second;
	}
		
	PointMap<Binary>::Type mBinaryMap;
	PointMap<Analog>::Type mAnalogMap;
	PointMap<Counter>::Type mCounterMap;
	PointMap<ControlStatus>::Type mControlStatusMap;
	PointMap<SetpointStatus>::Type mSetpointStatusMap;
	PointMap<OctetString>::Type mOctetStringMap;

};

}

#endif

