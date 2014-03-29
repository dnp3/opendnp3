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

#ifndef __EVENT_TYPE_H_
#define __EVENT_TYPE_H_

#include <openpal/Uncopyable.h>

#include <cstdint>

namespace opendnp3
{

enum class EventType : uint32_t
{
    Binary = 1,
    Analog = 1 << 1,
    Counter = 1 << 2,	
    FrozenCounter =	1 << 3,
	DoubleBitBinary = 1 << 4,
	BinaryOutputStatus =  1 << 5,
	AnalogOutputStatus  = 1 << 6	
};

enum class EventClass
{
    EC1 = 1,
    EC2 = 1 << 1,
    EC3 = 1 << 2
};

namespace events {

	const uint32_t BINARY = static_cast<uint32_t>(EventType::Binary);
	const uint32_t ANALOG = static_cast<uint32_t>(EventType::Analog);
	const uint32_t COUNTER = static_cast<uint32_t>(EventType::Counter);
	const uint32_t FROZEN_COUNTER = static_cast<uint32_t>(EventType::FrozenCounter);	
	const uint32_t DOUBLE_BIT_BINARY = static_cast<uint32_t>(EventType::DoubleBitBinary);
	const uint32_t BINARY_OUTPUT_STATUS = static_cast<uint32_t>(EventType::BinaryOutputStatus);
	const uint32_t ANALOG_OUTPUT_STATUS = static_cast<uint32_t>(EventType::AnalogOutputStatus);	

	static const uint32_t NONE = 0;
	static const uint32_t ALL_TYPES = ~NONE;
}

}

#endif
