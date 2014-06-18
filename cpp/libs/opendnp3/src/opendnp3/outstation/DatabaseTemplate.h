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
#ifndef __DATABASE_TEMPLATE_H_
#define __DATABASE_TEMPLATE_H_

#include <cstdint>

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

	DatabaseTemplate(uint32_t numBinary_ = 0,
	                 uint32_t numDoubleBinary_ = 0,
	                 uint32_t numAnalog_ = 0,
	                 uint32_t numCounter_ = 0,
	                 uint32_t numFrozenCounter_ = 0,
	                 uint32_t numBinaryOutputStatus_ = 0,
	                 uint32_t numAnalogOutputStatus_ = 0) :

		numBinary(numBinary_),
		numDoubleBinary(numDoubleBinary_),
		numAnalog(numAnalog_),
		numCounter(numCounter_),
		numFrozenCounter(numFrozenCounter_),
		numBinaryOutputStatus(numBinaryOutputStatus_),
		numAnalogOutputStatus(numAnalogOutputStatus_)
	{}

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
