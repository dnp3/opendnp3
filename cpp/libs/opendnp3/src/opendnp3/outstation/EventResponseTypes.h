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
#ifndef OPENDNP3_EVENTRESPONSETYPES_H
#define OPENDNP3_EVENTRESPONSETYPES_H

#include "opendnp3/outstation/EventResponseConfig.h"

#include "opendnp3/outstation/EventWriteOperation.h"

#include <openpal/util/Uncopyable.h>

namespace opendnp3
{

/**
 * Function for converting enums to serializers
 */
class EventResponseTypes : openpal::PureStatic
{

public:

	static EventHeaderWriteFunc Lookup(EventBinaryResponse enumeration);
	static EventHeaderWriteFunc Lookup(EventDoubleBinaryResponse enumeration);
	static EventHeaderWriteFunc Lookup(EventCounterResponse enumeration);
	static EventHeaderWriteFunc Lookup(EventFrozenCounterResponse enumeration);
	static EventHeaderWriteFunc Lookup(EventAnalogResponse enumeration);
	static EventHeaderWriteFunc Lookup(EventBinaryOutputStatusResponse enumeration);
	static EventHeaderWriteFunc Lookup(EventAnalogOutputStatusResponse enumeration);
	
};

}

#endif

