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

#ifndef __STATICALLY_ALLOCATED_EVENT_BUFFER_
#define __STATICALLY_ALLOCATED_EVENT_BUFFER_

#include "EventBufferFacade.h"
#include <openpal/StaticArray.h>

namespace opendnp3
{

template <uint32_t B, uint32_t A, uint32_t C>
class StaticallyAllocatedEventBuffer
{
	public:		

		//EventBufferFacade GetFacade();

	private:

		openpal::StaticArray<uint16_t, uint16_t, B> binaryStack;
		openpal::StaticArray<Event<Binary>, uint16_t, B> binaryArray;

		openpal::StaticArray<uint16_t, uint16_t, A> analogStack;
		openpal::StaticArray<Event<Analog>, uint16_t, A> analogArray;

		openpal::StaticArray<uint16_t, uint16_t, C> counterStack;
		openpal::StaticArray<Event<Counter>, uint16_t, C> counterArray;

		openpal::StaticArray<openpal::DoubleListNode<SequenceRecord>, uint16_t, B+A+C> sequenceOfEvents;
		openpal::StaticArray<openpal::DoubleListNode<SequenceRecord>*, uint16_t, B+A+C> selectedEvents;		
};

}

#endif
