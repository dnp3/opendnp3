#include <opendnp3/SlaveConfig.h>

#include <opendnp3/DNPConstants.h>

using namespace openpal;

namespace opendnp3
{

EventMaxConfig::EventMaxConfig() :
	mMaxBinaryEvents(1000),
	mMaxAnalogEvents(1000),
	mMaxCounterEvents(1000),
	mMaxVtoEvents(100)
{}

EventMaxConfig::EventMaxConfig(size_t aMaxBinaryEvents, size_t aMaxAnalogEvents, size_t aMaxCounterEvents, size_t aMaxVtoEvents) :
	mMaxBinaryEvents(aMaxBinaryEvents),
	mMaxAnalogEvents(aMaxAnalogEvents),
	mMaxCounterEvents(aMaxCounterEvents),
	mMaxVtoEvents(aMaxVtoEvents)
{}

SlaveConfig::SlaveConfig() :
	mMaxControls(1),
	mDisableUnsol(false),
	mUnsolMask(true, true, true),
	mAllowTimeSync(false),
	mTimeSyncPeriod(TimeDuration::Minutes(10)),
	mUnsolPackDelay(TimeDuration::Milliseconds(200)),
	mUnsolRetryDelay(TimeDuration::Seconds(2)),
	mSelectTimeout(TimeDuration::Seconds(5)),
	mMaxFragSize(DEFAULT_FRAG_SIZE),	
	mEventMaxConfig(),
	mStaticBinary(StaticBinaryResponse::Group1Var2),
	mStaticAnalog(StaticAnalogResponse::Group30Var1),
	mStaticCounter(StaticCounterResponse::Group20Var1),
	mStaticSetpointStatus(StaticSetpointStatusResponse::Group40Var1),
	mEventBinary(EventBinaryResponse::Group2Var1),
	mEventAnalog(EventAnalogResponse::Group32Var1),
	mEventCounter(EventCounterResponse::Group22Var1)
{}

}

/* vim: set ts=4 sw=4: */
