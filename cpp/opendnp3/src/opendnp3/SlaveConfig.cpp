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
