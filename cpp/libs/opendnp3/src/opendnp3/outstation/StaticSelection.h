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
#ifndef OPENDNP3_STATICSELECTION_H
#define OPENDNP3_STATICSELECTION_H


#include "opendnp3/outstation/DatabaseTemplate.h"

#include "opendnp3/app/MeasurementTypes.h"
#include "opendnp3/app/TimeAndInterval.h"
#include "opendnp3/outstation/MeasurementCell.h"

#include <openpal/util/Uncopyable.h>
#include <openpal/container/DynamicArray.h>


namespace opendnp3
{

/**
* Represents static data that has been selected for reporting
*/
class StaticSelection : private openpal::Uncopyable
{

public:

	StaticSelection(const DatabaseTemplate& dbTemplate) :
		binaries(dbTemplate.numBinary),
		doubleBinaries(dbTemplate.numDoubleBinary),
		analogs(dbTemplate.numAnalog),
		counters(dbTemplate.numCounter),
		frozenCounters(dbTemplate.numFrozenCounter),
		binaryOutputStatii(dbTemplate.numBinaryOutputStatus),
		analogOutputStatii(dbTemplate.numAnalogOutputStatus),
		timeAndIntervals(dbTemplate.numTimeAndInterval)
	{}

	template <class T>
	openpal::ArrayView<BufferedCell<T>, uint16_t> GetArrayView();

	template <>
	openpal::ArrayView<BufferedCell<Binary>, uint16_t> GetArrayView() { return binaries.ToView(); }

	template <>
	openpal::ArrayView<BufferedCell<DoubleBitBinary>, uint16_t> GetArrayView() { return doubleBinaries.ToView(); }

	template <>
	openpal::ArrayView<BufferedCell<Counter>, uint16_t> GetArrayView() { return counters.ToView(); }

	template <>
	openpal::ArrayView<BufferedCell<FrozenCounter>, uint16_t> GetArrayView() { return frozenCounters.ToView(); }

	template <>
	openpal::ArrayView<BufferedCell<Analog>, uint16_t> GetArrayView() { return analogs.ToView(); }

	template <>
	openpal::ArrayView<BufferedCell<BinaryOutputStatus>, uint16_t> GetArrayView() { return binaryOutputStatii.ToView(); }

	template <>
	openpal::ArrayView<BufferedCell<AnalogOutputStatus>, uint16_t> GetArrayView() { return analogOutputStatii.ToView(); }

	template <>
	openpal::ArrayView<BufferedCell<TimeAndInterval>, uint16_t> GetArrayView() { return timeAndIntervals.ToView(); }

private:	
		
	openpal::DynamicArray<BufferedCell<Binary>, uint16_t> binaries;
	openpal::DynamicArray<BufferedCell<DoubleBitBinary>, uint16_t> doubleBinaries;
	openpal::DynamicArray<BufferedCell<Analog>, uint16_t> analogs;
	openpal::DynamicArray<BufferedCell<Counter>, uint16_t> counters;
	openpal::DynamicArray<BufferedCell<FrozenCounter>, uint16_t> frozenCounters;
	openpal::DynamicArray<BufferedCell<BinaryOutputStatus>, uint16_t> binaryOutputStatii;
	openpal::DynamicArray<BufferedCell<AnalogOutputStatus>, uint16_t> analogOutputStatii;
	openpal::DynamicArray<BufferedCell<TimeAndInterval>, uint16_t> timeAndIntervals;
};

}

#endif
