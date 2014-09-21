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
#ifndef OPENDNP3_EVENT_H
#define OPENDNP3_EVENT_H

#include "opendnp3/app/IndexedValue.h"
#include "opendnp3/app/EventType.h"

namespace opendnp3
{

struct Evented : public Indexed<uint16_t>
{
	Evented(uint16_t index_, EventClass clazz_) : Indexed<uint16_t>(index_), clazz(clazz_)
	{}

	Evented() : clazz(EventClass::EC1)
	{}

	EventClass clazz;	// class of the event (CLASS<1-3>)
};

/**
 * Record of an event that includes value, index, and class
 */
template <typename ValueType>
struct Event : public Evented
{
	Event(const ValueType& value_, uint16_t index_, EventClass clazz_) :
		Evented(index_, clazz_),
		value(value_)
	{}

	Event() : Evented(), value()
	{}

	ValueType value;
};

} //end namespace



#endif

