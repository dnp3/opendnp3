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
#ifndef OPENDNP3_STATICBUFFERVIEW_H
#define OPENDNP3_STATICBUFFERVIEW_H

#include "opendnp3/app/TimeAndInterval.h"
#include "opendnp3/app/MeasurementTypes.h"

#include "opendnp3/outstation/Cell.h"

#include <openpal/container/ArrayView.h>

namespace opendnp3
{

/**
* StaticBufferView provides abstracted access to storage for current measurement values and associated metadata
*/
class StaticBufferView
{

public:	

	StaticBufferView(
		openpal::ArrayView<Cell<Binary>, uint16_t> binaries_,
		openpal::ArrayView<Cell<DoubleBitBinary>, uint16_t> doubleBinaries_,
		openpal::ArrayView<Cell<Analog>, uint16_t> analogs_,
		openpal::ArrayView<Cell<Counter>, uint16_t> counters_,
		openpal::ArrayView<Cell<FrozenCounter>, uint16_t> frozenCounters_,
		openpal::ArrayView<Cell<BinaryOutputStatus>, uint16_t> binaryOutputStatii_,
		openpal::ArrayView<Cell<AnalogOutputStatus>, uint16_t> analogOutputStatii_,
		openpal::ArrayView<Cell<TimeAndInterval>, uint16_t> timeAndIntervals_
		) :
		binaries(binaries_),
		doubleBinaries(doubleBinaries_),
		analogs(analogs_),
		counters(counters_),
		frozenCounters(frozenCounters_),
		binaryOutputStatii(binaryOutputStatii_),
		analogOutputStatii(analogOutputStatii_),
		timeAndIntervals(timeAndIntervals_)
	{}

	//  ----------- Views of the underlying storage ---------

	openpal::ArrayView<Cell<Binary>, uint16_t> binaries;
	openpal::ArrayView<Cell<DoubleBitBinary>, uint16_t> doubleBinaries;
	openpal::ArrayView<Cell<Analog>, uint16_t> analogs;
	openpal::ArrayView<Cell<Counter>, uint16_t> counters;
	openpal::ArrayView<Cell<FrozenCounter>, uint16_t> frozenCounters;
	openpal::ArrayView<Cell<BinaryOutputStatus>, uint16_t> binaryOutputStatii;
	openpal::ArrayView<Cell<AnalogOutputStatus>, uint16_t> analogOutputStatii;
	openpal::ArrayView<Cell<TimeAndInterval>, uint16_t> timeAndIntervals;	
};

}

#endif
