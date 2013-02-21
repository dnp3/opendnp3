
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the GNU Affero General Public License Version 3.0
// (the "Additional License"). You may not use these modifications except in
// compliance with the additional license. You may obtain a copy of the Additional
// License at
//
// http://www.gnu.org/licenses/agpl.html
//
// Contact Automatak, LLC for a commercial license to these modifications
//
#ifndef __SLAVE_CONFIG_H_
#define __SLAVE_CONFIG_H_

#include <assert.h>

#include "Exception.h"
#include "ClassMask.h"
#include "ObjectInterfaces.h"

namespace opendnp3
{

/// Group/Variation pair
struct GrpVar {
	GrpVar() : Grp(-1), Var(-1) {}
	GrpVar(int aGrp, int aVar) : Grp(aGrp), Var(aVar) {}

	int Grp;
	int Var;
};

/// Configuration of max event counts
struct EventMaxConfig {
	EventMaxConfig();

	EventMaxConfig(size_t, size_t, size_t, size_t);

	/// The number of binary events the slave will buffer before overflowing
	size_t mMaxBinaryEvents;

	/// The number of analog events the slave will buffer before overflowing
	size_t mMaxAnalogEvents;

	/// The number of counter events the slave will buffer before overflowing
	size_t mMaxCounterEvents;

	/// The number of vto events the slave will buffer before overflowing
	size_t mMaxVtoEvents;
};

/** Configuration information for a dnp3 slave (outstation)

Used as both input describing the startup configuration of the slave, and as configuration state of mutable properties (i.e. unsolicited responses).

Major feature areas are unsolicited responses, time synchronization requests, event buffer limits, and the DNP3 object/variations to use by default
when the master requests class data or variation 0.

*/
struct SlaveConfig {
	SlaveConfig();


	/// The maximum number of controls the slave will attempt to process from a single APDU
	size_t mMaxControls;

	/// if true, fully disables unsolicited mode as if the slave didn't support it
	bool mDisableUnsol;

	/// controls what unsol classes are enabled
	ClassMask mUnsolMask;

	/// if true, the slave will request time synchronization on an interval
	bool mAllowTimeSync;


	/// The period of time sync interval in milliseconds
	millis_t mTimeSyncPeriod;

	/// The amount of time the slave will wait before sending new unsolicited data ( <= 0 == immediate)
	millis_t mUnsolPackDelay;

	/// How long the slave will wait before retrying an unsuccessful unsol response
	millis_t mUnsolRetryDelay;

	/// How long the slave will allow an operate to proceed after a prior select
	millis_t mSelectTimeout;

	/// The maximum fragment size the slave will use for data it sends
	size_t mMaxFragSize;

	/// The number of objects to store in the VtoWriter queue.
	size_t mVtoWriterQueueSize;

	/// Structure that defines the maximum number of events to buffer
	EventMaxConfig mEventMaxConfig;

	// default static response types

	/// The default group/variation to use for static binary responses
	GrpVar mStaticBinary;

	/// The default group/variation to use for static analog responses
	GrpVar mStaticAnalog;

	/// The default group/variation to use for static counter responses
	GrpVar mStaticCounter;

	/// The default group/variation to use for static setpoint status responses
	GrpVar mStaticSetpointStatus;


	/// default event response types

	/// The default group/variation to use for binary event responses
	GrpVar mEventBinary;

	/// The default group/variation to use for analog event responses
	GrpVar mEventAnalog;

	/// The default group/variation to use for counter event responses
	GrpVar mEventCounter;

	/// The default group/variation to use for VTO event responses
	GrpVar mEventVto;

};

}

/* vim: set ts=4 sw=4: */

#endif

