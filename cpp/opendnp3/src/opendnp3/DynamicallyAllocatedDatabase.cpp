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

namespace opendnp3
{

DynamicallyAllocatedDatabase::DynamicallyAllocatedDatabase(const DatabaseTemplate& databaseTemplate):
	binaryValues(databaseTemplate.numBinary),
	analogValues(databaseTemplate.numAnalog),
	counterValues(databaseTemplate.numCounter),
	controlStatusValues(databaseTemplate.numControlStatus),
	setpointStatusValues(databaseTemplate.numSetpointStatus),
	binaryMetadata(databaseTemplate.numBinary),
	analogMetadata(databaseTemplate.numAnalog),
	counterMetadata(databaseTemplate.numCounter)
{

}

void DynamicallyAllocatedDatabase::Configure(const DatabaseConfiguration& config)
{
	config.binaryMetadata.foreachIndex([this](const EventPointRecord& r, uint32_t i) { binaryMetadata[i].clazz = r.EventClass; });
	config.analogMetadata.foreachIndex([this](const DeadbandPointRecord<double>& r, uint32_t i) { 
		analogMetadata[i].clazz = r.EventClass;
		analogMetadata[i].deadband = r.Deadband;
	});
	config.counterMetadata.foreachIndex([this](const DeadbandPointRecord<uint32_t>& r, uint32_t i) { 
		counterMetadata[i].clazz = r.EventClass; 
		counterMetadata[i].deadband;
	});
}

StaticDataFacade DynamicallyAllocatedDatabase::GetFacade()
{
	BinaryCollection binaries(binaryValues.ToIndexable(), binaryMetadata.ToIndexable());
	AnalogCollection analogs(analogValues.ToIndexable(), analogMetadata.ToIndexable());
	CounterCollection counters(counterValues.ToIndexable(), counterMetadata.ToIndexable());

	return StaticDataFacade(
		binaries, 
		analogs, 
		counters, 
		controlStatusValues.ToIndexable(),
		setpointStatusValues.ToIndexable()
	);
}
	
}

