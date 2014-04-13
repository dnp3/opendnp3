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
#ifndef __OUTSTATION_PARAMS_H_
#define __OUTSTATION_PARAMS_H_

#include <openpal/TimeDuration.h>

namespace opendnp3
{

struct OutstationParams
{
	OutstationParams();

	/// The maximum number of controls the outstation will attempt to process from a single APDU
	uint8_t maxControlsPerRequest;			

	/// How long the outstation will allow an operate to proceed after a prior select
	openpal::TimeDuration selectTimeout;

	/// The maximum fragment size the outstation will use for data it sends
	uint32_t maxTxFragSize;
};

}

#endif
