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
#ifndef __EVENT_RESPONSE_TYPES_H_
#define __EVENT_RESPONSE_TYPES_H_

#include "opendnp3/gen/EventBinaryResponse.h"
#include "opendnp3/gen/EventDoubleBinaryResponse.h"
#include "opendnp3/gen/EventCounterResponse.h"
#include "opendnp3/gen/EventAnalogResponse.h"
#include "opendnp3/gen/EventFrozenCounterResponse.h"
#include "opendnp3/gen/EventBinaryOutputStatusResponse.h"
#include "opendnp3/gen/EventAnalogOutputStatusResponse.h"

#include "opendnp3/app/IDNP3Serializer.h"
#include "opendnp3/app/MeasurementTypes.h"

#include <openpal/util/Uncopyable.h>

namespace opendnp3
{

/**
 * Function for converting enums to serializers
 */
class EventResponseTypes : openpal::PureStatic
{

public:

	static IDNP3Serializer<Binary>& Lookup(EventBinaryResponse enumeration);
	static IDNP3Serializer<DoubleBitBinary>& Lookup(EventDoubleBinaryResponse enumeration);
	static IDNP3Serializer<Counter>& Lookup(EventCounterResponse enumeration);
	static IDNP3Serializer<FrozenCounter>& Lookup(EventFrozenCounterResponse enumeration);
	static IDNP3Serializer<Analog>& Lookup(EventAnalogResponse enumeration);
	static IDNP3Serializer<BinaryOutputStatus>& Lookup(EventBinaryOutputStatusResponse enumeration);
	static IDNP3Serializer<AnalogOutputStatus>& Lookup(EventAnalogOutputStatusResponse enumeration);
	
};

}

#endif

