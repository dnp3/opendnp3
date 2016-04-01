/*
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

namespace opendnp3
{

/**
* Specifies the number and type of measurements in an outstation database.
*/
struct DatabaseTemplate
{
	static DatabaseTemplate BinaryOnly(uint16_t count)
	{
		return DatabaseTemplate(count);
	}

	static DatabaseTemplate DoubleBinaryOnly(uint16_t count)
	{
		return DatabaseTemplate(0, count);
	}

	static DatabaseTemplate AnalogOnly(uint16_t count)
	{
		return DatabaseTemplate(0, 0, count);
	}

	static DatabaseTemplate CounterOnly(uint16_t count)
	{
		return DatabaseTemplate(0, 0, 0, count);
	}

	static DatabaseTemplate FrozenCounterOnly(uint16_t count)
	{
		return DatabaseTemplate(0, 0, 0, 0, count);
	}

	static DatabaseTemplate BinaryOutputStatusOnly(uint16_t count)
	{
		return DatabaseTemplate(0, 0, 0, 0, 0, count);
	}

	static DatabaseTemplate AnalogOutputStatusOnly(uint16_t count)
	{
		return DatabaseTemplate(0, 0, 0, 0, 0, 0, count);
	}

	static DatabaseTemplate TimeAndIntervalOnly(uint16_t count)
	{
		return DatabaseTemplate(0, 0, 0, 0, 0, 0, 0, count);
	}

	static DatabaseTemplate AllTypes(uint16_t count)
	{
		return DatabaseTemplate(count, count, count, count, count, count, count);
	}

	DatabaseTemplate(uint16_t numBinary_ = 0,
	                 uint16_t numDoubleBinary_ = 0,
	                 uint16_t numAnalog_ = 0,
	                 uint16_t numCounter_ = 0,
	                 uint16_t numFrozenCounter_ = 0,
	                 uint16_t numBinaryOutputStatus_ = 0,
	                 uint16_t numAnalogOutputStatus_ = 0,
	                 uint16_t numTimeAndInterval_ = 0) :

		numBinary(numBinary_),
		numDoubleBinary(numDoubleBinary_),
		numAnalog(numAnalog_),
		numCounter(numCounter_),
		numFrozenCounter(numFrozenCounter_),
		numBinaryOutputStatus(numBinaryOutputStatus_),
		numAnalogOutputStatus(numAnalogOutputStatus_),
		numTimeAndInterval(numTimeAndInterval_)		
	{}

	uint16_t numBinary;
	uint16_t numDoubleBinary;
	uint16_t numAnalog;
	uint16_t numCounter;
	uint16_t numFrozenCounter;
	uint16_t numBinaryOutputStatus;
	uint16_t numAnalogOutputStatus;
	uint16_t numTimeAndInterval;	

};

}

#endif
