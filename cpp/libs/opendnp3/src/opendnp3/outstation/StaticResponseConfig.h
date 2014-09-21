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
#ifndef OPENDNP3_STATICRESPONSECONFIG_H
#define OPENDNP3_STATICRESPONSECONFIG_H

#include <assert.h>

#include "opendnp3/gen/StaticAnalogResponse.h"
#include "opendnp3/gen/StaticBinaryResponse.h"
#include "opendnp3/gen/StaticDoubleBinaryResponse.h"
#include "opendnp3/gen/StaticCounterResponse.h"
#include "opendnp3/gen/StaticFrozenCounterResponse.h"
#include "opendnp3/gen/StaticBinaryOutputStatusResponse.h"
#include "opendnp3/gen/StaticAnalogOutputStatusResponse.h"

namespace opendnp3
{

struct StaticResponseConfig
{
	StaticResponseConfig();

	/// The default group/variation to use for static binary responses
	StaticBinaryResponse binary;

	/// The default group/variation to use for static double-bit binary responses
	StaticDoubleBinaryResponse doubleBinary;

	/// The default group/variation to use for static analog responses
	StaticAnalogResponse analog;

	/// The default group/variation to use for static counter responses
	StaticCounterResponse counter;

	/// The default group/variation to use for static counter responses
	StaticFrozenCounterResponse frozenCounter;

	/// The default group/variation to use for static control status responses
	StaticBinaryOutputStatusResponse binaryOutputStatus;

	/// The default group/variation to use for static setpoint status responses
	StaticAnalogOutputStatusResponse analogOutputStatus;
};

}



#endif

