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

#include "Uncopyable.h"

#include <cstdint>

namespace opendnp3
{

enum class EventType: uint8_t
{
	Binary = 0x01,
	Analog = 0x02,
	Counter = 0x04,
	FrozenCounter = 0x05
};

enum class EventClass: uint8_t
{
	EC1 = 0x01,
	EC2 = 0x02,
	EC3 = 0x04
};

struct EventTypeMasks : private PureStatic
{
	static const uint8_t BINARY = static_cast<uint8_t>(EventType::Binary);
	static const uint8_t ANALOG = static_cast<uint8_t>(EventType::Analog);
	static const uint8_t COUNTER = static_cast<uint8_t>(EventType::Counter);
	static const uint8_t FROZEN_COUNTER = static_cast<uint8_t>(EventType::FrozenCounter);

	static const uint8_t ALL_TYPES = BINARY | ANALOG | COUNTER | FROZEN_COUNTER;
};

}

#endif
