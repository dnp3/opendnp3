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
#ifndef __STATICALLY_ALLOCATED_DATABASE_H_
#define __STATICALLY_ALLOCATED_DATABASE_H_

#include "opendnp3/outstation/StaticDataFacade.h"
#include <openpal/StaticArray.h>

namespace opendnp3
{

template <uint16_t NB, uint16_t NDB, uint16_t NA, uint16_t NC, uint16_t NFC, uint16_t NBOS, uint16_t NAOS>
class StaticallyAllocatedDatabase
{
public:	

	StaticDataFacade GetFacade()
	{
		BinaryCollection binaries(binaryValues.ToIndexable(), binaryMetadata.ToIndexable());
		DoubleBinaryCollection dobuleBinaries(doubleBinaryValues.ToIndexable(), doubleBinaryMetadata.ToIndexable());
		AnalogCollection analogs(analogValues.ToIndexable(), analogMetadata.ToIndexable());
		CounterCollection counters(counterValues.ToIndexable(), counterMetadata.ToIndexable());
		FrozenCounterCollection frozenCounters(frozenCounterValues.ToIndexable(), frozenCounterMetadata.ToIndexable());
		BinaryOutputStatusCollection binaryOutputStatii(binaryOutputStatusValues.ToIndexable(), binaryOutputStatusMetadata.ToIndexable());
		AnalogOutputStatusCollection analogOutputStatii(analogOutputStatusValues.ToIndexable(), analogOutputStatusMetadata.ToIndexable());

		return StaticDataFacade(
			binaries,
			dobuleBinaries,
			analogs,
			counters,
			frozenCounters,
			binaryOutputStatii,
			analogOutputStatii
		);
	}

private:

	openpal::StaticArray<DualValue<Binary>, uint16_t, NB> binaryValues;
	openpal::StaticArray<DualValue<DoubleBitBinary>, uint16_t, NDB> doubleBinaryValues;
	openpal::StaticArray<DualValue<Analog>, uint16_t, NA> analogValues;
	openpal::StaticArray<DualValue<Counter>, uint16_t, NC> counterValues;
	openpal::StaticArray<DualValue<FrozenCounter>, uint16_t, NFC> frozenCounterValues;
	openpal::StaticArray<DualValue<BinaryOutputStatus>, uint16_t, NBOS> binaryOutputStatusValues;
	openpal::StaticArray<DualValue<AnalogOutputStatus>, uint16_t, NAOS> analogOutputStatusValues;

	openpal::StaticArray<BinaryMetadata, uint16_t, NB> binaryMetadata;
	openpal::StaticArray<DoubleBinaryMetadata, uint16_t, NDB> doubleBinaryMetadata;
	openpal::StaticArray<AnalogMetadata, uint16_t, NA> analogMetadata;
	openpal::StaticArray<CounterMetadata, uint16_t, NC> counterMetadata;
	openpal::StaticArray<FrozenCounterMetadata, uint16_t, NFC> frozenCounterMetadata;
	openpal::StaticArray<BinaryOutputStatusMetadata, uint16_t, NBOS> binaryOutputStatusMetadata;
	openpal::StaticArray<AnalogOutputStatusMetadata, uint16_t, NAOS> analogOutputStatusMetadata;
};

}

#endif
