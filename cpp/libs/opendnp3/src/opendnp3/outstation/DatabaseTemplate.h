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
// TODO - change these to 16-bit
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

	static DatabaseTemplate TimeAndIntervalOnly(uint32_t count)
	{
		return DatabaseTemplate(0, 0, 0, 0, 0, 0, 0, count);
	}

	static DatabaseTemplate AllTypes(uint32_t count)
	{
		return DatabaseTemplate(count, count, count, count, count, count, count, count);
	}

	DatabaseTemplate(uint32_t numBinary_,
	                 uint32_t numDoubleBinary_ = 0,
	                 uint32_t numAnalog_ = 0,
	                 uint32_t numCounter_ = 0,
	                 uint32_t numFrozenCounter_ = 0,
	                 uint32_t numBinaryOutputStatus_ = 0,
	                 uint32_t numAnalogOutputStatus_ = 0,
					 uint32_t numTimeAndInterval_ = 0) :
    
        binaryIndexes(numBinary_),
        doubleBinaryIndexes(numDoubleBinary_),
        analogIndexes(numAnalog_),
        counterIndexes(numCounter_),
        frozenCounterIndexes(numFrozenCounter_),
        binaryOutputStatusIndexes(numBinaryOutputStatus_),
        analogOutputStatusIndexes(numAnalogOutputStatus_),
        timeAndIntervalIndexes(numTimeAndInterval_)
	{}
    
    DatabaseTemplate(PointIndexes binaryIndexes_ = PointIndexes::EMPTYINDEXES,
	                 PointIndexes doubleBinaryIndexes_ = PointIndexes::EMPTYINDEXES,
	                 PointIndexes analogIndexes_ = PointIndexes::EMPTYINDEXES,
	                 PointIndexes counterIndexes_ = PointIndexes::EMPTYINDEXES,
	                 PointIndexes frozenCounterIndexes_ = PointIndexes::EMPTYINDEXES,
	                 PointIndexes binaryOutputStatusIndexes_ = PointIndexes::EMPTYINDEXES,
	                 PointIndexes analogOutputStatusIndexes_ = PointIndexes::EMPTYINDEXES,
                     PointIndexes timeAndIntervalIndexes_ = PointIndexes::EMPTYINDEXES) :
    
	binaryIndexes(binaryIndexes_),
    doubleBinaryIndexes(doubleBinaryIndexes_),
    analogIndexes(analogIndexes_),
    counterIndexes(counterIndexes_),
    frozenCounterIndexes(frozenCounterIndexes_),
    binaryOutputStatusIndexes(binaryOutputStatusIndexes_),
    analogOutputStatusIndexes(analogOutputStatusIndexes_),
    timeAndIntervalIndexes(timeAndIntervalIndexes_)
	{}
    
    PointIndexes binaryIndexes;
    PointIndexes doubleBinaryIndexes;
    PointIndexes analogIndexes;
    PointIndexes counterIndexes;
    PointIndexes frozenCounterIndexes;
    PointIndexes binaryOutputStatusIndexes;
    PointIndexes analogOutputStatusIndexes;
    PointIndexes timeAndIntervalIndexes;
};

}

#endif
