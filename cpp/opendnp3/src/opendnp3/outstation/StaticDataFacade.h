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
#ifndef __STATIC_DATA_FACADE_H_
#define __STATIC_DATA_FACADE_H_

#include <openpal/Indexable.h>

#include "opendnp3/app/MeasurementTypes.h"

#include "opendnp3/outstation/EventMetadata.h"
#include "opendnp3/outstation/ValueMetadataPair.h"
#include "opendnp3/outstation/DualValue.h"

namespace opendnp3
{

typedef SimpleEventMetadata<Binary> BinaryMetadata;
typedef DeadbandMetadata<Analog, double> AnalogMetadata;
typedef DeadbandMetadata<Counter, uint32_t> CounterMetadata;
typedef DeadbandMetadata<FrozenCounter, uint32_t> FrozenCounterMetadata;

typedef ValueMetadataPair<DualValue<Binary>, BinaryMetadata> BinaryCollection;
typedef ValueMetadataPair<DualValue<Analog>, AnalogMetadata> AnalogCollection;
typedef ValueMetadataPair<DualValue<Counter>, CounterMetadata> CounterCollection;
typedef ValueMetadataPair<DualValue<FrozenCounter>, FrozenCounterMetadata> FrozenCounterCollection;
typedef openpal::Indexable<DualValue<BinaryOutputStatus>, uint16_t> BinaryOutputStatusCollection;
typedef openpal::Indexable<DualValue<AnalogOutputStatus>, uint16_t> AnalogOutputStatusCollection;

// Doesn't contain the real buffers, just a facade that allows use to easily switch
// between static and dynamic allocation
struct StaticDataFacade
{
	StaticDataFacade(
	    BinaryCollection aBinaries,
	    AnalogCollection aAnalogs,
	    CounterCollection aCounters,
	    FrozenCounterCollection aFrozenCounters,
	    BinaryOutputStatusCollection aControlStatii,
	    AnalogOutputStatusCollection aSetpointStatii
	):
		binaries(aBinaries),
		analogs(aAnalogs),
		counters(aCounters),
		frozenCounters(aFrozenCounters),
		controlStatii(aControlStatii),
		setpointStatii(aSetpointStatii)
	{}

	BinaryCollection binaries;
	AnalogCollection analogs;
	CounterCollection counters;
	FrozenCounterCollection frozenCounters;
	BinaryOutputStatusCollection controlStatii;
	AnalogOutputStatusCollection setpointStatii;
};

}

#endif
