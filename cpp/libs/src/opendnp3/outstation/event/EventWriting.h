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
#ifndef OPENDNP3_EVENTWRITING_H
#define OPENDNP3_EVENTWRITING_H

#include "IEventWriteHandler.h"

#include "EventLists.h"

namespace opendnp3
{

class EventWriting : private openpal::StaticOnly
{

public:

	typedef openpal::LinkedListIterator<EventRecord> event_iter_t;

	static uint32_t Write(EventLists& lists, IEventWriteHandler& handler);

	static EventRecord* FindNextSelected(event_iter_t& iter, EventType type);

private:

	

	static uint16_t WriteSome(event_iter_t& iterator, EventLists& lists, IEventWriteHandler& handler);

	template <class T>
	static uint16_t WriteSomeOfType(event_iter_t& iterator, EventLists& lists, IEventWriteHandler& handler);
};

}

#endif

