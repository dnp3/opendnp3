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
#ifndef OPENDNP3_STATICBUFFERS_H
#define OPENDNP3_STATICBUFFERS_H

#include "StaticBufferView.h"

#include "opendnp3/outstation/DatabaseTemplate.h"

#include <openpal/executor/IMutex.h>
#include <openpal/container/DynamicArray.h>

namespace opendnp3
{

/**
* The static database provides storage for current values and all of the associated metadata
*/
class StaticBuffers : private openpal::Uncopyable
{

public:

	StaticBuffers(const DatabaseTemplate& dbTemplate) :
		binaries(dbTemplate.numBinary),
		doubleBinaries(dbTemplate.numDoubleBinary),
		analogs(dbTemplate.numAnalog),
		counters(dbTemplate.numCounter),
		frozenCounters(dbTemplate.numFrozenCounter),
		binaryOutputStatii(dbTemplate.numBinaryOutputStatus),
		analogOutputStatii(dbTemplate.numAnalogOutputStatus),
		timeAndIntervals(dbTemplate.numTimeAndInterval)
	{}	

	StaticBufferView GetView() const
	{
		return StaticBufferView(
			binaries.ToView(),
			doubleBinaries.ToView(),
			analogs.ToView(),
			counters.ToView(),
			frozenCounters.ToView(),
			binaryOutputStatii.ToView(),
			analogOutputStatii.ToView(),
			timeAndIntervals.ToView()
		);
	}	

	template <class T>
	openpal::ArrayView<MeasurementCell<T>, uint16_t> GetArrayView();

	template <>
	openpal::ArrayView<MeasurementCell<Binary>, uint16_t> GetArrayView() { return binaries.ToView(); }

	template <>
	openpal::ArrayView<MeasurementCell<DoubleBitBinary>, uint16_t> GetArrayView() { return doubleBinaries.ToView(); }

	template <>
	openpal::ArrayView<MeasurementCell<Counter>, uint16_t> GetArrayView() { return counters.ToView(); }

	template <>
	openpal::ArrayView<MeasurementCell<FrozenCounter>, uint16_t> GetArrayView() { return frozenCounters.ToView(); }

	template <>
	openpal::ArrayView<MeasurementCell<Analog>, uint16_t> GetArrayView() { return analogs.ToView(); }

	template <>
	openpal::ArrayView<MeasurementCell<BinaryOutputStatus>, uint16_t> GetArrayView() { return binaryOutputStatii.ToView(); }

	template <>
	openpal::ArrayView<MeasurementCell<AnalogOutputStatus>, uint16_t> GetArrayView() { return analogOutputStatii.ToView(); }

	template <>
	openpal::ArrayView<MeasurementCell<TimeAndInterval>, uint16_t> GetArrayView() { return timeAndIntervals.ToView(); }


private:
		
	openpal::DynamicArray<MeasurementCell<Binary>, uint16_t> binaries;
	openpal::DynamicArray<MeasurementCell<DoubleBitBinary>, uint16_t> doubleBinaries;
	openpal::DynamicArray<MeasurementCell<Analog>, uint16_t> analogs;
	openpal::DynamicArray<MeasurementCell<Counter>, uint16_t> counters;
	openpal::DynamicArray<MeasurementCell<FrozenCounter>, uint16_t> frozenCounters;
	openpal::DynamicArray<MeasurementCell<BinaryOutputStatus>, uint16_t> binaryOutputStatii;
	openpal::DynamicArray<MeasurementCell<AnalogOutputStatus>, uint16_t> analogOutputStatii;
	openpal::DynamicArray<MeasurementCell<TimeAndInterval>, uint16_t> timeAndIntervals;
};

}

#endif
