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
#ifndef OPENDNP3_DATABASETEMPLATE_H
#define OPENDNP3_DATABASETEMPLATE_H

#include <cstdint>

#include "opendnp3/app/PointIndexes.h"

namespace opendnp3
{

// template for specifying the size of types in an outstation database
struct DatabaseTemplate
{
	static DatabaseTemplate BinaryOnly(uint32_t count)
	{
		return DatabaseTemplate(count);
	}

	static DatabaseTemplate DoubleBinaryOnly(uint32_t count)
	{
		return DatabaseTemplate(0, count);
	}

	static DatabaseTemplate AnalogOnly(uint32_t count)
	{
		return DatabaseTemplate(0, 0, count);
	}

	static DatabaseTemplate CounterOnly(uint32_t count)
	{
		return DatabaseTemplate(0, 0, 0, count);
	}

	static DatabaseTemplate FrozenCounterOnly(uint32_t count)
	{
		return DatabaseTemplate(0, 0, 0, 0, count);
	}

	static DatabaseTemplate BinaryOutputStatusOnly(uint32_t count)
	{
		return DatabaseTemplate(0, 0, 0, 0, 0, count);
	}

	static DatabaseTemplate AnalogOutputStatusOnly(uint32_t count)
	{
		return DatabaseTemplate(0, 0, 0, 0, 0, 0, count);
	}

	static DatabaseTemplate AllTypes(uint32_t count)
	{
		return DatabaseTemplate(count, count, count, count, count, count, count);
	}

	DatabaseTemplate(uint32_t numBinary_,
	                 uint32_t numDoubleBinary_ = 0,
	                 uint32_t numAnalog_ = 0,
	                 uint32_t numCounter_ = 0,
	                 uint32_t numFrozenCounter_ = 0,
	                 uint32_t numBinaryOutputStatus_ = 0,
	                 uint32_t numAnalogOutputStatus_ = 0) :
    
        binaryRange(numBinary_),
        doubleBinaryRange(numDoubleBinary_),
        analogRange(numAnalog_),
        counterRange(numCounter_),
        frozenCounterRange(numFrozenCounter_),
        binaryOutputStatusRange(numBinaryOutputStatus_),
        analogOutputStatusRange(numAnalogOutputStatus_),

        binaryIndexes(openpal::Indexable<PointRange, uint32_t>(&binaryRange,1)),
        doubleBinaryIndexes(openpal::Indexable<PointRange, uint32_t>(&doubleBinaryRange,1)),
        analogIndexes(openpal::Indexable<PointRange, uint32_t>(&analogRange,1)),
        counterIndexes(openpal::Indexable<PointRange, uint32_t>(&counterRange,1)),
        frozenCounterIndexes(openpal::Indexable<PointRange, uint32_t>(&frozenCounterRange,1)),
        binaryOutputStatusIndexes(openpal::Indexable<PointRange, uint32_t>(&binaryOutputStatusRange,1)),
        analogOutputStatusIndexes(openpal::Indexable<PointRange, uint32_t>(&analogOutputStatusRange,1)),

		numBinary(numBinary_),
		numDoubleBinary(numDoubleBinary_),
		numAnalog(numAnalog_),
		numCounter(numCounter_),
		numFrozenCounter(numFrozenCounter_),
		numBinaryOutputStatus(numBinaryOutputStatus_),
		numAnalogOutputStatus(numAnalogOutputStatus_)
	{}
    
    DatabaseTemplate(PointIndexes binaryIndexes_ = PointIndexes::EMPTYINDEXES,
	                 PointIndexes doubleBinaryIndexes_ = PointIndexes::EMPTYINDEXES,
	                 PointIndexes analogIndexes_ = PointIndexes::EMPTYINDEXES,
	                 PointIndexes counterIndexes_ = PointIndexes::EMPTYINDEXES,
	                 PointIndexes frozenCounterIndexes_ = PointIndexes::EMPTYINDEXES,
	                 PointIndexes binaryOutputStatusIndexes_ = PointIndexes::EMPTYINDEXES,
	                 PointIndexes analogOutputStatusIndexes_ = PointIndexes::EMPTYINDEXES) :
    
    binaryRange(binaryIndexes_.ToRange()),
    doubleBinaryRange(doubleBinaryIndexes_.ToRange()),
    analogRange(analogIndexes_.ToRange()),
    counterRange(counterIndexes_.ToRange()),
    frozenCounterRange(frozenCounterIndexes_.ToRange()),
    binaryOutputStatusRange(binaryOutputStatusIndexes_.ToRange()),
    analogOutputStatusRange(analogOutputStatusIndexes_.ToRange()),
    
	binaryIndexes(binaryIndexes_),
    doubleBinaryIndexes(doubleBinaryIndexes_),
    analogIndexes(analogIndexes_),
    counterIndexes(counterIndexes_),
    frozenCounterIndexes(frozenCounterIndexes_),
    binaryOutputStatusIndexes(binaryOutputStatusIndexes_),
    analogOutputStatusIndexes(analogOutputStatusIndexes_),

	numBinary(binaryIndexes_.IndexCount()),
	numDoubleBinary(doubleBinaryIndexes_.IndexCount()),
	numAnalog(analogIndexes_.IndexCount()),
	numCounter(counterIndexes_.IndexCount()),
	numFrozenCounter(frozenCounterIndexes_.IndexCount()),
	numBinaryOutputStatus(binaryOutputStatusIndexes_.IndexCount()),
	numAnalogOutputStatus(analogOutputStatusIndexes_.IndexCount())
	{}
    
    PointRange binaryRange;
    PointRange doubleBinaryRange;
    PointRange analogRange;
    PointRange counterRange;
    PointRange frozenCounterRange;
    PointRange binaryOutputStatusRange;
    PointRange analogOutputStatusRange;

    PointIndexes binaryIndexes;
    PointIndexes doubleBinaryIndexes;
    PointIndexes analogIndexes;
    PointIndexes counterIndexes;
    PointIndexes frozenCounterIndexes;
    PointIndexes binaryOutputStatusIndexes;
    PointIndexes analogOutputStatusIndexes;

	uint32_t numBinary;
	uint32_t numDoubleBinary;
	uint32_t numAnalog;
	uint32_t numCounter;
	uint32_t numFrozenCounter;
	uint32_t numBinaryOutputStatus;
	uint32_t numAnalogOutputStatus;

};

}

#endif
