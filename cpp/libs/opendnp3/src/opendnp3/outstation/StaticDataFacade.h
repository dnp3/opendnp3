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
#ifndef OPENDNP3_STATICDATAFACADE_H
#define OPENDNP3_STATICDATAFACADE_H

#include <openpal/container/Indexable.h>

#include "opendnp3/app/MeasurementTypes.h"

#include "opendnp3/outstation/EventMetadata.h"
#include "opendnp3/outstation/ValueMetadataPair.h"
#include "opendnp3/outstation/DualValue.h"

namespace opendnp3
{

typedef SimpleEventMetadata<Binary> BinaryMetadata;
typedef SimpleEventMetadata<DoubleBitBinary> DoubleBinaryMetadata;
typedef DeadbandMetadata<Analog, double> AnalogMetadata;
typedef DeadbandMetadata<Counter, uint32_t> CounterMetadata;
typedef DeadbandMetadata<FrozenCounter, uint32_t> FrozenCounterMetadata;
typedef SimpleEventMetadata<BinaryOutputStatus> BinaryOutputStatusMetadata;
typedef DeadbandMetadata<AnalogOutputStatus, double> AnalogOutputStatusMetadata;

typedef ValueMetadataPair<DualValue<Binary>, BinaryMetadata> BinaryCollection;
typedef ValueMetadataPair<DualValue<DoubleBitBinary>, DoubleBinaryMetadata> DoubleBinaryCollection;
typedef ValueMetadataPair<DualValue<Analog>, AnalogMetadata> AnalogCollection;
typedef ValueMetadataPair<DualValue<Counter>, CounterMetadata> CounterCollection;
typedef ValueMetadataPair<DualValue<FrozenCounter>, FrozenCounterMetadata> FrozenCounterCollection;
typedef ValueMetadataPair<DualValue<BinaryOutputStatus>, BinaryOutputStatusMetadata> BinaryOutputStatusCollection;
typedef ValueMetadataPair<DualValue<AnalogOutputStatus>, AnalogOutputStatusMetadata> AnalogOutputStatusCollection;

// Doesn't contain the real buffers, just a facade that allows use to easily switch
// between static and dynamic allocation
struct StaticDataFacade
{
	StaticDataFacade(
	    BinaryCollection binaries_,
	    DoubleBinaryCollection doubleBinaries_,
	    AnalogCollection analogs_,
	    CounterCollection counters_,
	    FrozenCounterCollection frozenCounters_,
	    BinaryOutputStatusCollection binaryOutputStatii_,
	    AnalogOutputStatusCollection analogOutputStatii_
	):
		binaries(binaries_),
		doubleBinaries(doubleBinaries_),
		analogs(analogs_),
		counters(counters_),
		frozenCounters(frozenCounters_),
		binaryOutputStatii(binaryOutputStatii_),
		analogOutputStatii(analogOutputStatii_)
	{}

	BinaryCollection binaries;
	DoubleBinaryCollection doubleBinaries;
	AnalogCollection analogs;
	CounterCollection counters;
	FrozenCounterCollection frozenCounters;
	BinaryOutputStatusCollection binaryOutputStatii;
	AnalogOutputStatusCollection analogOutputStatii;
};

}

#endif
