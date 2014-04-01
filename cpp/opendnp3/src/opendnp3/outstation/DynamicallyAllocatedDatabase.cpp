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

#include "DynamicallyAllocatedDatabase.h"

#include <cstddef>

namespace opendnp3
{

DynamicallyAllocatedDatabase::DynamicallyAllocatedDatabase(const DatabaseTemplate& databaseTemplate):
	binaryValues(databaseTemplate.numBinary),
	doubleBinaryValues(databaseTemplate.numDoubleBinary),
	analogValues(databaseTemplate.numAnalog),
	counterValues(databaseTemplate.numCounter),
	frozenCounterValues(databaseTemplate.numFrozenCounter),
	binaryOutputStatusValues(databaseTemplate.numBinaryOutputStatus),
	analogOutputStatusValues(databaseTemplate.numAnalogOutputStatus),

	binaryMetadata(databaseTemplate.numBinary),
	doubleBinaryMetadata(databaseTemplate.numDoubleBinary),
	analogMetadata(databaseTemplate.numAnalog),
	counterMetadata(databaseTemplate.numCounter),
	frozenCounterMetadata(databaseTemplate.numFrozenCounter),
	binaryOutputStatusMetadata(databaseTemplate.numBinaryOutputStatus),
	analogOutputStatusMetadata(databaseTemplate.numAnalogOutputStatus)
{

}

StaticDataFacade DynamicallyAllocatedDatabase::GetFacade()
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

}

