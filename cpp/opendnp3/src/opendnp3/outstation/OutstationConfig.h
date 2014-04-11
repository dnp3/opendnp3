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
#ifndef __SLAVE_CONFIG_H_
#define __SLAVE_CONFIG_H_

#include <assert.h>


#include <openpal/TimeDuration.h>

#include "opendnp3/app/ClassMask.h"

#include "opendnp3/gen/EventAnalogResponse.h"
#include "opendnp3/gen/EventBinaryResponse.h"
#include "opendnp3/gen/EventCounterResponse.h"

#include "opendnp3/gen/StaticAnalogResponse.h"
#include "opendnp3/gen/StaticBinaryResponse.h"
#include "opendnp3/gen/StaticCounterResponse.h"
#include "opendnp3/gen/StaticFrozenCounterResponse.h"
#include "opendnp3/gen/StaticBinaryOutputStatusResponse.h"
#include "opendnp3/gen/StaticAnalogOutputStatusResponse.h"

namespace opendnp3
{

/** Configuration information for a dnp3 outstation (outstation)

Used as both input describing the startup configuration of the outstation, and as configuration state of mutable properties (i.e. unsolicited responses).

Major feature areas are unsolicited responses, time synchronization requests, event buffer limits, and the DNP3 object/variations to use by default
when the master requests class data or variation 0.

*/
struct OutstationConfig
{
	OutstationConfig();

	/// The maximum number of controls the outstation will attempt to process from a single APDU
	uint8_t maxControls;

	/// if true, fully disables unsolicited mode as if the outstation didn't support it
	bool disableUnsol;

	/// controls what unsol classes are enabled
	ClassMask unsolMask;

	/// if true, the outstation will request time synchronization on an interval
	bool allowTimeSync;


	/// The period of time sync interval in milliseconds
	openpal::TimeDuration timeSyncPeriod;

	/// The amount of time the outstation will wait before sending new unsolicited data ( <= 0 == immediate)
	openpal::TimeDuration unsolPackDelay;

	/// How long the outstation will wait before retrying an unsuccessful unsol response
	openpal::TimeDuration unsolRetryDelay;

	/// How long the outstation will allow an operate to proceed after a prior select
	openpal::TimeDuration selectTimeout;

	/// The maximum fragment size the outstation will use for data it sends
	uint32_t maxFragSize;

	// -------------  default static response types ------------------

	/// The default group/variation to use for static binary responses
	StaticBinaryResponse staticBinary;

	/// The default group/variation to use for static analog responses
	StaticAnalogResponse staticAnalog;

	/// The default group/variation to use for static counter responses
	StaticCounterResponse staticCounter;

	/// The default group/variation to use for static counter responses
	StaticFrozenCounterResponse staticFrozenCounter;

	/// The default group/variation to use for static control status responses
	StaticBinaryOutputStatusResponse staticBinaryOutputStatus;

	/// The default group/variation to use for static setpoint status responses
	StaticAnalogOutputStatusResponse staticAnalogOutputStatus;

	// default event response types

	/// The default group/variation to use for binary event responses
	EventBinaryResponse eventBinary;

	/// The default group/variation to use for analog event responses
	EventAnalogResponse eventAnalog;

	/// The default group/variation to use for counter event responses
	EventCounterResponse eventCounter;


};

}



#endif

