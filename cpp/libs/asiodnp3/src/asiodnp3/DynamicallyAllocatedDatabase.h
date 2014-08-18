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
#ifndef __DYNAMICICALLY_ALLOCATED_DATABASE_H_
#define __DYNAMICICALLY_ALLOCATED_DATABASE_H_

#include <opendnp3/outstation/StaticDataFacade.h>
#include <opendnp3/outstation/DatabaseTemplate.h>
#include <opendnp3/app/DynamicPointIndexes.h>

#include <openpal/container/DynamicArray.h>

namespace asiodnp3
{

class DynamicallyAllocatedDatabase
{
public:

	DynamicallyAllocatedDatabase(const opendnp3::DatabaseTemplate& databaseTemplate);

	opendnp3::StaticDataFacade GetFacade();

private:

	openpal::DynamicArray<opendnp3::DualValue<opendnp3::Binary>, uint16_t> binaryValues;
	openpal::DynamicArray<opendnp3::DualValue<opendnp3::DoubleBitBinary>, uint16_t> doubleBinaryValues;
	openpal::DynamicArray<opendnp3::DualValue<opendnp3::Analog>, uint16_t> analogValues;
	openpal::DynamicArray<opendnp3::DualValue<opendnp3::Counter>, uint16_t> counterValues;
	openpal::DynamicArray<opendnp3::DualValue<opendnp3::FrozenCounter>, uint16_t> frozenCounterValues;
	openpal::DynamicArray<opendnp3::DualValue<opendnp3::BinaryOutputStatus>, uint16_t> binaryOutputStatusValues;
	openpal::DynamicArray<opendnp3::DualValue<opendnp3::AnalogOutputStatus>, uint16_t> analogOutputStatusValues;

	openpal::DynamicArray<opendnp3::BinaryMetadata, uint16_t> binaryMetadata;
	openpal::DynamicArray<opendnp3::DoubleBinaryMetadata, uint16_t> doubleBinaryMetadata;
	openpal::DynamicArray<opendnp3::AnalogMetadata, uint16_t> analogMetadata;
	openpal::DynamicArray<opendnp3::CounterMetadata, uint16_t> counterMetadata;
	openpal::DynamicArray<opendnp3::FrozenCounterMetadata, uint16_t> frozenCounterMetadata;
	openpal::DynamicArray<opendnp3::BinaryOutputStatusMetadata, uint16_t> binaryOutputStatusMetadata;
	openpal::DynamicArray<opendnp3::AnalogOutputStatusMetadata, uint16_t> analogOutputStatusMetadata;

    opendnp3::DynamicPointIndexes binaryIndexes;
    opendnp3::DynamicPointIndexes doubleBinaryIndexes;
    opendnp3::DynamicPointIndexes analogIndexes;
    opendnp3::DynamicPointIndexes counterIndexes;
    opendnp3::DynamicPointIndexes frozenCounterIndexes;
    opendnp3::DynamicPointIndexes binaryOutputStatusIndexes;
    opendnp3::DynamicPointIndexes analogOutputStatusIndexes;

};

}

#endif
