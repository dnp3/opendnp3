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

#include "IEventBuffer.h"
#include "MeasurementHelpers.h"
#include "PointInfo.h"

#include <opendnp3/DNPConstants.h>
#include <opendnp3/IDataObserver.h>

#include <openpal/Loggable.h>
#include <openpal/DynamicCollection.h>

#include <vector>
#include <limits>

#ifdef max
#undef max
#endif

namespace opendnp3
{

struct DeviceTemplate;

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
	void Configure(MeasurementType aType, size_t aNumPoints);	
	void SetClass(MeasurementType aType, PointClass aClass); //set classes for all indices

	void SetEventBuffer(IEventBuffer*);

	/* Functions for obtaining iterators */

	openpal::Indexable<PointInfo<Binary>> Binaries() { return mBinaries.ToIndexable(); }
	openpal::Indexable<PointInfo<Analog>> Analogs() { return mAnalogs.ToIndexable(); }
	openpal::Indexable<PointInfo<Counter>> Counters() { return mCounters.ToIndexable(); }
	openpal::Indexable<PointInfo<ControlStatus>> ControlStatii() { return mControlStatii.ToIndexable(); }
	openpal::Indexable<PointInfo<SetpointStatus>> SetpointStatii() { return mSetpointStatii.ToIndexable(); }

	// IDataObserver functions
	void Update(const Binary& arPoint, uint32_t) final;
	void Update(const Analog& arPoint, uint32_t) final;
	void Update(const Counter& arPoint, uint32_t) final;
	void Update(const ControlStatus& arPoint, uint32_t) final;
	void Update(const SetpointStatus& arPoint, uint32_t) final;

	/////////////////////////////////////////
	//	Static data
	/////////////////////////////////////////

	openpal::DynamicCollection<PointInfo<Binary>> mBinaries;	
	openpal::DynamicCollection<PointInfo<Analog>> mAnalogs;
	openpal::DynamicCollection<PointInfo<Counter>> mCounters;
	openpal::DynamicCollection<PointInfo<ControlStatus>> mControlStatii;
	openpal::DynamicCollection<PointInfo<SetpointStatus>> mSetpointStatii;

private:

	// ITransactable  functions, no lock on this structure.
	void Start() final {}
	void End() final {}	

	template <typename T>
	inline static void Configure(openpal::DynamicCollection<PointInfo<T>>& collection, uint32_t size)
	{	
		collection.Resize(size);
		for(uint32_t i = 0; i < collection.Size(); ++i) collection[i].index = i;
	}

	template<typename T>
	void SetAllOnline( std::vector< PointInfo<T> >& arVector );

	template<typename T>
	bool UpdateValue(std::vector< PointInfo<T> >& arVec, const T& arValue, size_t aIndex);

	template<typename T>
	void PerformRead(std::vector< PointInfo<T> >& arVec, T& arValue, size_t aIndex);

	

	IEventBuffer* mpEventBuffer;
};



}

#endif

