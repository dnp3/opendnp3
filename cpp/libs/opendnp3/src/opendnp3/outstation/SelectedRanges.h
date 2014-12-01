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
#ifndef OPENDNP3_SELECTEDRANGES_H
#define OPENDNP3_SELECTEDRANGES_H

#include <openpal/util/Uncopyable.h>

#include "opendnp3/app/Range.h"
#include "opendnp3/app/TimeAndInterval.h"
#include "opendnp3/app/MeasurementTypes.h"


namespace opendnp3
{

/**
* Selected ranges for each static datatype
*/
class SelectedRanges : private openpal::Uncopyable
{

public:

	template <class T>
	Range GetRange() { return GetRangeRef<T>(); }

	template <class T>
	void SetRange(const Range& range) { GetRangeRef<T>() = range; }
	
private:

	template <class T>
	Range& GetRangeRef();	

	template <>
	Range& GetRangeRef<Binary>() { return binaries; }

	template <>
	Range& GetRangeRef<DoubleBitBinary>() { return doubleBinaries; }

	template <>
	Range& GetRangeRef<Analog>() { return analogs; }

	template <>
	Range& GetRangeRef<Counter>() { return counters; }

	template <>
	Range& GetRangeRef<FrozenCounter>() { return frozenCounters; }

	template <>
	Range& GetRangeRef<BinaryOutputStatus>() { return binaryOutputStatii; }

	template <>
	Range& GetRangeRef<AnalogOutputStatus>() { return analogOutputStatii; }

	template <>
	Range& GetRangeRef<TimeAndInterval>() { return timeAndIntervals; }

	Range binaries;
	Range doubleBinaries;
	Range analogs;
	Range counters;
	Range frozenCounters;
	Range binaryOutputStatii;
	Range analogOutputStatii;
	Range timeAndIntervals;
};

}

#endif
