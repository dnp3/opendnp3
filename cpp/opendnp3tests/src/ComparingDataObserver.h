#ifndef __COMPARING_DATA_OBSERVER_H_
#define __COMPARING_DATA_OBSERVER_H_

#include <opendnp3/IDataObserver.h>

#include "FlexibleDataObserver.h"

#include <map>
#include <mutex>
#include <iostream>
#include <condition_variable>

namespace opendnp3
{

class ComparingDataObserver : public IDataObserver, public IMeasurementHandler
{
public:

	ComparingDataObserver(FlexibleDataObserver* apObserver);

	void Reset();

	bool WaitForSameData(openpal::TimeDuration aTimeout);

	void DescribeMissingData();

	void Load(const IMeasurementUpdate& arUpdate);

private:

	bool mSameData;

	bool IsSameData();

	std::mutex mMutex;
	std::condition_variable mCondition;
	FlexibleDataObserver* mpObserver;

	typedef std::map<size_t, bool> CompareMap;

	CompareMap mBinaryMap;
	CompareMap mAnalogMap;
	CompareMap mCounterMap;
	CompareMap mControlStatusMap;
	CompareMap mSetpointStatusMap;

	void _Start();
	void _End();

	void _Update(const Binary& arPoint, size_t aIndex);
	void _Update(const Analog& arPoint, size_t aIndex);
	void _Update(const Counter& arPoint, size_t aIndex);
	void _Update(const ControlStatus& arPoint, size_t aIndex);
	void _Update(const SetpointStatus& arPoint, size_t aIndex);

	template <class T>
	void UpdateAny(const T& arPoint, size_t aIndex, const typename PointMap<T>::Type& arMap, CompareMap& arCompareMap);

	template <class T>
	void DescribeAny(const typename PointMap<T>::Type& arMap, const CompareMap& arCompareMap);
};


template <class T>
void ComparingDataObserver::DescribeAny(const typename PointMap<T>::Type& arMap, const CompareMap& arCompareMap)
{
for(auto pair: arMap) {
		CompareMap::const_iterator j = arCompareMap.find(pair.first);
		if(j == arCompareMap.end()) {
			std::cout << "Missing: " << pair.first << " - " << pair.second.ToString() << std::endl;
		}
	}
}

template <class T>
void ComparingDataObserver::UpdateAny(const T& arPoint, size_t aIndex, const typename PointMap<T>::Type& arMap, CompareMap& arCompareMap)
{
	typename PointMap<T>::Type::const_iterator i = arMap.find(aIndex);
	if(i == arMap.end()) {
		std::cout << "Unexpected index: " << aIndex << " - " << arPoint.ToString() << std::endl;
	}
	else {
		if(i->second == arPoint) {
			arCompareMap[aIndex] = true;
		}
		else {
			arCompareMap.erase(aIndex);
		}
	}
}

}

#endif

