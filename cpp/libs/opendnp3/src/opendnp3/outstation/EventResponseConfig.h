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
#ifndef OPENDNP3_EVENTRESPONSECONFIG_H
#define OPENDNP3_EVENTRESPONSECONFIG_H

#include "opendnp3/gen/EventAnalogVariation.h"
#include "opendnp3/gen/EventBinaryVariation.h"
#include "opendnp3/gen/EventDoubleBinaryVariation.h"
#include "opendnp3/gen/EventCounterVariation.h"
#include "opendnp3/gen/EventFrozenCounterVariation.h"
#include "opendnp3/gen/EventBinaryOutputStatusVariation.h"
#include "opendnp3/gen/EventAnalogOutputStatusVariation.h"

namespace opendnp3
{

struct EventResponseConfig
{
	EventResponseConfig();

	/// The default group/variation to use for binary events
	EventBinaryVariation binary;

	/// The default group/variation to use for double-bit binary events
	EventDoubleBinaryVariation doubleBinary;

	/// The default group/variation to use for analog events
	EventAnalogVariation analog;

	/// The default group/variation to use for counter events
	EventCounterVariation counter;

	/// The default group/variation to use for frozen counter events
	EventFrozenCounterVariation frozenCounter;

	/// The default group/variation to use for binary output status events
	EventBinaryOutputStatusVariation binaryOutputStatus;

	/// The default group/variation to use for binary output status events
	EventAnalogOutputStatusVariation analogOutputStatus;


};

}



#endif

