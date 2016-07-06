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
#ifndef OPENDNP3_MASTERPARAMS_H
#define OPENDNP3_MASTERPARAMS_H

#include <openpal/executor/TimeDuration.h>

#include "opendnp3/gen/TimeSyncMode.h"
#include "opendnp3/app/ClassField.h"

namespace opendnp3
{

/**
Configuration information for the dnp3 master
*/
struct MasterParams
{
	/// Default constructor
	MasterParams();

	/// Application layer response timeout
	openpal::TimeDuration responseTimeout;

	/// If true, the master will do time syncs when it sees the time IIN bit from the outstation
	TimeSyncMode timeSyncMode;

	/// If true, the master will disable unsol on startup for all 3 classes
	bool disableUnsolOnStartup;

	/// If true, the master will not clear the restart IIN bit in response to detecting it set
	bool ignoreRestartIIN;

	/// Bitwise mask used determine which classes are enabled for unsol, if 0 unsol is not enabled
	ClassField unsolClassMask;

	/// Which classes should be requested in a startup integrity scan, defaults to 3/2/1/0
	/// A mask equal to 0 means no startup integrity scan will be performed
	ClassField startupIntegrityClassMask;

	/// Defines whether an integrity scan will be performed when the EventBufferOverflow IIN is detected
	bool integrityOnEventOverflowIIN;

	/// Which classes should be requested in an event scan when detecting corresponding events available IIN
	ClassField eventScanOnEventsAvailableClassMask;

	/// Time delay before retrying a failed task
	openpal::TimeDuration taskRetryPeriod;

	/// Time delay before failing a non-recurring task (e.g. commands) that cannot start
	openpal::TimeDuration taskStartTimeout;

	/// maximum APDU tx size in bytes
	uint32_t maxTxFragSize;

	/// maximum APDU rx size in bytes
	uint32_t maxRxFragSize;
};

}

#endif

