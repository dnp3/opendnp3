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

#include "EventStorage.h"

namespace opendnp3
{

EventStorage::EventStorage(const EventBufferConfig& config) :
	events(config.TotalEvents()),
	binary(config.maxBinaryEvents),
	doubleBinary(config.maxDoubleBinaryEvents),
	analog(config.maxAnalogEvents),
	counter(config.maxCounterEvents),
	frozenCounter(config.maxFrozenCounterEvents),
	binaryOutputStatus(config.maxBinaryOutputStatusEvents),
	analogOutputStatus(config.maxAnalogOutputStatusEvents)
{}

uint32_t EventStorage::Write(EventWriteHandler& handler)
{
	// iterate over the selected events in the buffer
	auto iterator = this->events.Iterate();

	uint32_t total_num_written = 0;
	while (true)
	{
		uint16_t num_written = this->WriteSome(handler, iterator);
		if (num_written == 0)
		{
			return total_num_written;
		}
		else
		{
			total_num_written += num_written;
		}
	}
	
	return total_num_written;
}

uint16_t EventStorage::WriteSome(EventWriteHandler& handler, event_iterator_t& iterator)
{
	const auto next = iterator.Find(EventStorage::IsSelected);
	
	// we are out of selected events
	if (!next) return 0;

	// now enter a type-dependent write routine
	switch (next->type)
	{
	case(EventType::Binary):
		return WriteSomeOfType<BinaryInfo>(handler, iterator, *next);
	default:
		// this case should never happen, terminate
		return 0;
	}
}

}


