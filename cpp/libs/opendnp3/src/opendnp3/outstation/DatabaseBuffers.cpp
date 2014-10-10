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

#include "DatabaseBuffers.h"

namespace opendnp3
{

DatabaseBuffers::DatabaseBuffers(const DatabaseTemplate& databaseTemplate):
	binaryValues(databaseTemplate.binaryIndexes.IndexCount()),
	doubleBinaryValues(databaseTemplate.doubleBinaryIndexes.IndexCount()),
	analogValues(databaseTemplate.analogIndexes.IndexCount()),
	counterValues(databaseTemplate.counterIndexes.IndexCount()),
	frozenCounterValues(databaseTemplate.frozenCounterIndexes.IndexCount()),
	binaryOutputStatusValues(databaseTemplate.binaryOutputStatusIndexes.IndexCount()),
	analogOutputStatusValues(databaseTemplate.analogOutputStatusIndexes.IndexCount()),
	timeAndIntervalValues(databaseTemplate.timeAndIntervalIndexes.IndexCount()),

	binaryMetadata(databaseTemplate.binaryIndexes.IndexCount()),
	doubleBinaryMetadata(databaseTemplate.doubleBinaryIndexes.IndexCount()),
	analogMetadata(databaseTemplate.analogIndexes.IndexCount()),
	counterMetadata(databaseTemplate.counterIndexes.IndexCount()),
	frozenCounterMetadata(databaseTemplate.frozenCounterIndexes.IndexCount()),
	binaryOutputStatusMetadata(databaseTemplate.binaryOutputStatusIndexes.IndexCount()),
	analogOutputStatusMetadata(databaseTemplate.analogOutputStatusIndexes.IndexCount()),

    binaryIndexes(databaseTemplate.binaryIndexes),
    doubleBinaryIndexes(databaseTemplate.doubleBinaryIndexes),
    analogIndexes(databaseTemplate.analogIndexes),
    counterIndexes(databaseTemplate.counterIndexes),
    frozenCounterIndexes(databaseTemplate.frozenCounterIndexes),
    binaryOutputStatusIndexes(databaseTemplate.binaryOutputStatusIndexes),
    analogOutputStatusIndexes(databaseTemplate.analogOutputStatusIndexes),
    timeAndIntervalIndexes(databaseTemplate.timeAndIntervalIndexes),
    
	binaries(binaryValues.ToIndexable(), binaryMetadata.ToIndexable(), binaryIndexes),
	doubleBinaries(doubleBinaryValues.ToIndexable(), doubleBinaryMetadata.ToIndexable(), doubleBinaryIndexes),
	analogs(analogValues.ToIndexable(), analogMetadata.ToIndexable(), analogIndexes),
	counters(counterValues.ToIndexable(), counterMetadata.ToIndexable(), counterIndexes),
	frozenCounters(frozenCounterValues.ToIndexable(), frozenCounterMetadata.ToIndexable(), frozenCounterIndexes),
	binaryOutputStatii(binaryOutputStatusValues.ToIndexable(), binaryOutputStatusMetadata.ToIndexable(), binaryOutputStatusIndexes),
	analogOutputStatii(analogOutputStatusValues.ToIndexable(), analogOutputStatusMetadata.ToIndexable(), analogOutputStatusIndexes),
	timeAndIntervals(timeAndIntervalValues.ToIndexable(), openpal::Indexable<EmptyMetadata, uint16_t>::Empty(), timeAndIntervalIndexes)    
{

}

}

