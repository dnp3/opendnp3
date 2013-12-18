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

#include "OutstationEventBuffer.h"

namespace opendnp3
{

OutstationEventBuffer::OutstationEventBuffer(const EventBufferFacade& aFacade) :
	overflow(false),
	facade(aFacade)
{

}

void OutstationEventBuffer::OnTransmitFailure()
{
	facade.selectedEvents.Clear();
}

void OutstationEventBuffer::OnTransmitSuccess()
{
	while(facade.selectedEvents.IsNotEmpty()) // guaranteed that 
	{
		auto index = facade.selectedEvents.Pop(); // that will be the biggest index =(
		// we need a linked list facade!! This will allow us to delete O(1)!!!
	}	
}

void OutstationEventBuffer::Update(const Event<Binary>& aEvent)
{
	overflow |= !InsertEvent(aEvent, EventType::Binary, facade.binaryEvents);
}

void OutstationEventBuffer::Update(const Event<Analog>& aEvent)
{
	overflow |= !InsertEvent(aEvent, EventType::Analog,  facade.analogEvents);
}

void OutstationEventBuffer::Update(const Event<Counter>& aEvent)
{
	overflow |= !InsertEvent(aEvent, EventType::Counter,  facade.counterEvents);
}

}
