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

#include "EventBufferConfig.h"

namespace opendnp3
{

EventBufferConfig::EventBufferConfig() :
	maxBinaryEvents(1000),
	maxAnalogEvents(1000),
	maxCounterEvents(1000),
	maxFrozenCounterEvents(100)
{}

EventBufferConfig::EventBufferConfig(
		uint32_t maxBinaryEvents_, 
		uint32_t maxDoubleBinaryEvents_,
		uint32_t maxAnalogEvents_,
		uint32_t maxCounterEvents_, 
		uint32_t maxFrozenCounterEvents_,
		uint32_t maxBinaryOutputStatusEvents_,
		uint32_t maxAnalogOutputStatusEvents_) :

	maxBinaryEvents(maxBinaryEvents_),
	maxDoubleBinaryEvents(maxDoubleBinaryEvents_),
	maxAnalogEvents(maxAnalogEvents_),
	maxCounterEvents(maxCounterEvents_),
	maxFrozenCounterEvents(maxFrozenCounterEvents_),
	maxBinaryOutputStatusEvents(maxBinaryOutputStatusEvents_),
	maxAnalogOutputStatusEvents(maxAnalogOutputStatusEvents_)
{

}

uint32_t EventBufferConfig::TotalEvents() const
{
	return maxBinaryEvents + 
		maxDoubleBinaryEvents + 
		maxAnalogEvents + 
		maxCounterEvents + 
		maxFrozenCounterEvents + 
		maxBinaryOutputStatusEvents + 
		maxAnalogOutputStatusEvents;
}


}

