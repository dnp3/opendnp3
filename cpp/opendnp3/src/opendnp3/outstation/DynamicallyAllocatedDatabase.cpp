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
	analogValues(databaseTemplate.numAnalog),
	counterValues(databaseTemplate.numCounter),
	frozenCounterValues(databaseTemplate.numFrozenCounter),
	BinaryOutputStatusValues(databaseTemplate.numBinaryOutputStatus),
	AnalogOutputStatusValues(databaseTemplate.numAnalogOutputStatus),
	binaryMetadata(databaseTemplate.numBinary),
	analogMetadata(databaseTemplate.numAnalog),
	counterMetadata(databaseTemplate.numCounter),
	frozenCounterMetadata(databaseTemplate.numFrozenCounter)
{

}

void DynamicallyAllocatedDatabase::Configure(const DatabaseConfiguration& config)
{
	for(size_t i = 0; i < config.binaryMetadata.size(); ++i)
	{
		binaryMetadata[i].clazz = config.binaryMetadata[i].EventClass;
	}

	for(size_t i = 0; i < config.analogMetadata.size(); ++i)
	{
		analogMetadata[i].clazz = config.analogMetadata[i].EventClass;
		analogMetadata[i].deadband = config.analogMetadata[i].Deadband;
	}

	for(size_t i = 0; i < config.counterMetadata.size(); ++i)
	{
		counterMetadata[i].clazz = config.counterMetadata[i].EventClass;
		counterMetadata[i].deadband = config.counterMetadata[i].Deadband;
	}

	for(size_t i = 0; i < config.frozenCounterMetadata.size(); ++i)
	{
		frozenCounterMetadata[i].clazz = config.frozenCounterMetadata[i].EventClass;
		frozenCounterMetadata[i].deadband = config.frozenCounterMetadata[i].Deadband;
	}
}

StaticDataFacade DynamicallyAllocatedDatabase::GetFacade()
{
	BinaryCollection binaries(binaryValues.ToIndexable(), binaryMetadata.ToIndexable());
	AnalogCollection analogs(analogValues.ToIndexable(), analogMetadata.ToIndexable());
	CounterCollection counters(counterValues.ToIndexable(), counterMetadata.ToIndexable());
	FrozenCounterCollection frozenCounters(frozenCounterValues.ToIndexable(), frozenCounterMetadata.ToIndexable());

	return StaticDataFacade(
	           binaries,
	           analogs,
	           counters,
	           frozenCounters,
	           BinaryOutputStatusValues.ToIndexable(),
	           AnalogOutputStatusValues.ToIndexable()
	       );
}

}

