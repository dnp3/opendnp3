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
#ifndef OPENDNP3_OUTSTATIONPARAMS_H
#define OPENDNP3_OUTSTATIONPARAMS_H

#include <openpal/executor/TimeDuration.h>

#include "opendnp3/app/ClassField.h"

namespace opendnp3
{

struct OutstationParams
{
	OutstationParams();

	/// The maximum number of controls the outstation will attempt to process from a single APDU
	uint8_t maxControlsPerRequest;			

	/// How long the outstation will allow an operate to proceed after a prior select
	openpal::TimeDuration selectTimeout;

	/// Timeout for solicited confirms
	openpal::TimeDuration solConfirmTimeout;

	/// Timeout for unsolicited confirms
	openpal::TimeDuration unsolConfirmTimeout;

	/// Timeout for unsolicited retries
	openpal::TimeDuration unsolRetryTimeout;

	/// The maximum fragment size the outstation will use for fragments it sends
	uint32_t maxTxFragSize;

	/// The maximum fragment size the outstation will be able to receive
	uint32_t maxRxFragSize;

	/// Global enabled / disable for unsolicted messages. If false, the NULL unsolicited message is not even sent
	bool allowUnsolicited;

	/// Class mask for unsolicted, default to 0 as unsolicited has to be enabled
	ClassField unsolClassMask;


};

}

#endif
