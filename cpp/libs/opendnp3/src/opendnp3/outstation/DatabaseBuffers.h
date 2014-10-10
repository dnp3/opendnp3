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
#ifndef OPENDNP3_DATABASEBUFFERS_H
#define OPENDNP3_DATABASEBUFFERS_H

#include "opendnp3/outstation/EventMetadata.h"
#include "opendnp3/outstation/ValueMetadataPair.h"
#include "opendnp3/app/MeasurementTypes.h"
#include "opendnp3/app/TimeAndInterval.h"
#include "opendnp3/app/DynamicPointIndexes.h"
#include "opendnp3/outstation/DualValue.h"
#include "opendnp3/outstation/DatabaseTemplate.h"

#include <openpal/container/DynamicArray.h>

namespace opendnp3
{

struct EmptyMetadata {};

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
typedef ValueMetadataPair<DualValue<TimeAndInterval>, EmptyMetadata> TimeAndIntervalCollection;

class DatabaseBuffers
{
public:

	DatabaseBuffers(const DatabaseTemplate& databaseTemplate);	

private:

	openpal::DynamicArray<DualValue<Binary>, uint16_t> binaryValues;
	openpal::DynamicArray<DualValue<DoubleBitBinary>, uint16_t> doubleBinaryValues;
	openpal::DynamicArray<DualValue<Analog>, uint16_t> analogValues;
	openpal::DynamicArray<DualValue<Counter>, uint16_t> counterValues;
	openpal::DynamicArray<DualValue<FrozenCounter>, uint16_t> frozenCounterValues;
	openpal::DynamicArray<DualValue<BinaryOutputStatus>, uint16_t> binaryOutputStatusValues;
	openpal::DynamicArray<DualValue<AnalogOutputStatus>, uint16_t> analogOutputStatusValues;
	openpal::DynamicArray<DualValue<TimeAndInterval>, uint16_t> timeAndIntervalValues;

private:

	openpal::DynamicArray<BinaryMetadata, uint16_t> binaryMetadata;
	openpal::DynamicArray<DoubleBinaryMetadata, uint16_t> doubleBinaryMetadata;
	openpal::DynamicArray<AnalogMetadata, uint16_t> analogMetadata;
	openpal::DynamicArray<CounterMetadata, uint16_t> counterMetadata;
	openpal::DynamicArray<FrozenCounterMetadata, uint16_t> frozenCounterMetadata;
	openpal::DynamicArray<BinaryOutputStatusMetadata, uint16_t> binaryOutputStatusMetadata;
	openpal::DynamicArray<AnalogOutputStatusMetadata, uint16_t> analogOutputStatusMetadata;

    opendnp3::DynamicPointIndexes binaryIndexes;
    opendnp3::DynamicPointIndexes doubleBinaryIndexes;
    opendnp3::DynamicPointIndexes analogIndexes;
    opendnp3::DynamicPointIndexes counterIndexes;
    opendnp3::DynamicPointIndexes frozenCounterIndexes;
    opendnp3::DynamicPointIndexes binaryOutputStatusIndexes;
    opendnp3::DynamicPointIndexes analogOutputStatusIndexes;
    opendnp3::DynamicPointIndexes timeAndIntervalIndexes;
    
public:

	

	BinaryCollection binaries;
	DoubleBinaryCollection doubleBinaries;
	AnalogCollection analogs;
	CounterCollection counters;
	FrozenCounterCollection frozenCounters;
	BinaryOutputStatusCollection binaryOutputStatii;
	AnalogOutputStatusCollection analogOutputStatii;
	TimeAndIntervalCollection timeAndIntervals;

};

}

#endif
