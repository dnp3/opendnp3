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

#include <openpal/container/StaticArray.h>
#include <openpal/container/LinkedListAdapter.h>
#include <openpal/container/StackAdapter.h>

#include "opendnp3/outstation/SOERecord.h"

namespace opendnp3
{

template <uint16_t MaxEvents>
class StaticallyAllocatedEventBuffer
{
public:

	EventBufferFacade GetFacade()
	{		
		openpal::LinkedListAdapter<SOERecord, uint32_t> soeAdapter(sequenceOfEvents.ToIndexable());
		openpal::StackAdapter<openpal::ListNode<SOERecord>*, uint32_t> selectionAdapter(selectedEvents.ToIndexable());

		return EventBufferFacade(			
			soeAdapter,
			selectionAdapter
		);
	}

private:

	openpal::StaticArray<openpal::ListNode<opendnp3::SOERecord>, uint32_t, MaxEvents> sequenceOfEvents;
	openpal::StaticArray<openpal::ListNode<opendnp3::SOERecord>*, uint32_t, MaxEvents> selectedEvents;
};

}

#endif
