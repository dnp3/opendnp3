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
#ifndef __DATABASE_H_
#define __DATABASE_H_

#include "DatabaseInterfaces.h"
#include "MeasurementHelpers.h"

#include <opendnp3/DNPConstants.h>
#include <opendnp3/IDataObserver.h>

#include <openpal/Loggable.h>

#include <set>
#include <vector>
#include <limits>

#ifdef max
#undef max
#endif

namespace opendnp3
{

class INotifier;
class EventLog;
struct DeviceTemplate;

template <class T>
struct StaticIterator {
	typedef typename std::vector<PointInfo<T>>::const_iterator Type;
};


/**
Manages the static data model of a DNP3 slave. Dual-interface to update data points and read current values.

Passes data updates to an associated event buffer for event generation/management.
*/
class Database : public IDataObserver, public openpal::Loggable
{
public:

	Database(openpal::Logger aLogger);

	virtual ~Database();

	/* Configuration functions */

	void Configure(const DeviceTemplate& arTmp);
	void Configure(MeasurementType aType, size_t aNumPoints, bool aStartOnline = false);

	size_t NumType(MeasurementType aType);
	
	bool SetDeadband(MeasurementType, size_t aIndex, double aDeadband);
	bool SetClass(MeasurementType aType, size_t aIndex, PointClass aClass);
	void SetClass(MeasurementType aType, PointClass aClass); //set classes for all indices

	void SetEventBuffer(IEventBuffer*);

	/* Functions for obtaining iterators */

	StaticIterator<Binary>::Type BeginBinary() { return mBinaryVec.begin(); }	
	StaticIterator<Analog>::Type BeginAnalog() { return mAnalogVec.begin(); }
	StaticIterator<Counter>::Type BeginCounter() { return mCounterVec.begin(); }
	StaticIterator<ControlStatus>::Type BeginControlStatus() { return mControlStatusVec.begin(); }	
	StaticIterator<SetpointStatus>::Type BeginSetpointStatus() { return mSetpointStatusVec.begin(); }


private:

	// ITransactable  functions, no lock on this structure.
	void _Start() {}
	void _End() {}

	// IDataObserver functions
	void _Update(const Binary& arPoint, size_t);
	void _Update(const Analog& arPoint, size_t);
	void _Update(const Counter& arPoint, size_t);
	void _Update(const ControlStatus& arPoint, size_t);
	void _Update(const SetpointStatus& arPoint, size_t);

	template<typename T>
	void AssignIndices( std::vector< PointInfo<T> >& arVector );

	template<typename T>
	void SetAllOnline( std::vector< PointInfo<T> >& arVector );

	template<typename T>
	bool UpdateValue(std::vector< PointInfo<T> >& arVec, const T& arValue, size_t aIndex);

	template<typename T>
	void PerformRead(std::vector< PointInfo<T> >& arVec, T& arValue, size_t aIndex);

	/////////////////////////////////////////
	//	Static data
	/////////////////////////////////////////

	std::vector< PointInfo<Binary> > mBinaryVec;
	std::vector< PointInfo<Analog> > mAnalogVec;
	std::vector< PointInfo<Counter> > mCounterVec;
	std::vector< PointInfo<ControlStatus> > mControlStatusVec;
	std::vector< PointInfo<SetpointStatus> > mSetpointStatusVec;

	IEventBuffer* mpEventBuffer;

	template <typename T>
	size_t CalcNumType(const std::vector<T*>& arIdxVec);
};


template <typename T>
size_t Database::CalcNumType(const std::vector<T*>& arIdxVec)
{
	std::set<size_t> indexSet;

for(auto i: arIdxVec) indexSet.insert(i->Index);

	if(indexSet.size() > 0) return *indexSet.rbegin() + 1;
	else return 0;
}

template<typename T>
inline void Database::SetAllOnline( std::vector< PointInfo<T> >& arVector )
{
	// TODO
	// for(size_t i = 0; i < arVector.size(); ++i) arVector[i].mValue.SetQuality(T::ONLINE);
}

template<typename T>
inline void Database::AssignIndices( std::vector< PointInfo<T> >& arVector )
{
	for(size_t i = 0; i < arVector.size(); i++) {
		arVector[i].index = i;
	}
}

template<typename T>
bool Database::UpdateValue(std::vector< PointInfo<T> >& arVec, const T& arValue, size_t aIndex)
{
	if(aIndex >= arVec.size()) return false;
	else {
		T& value = arVec[aIndex].value;

		if(IsEvent<T>(arValue, arVec[aIndex].lastEvent, arVec[aIndex].deadband))
		{
			value = arValue;
			return ((arVec[aIndex].clazz & PC_ALL_EVENTS) != 0);
		}
		else {
			value = arValue;
			return false;
		}
	}
}

}

#endif

