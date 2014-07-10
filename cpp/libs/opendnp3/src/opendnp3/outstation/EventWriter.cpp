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

#include "EventWriter.h"

#include "opendnp3/outstation/EventResponseTypes.h"

namespace opendnp3
{
	bool EventWriter::WriteEventHeaders(ObjectWriter& writer, SelectionCriteria& criteria, SelectionIterator& iterator, const EventResponseConfig& config)
	{		
		// seek to the first value
		iterator.SeekNext(criteria);

		while (iterator.HasValue())
		{
			auto numWritten = WriteOneHeader(writer, criteria, iterator, config);			
			if (numWritten == 0)
			{
				return false;
			}
		}

		return true;
	}

	uint32_t EventWriter::WriteOneHeader(ObjectWriter& writer, SelectionCriteria& criteria, SelectionIterator& iterator, const EventResponseConfig& config)
	{
		switch (iterator.GetValue())
		{
			case(EventType::Binary) :
				return WriteFullHeader<Binary>(writer, criteria, iterator, EventResponseTypes::Lookup(config.binary));

			case(EventType::DoubleBitBinary) :
				return WriteFullHeader<DoubleBitBinary>(writer, criteria, iterator, EventResponseTypes::Lookup(config.doubleBinary));

			case(EventType::Counter) :
				return WriteFullHeader<Counter>(writer, criteria, iterator, EventResponseTypes::Lookup(config.counter));

			case(EventType::FrozenCounter) :
				return WriteFullHeader<FrozenCounter>(writer, criteria, iterator, EventResponseTypes::Lookup(config.frozenCounter));

			case(EventType::Analog) :
				return WriteFullHeader<Analog>(writer, criteria, iterator, EventResponseTypes::Lookup(config.analog));

			case(EventType::BinaryOutputStatus) :
				return WriteFullHeader<BinaryOutputStatus>(writer, criteria, iterator, EventResponseTypes::Lookup(config.binaryOutputStatus));

			case(EventType::AnalogOutputStatus) :
				return WriteFullHeader<AnalogOutputStatus>(writer, criteria, iterator, EventResponseTypes::Lookup(config.analogOutputStatus));

			default:
				return 0;
		}
	}
}

