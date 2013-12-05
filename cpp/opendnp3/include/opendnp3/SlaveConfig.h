#ifndef __SLAVE_CONFIG_H_
#define __SLAVE_CONFIG_H_

#include <assert.h>

#include <openpal/Exception.h>
#include <openpal/TimeDuration.h>

#include "ClassMask.h"
#include "ObjectInterfaces.h"

#include <opendnp3/gen/EventAnalogResponse.h>
#include <opendnp3/gen/EventBinaryResponse.h>
#include <opendnp3/gen/EventCounterResponse.h>

#include <opendnp3/gen/StaticAnalogResponse.h>
#include <opendnp3/gen/StaticBinaryResponse.h>
#include <opendnp3/gen/StaticCounterResponse.h>
#include <opendnp3/gen/StaticSetpointStatusResponse.h>

namespace opendnp3
{

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
	openpal::TimeDuration mTimeSyncPeriod;

	/// The amount of time the slave will wait before sending new unsolicited data ( <= 0 == immediate)
	openpal::TimeDuration mUnsolPackDelay;

	/// How long the slave will wait before retrying an unsuccessful unsol response
	openpal::TimeDuration mUnsolRetryDelay;

	/// How long the slave will allow an operate to proceed after a prior select
	openpal::TimeDuration mSelectTimeout;

	/// The maximum fragment size the slave will use for data it sends
	size_t mMaxFragSize;

	/// Structure that defines the maximum number of events to buffer
	EventMaxConfig mEventMaxConfig;

	// default static response types

	/// The default group/variation to use for static binary responses
	StaticBinaryResponse mStaticBinary;

	/// The default group/variation to use for static analog responses
	StaticAnalogResponse mStaticAnalog;

	/// The default group/variation to use for static counter responses
	StaticCounterResponse mStaticCounter;

	/// The default group/variation to use for static setpoint status responses
	StaticSetpointStatusResponse mStaticSetpointStatus;


	/// default event response types

	/// The default group/variation to use for binary event responses
	EventBinaryResponse mEventBinary;

	/// The default group/variation to use for analog event responses
	EventAnalogResponse mEventAnalog;

	/// The default group/variation to use for counter event responses
	EventCounterResponse mEventCounter;


};

}

/* vim: set ts=4 sw=4: */

#endif

