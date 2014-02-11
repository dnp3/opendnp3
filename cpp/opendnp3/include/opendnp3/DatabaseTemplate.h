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

	static DatabaseTemplate AnalogOnly(uint32_t count)
	{
		return DatabaseTemplate(0, count, 0);
	}

	static DatabaseTemplate CounterOnly(uint32_t count)
	{
		return DatabaseTemplate(0, 0, count);
	}

	static DatabaseTemplate FrozenCounterOnly(uint32_t count)
	{
		return DatabaseTemplate(0, 0, 0, count);
	}

	DatabaseTemplate(uint32_t aNumBinary = 0,
	              uint32_t aNumAnalog = 0,
	              uint32_t aNumCounter = 0,
		      uint32_t aNumFrozenCounter = 0,
	              uint32_t aNumControlStatus = 0,
	              uint32_t aNumSetpointStatus = 0) :
		numBinary(aNumBinary),			
		numAnalog(aNumAnalog),
		numCounter(aNumCounter),
		numFrozenCounter(aNumFrozenCounter),
		numControlStatus(aNumControlStatus),
		numSetpointStatus(aNumSetpointStatus)
	{}

	uint32_t numBinary;
	uint32_t numAnalog;
	uint32_t numCounter;
	uint32_t numFrozenCounter;
	uint32_t numControlStatus;
	uint32_t numSetpointStatus;
};

}

#endif
