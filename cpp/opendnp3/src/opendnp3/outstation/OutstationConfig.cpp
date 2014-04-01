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
#include "OutstationConfig.h"

#include "opendnp3/StaticSizeConfiguration.h"

using namespace openpal;

namespace opendnp3
{

	OutstationConfig::OutstationConfig() :
	mMaxControls(1),
	mDisableUnsol(false),
	mUnsolMask(true, true, true),
	mAllowTimeSync(false),
	mTimeSyncPeriod(TimeDuration::Minutes(10)),
	mUnsolPackDelay(TimeDuration::Milliseconds(200)),
	mUnsolRetryDelay(TimeDuration::Seconds(2)),
	mSelectTimeout(TimeDuration::Seconds(5)),
	mMaxFragSize(sizes::DEFAULT_APDU_BUFFER_SIZE),	
	mStaticBinary(StaticBinaryResponse::Group1Var2),
	mStaticAnalog(StaticAnalogResponse::Group30Var1),
	mStaticCounter(StaticCounterResponse::Group20Var1),
	mStaticFrozenCounter(StaticFrozenCounterResponse::Group21Var1),
	mStaticBinaryOutputStatus(StaticBinaryOutputStatusResponse::Group10Var2),
	mStaticAnalogOutputStatus(StaticAnalogOutputStatusResponse::Group40Var1),
	mEventBinary(EventBinaryResponse::Group2Var1),
	mEventAnalog(EventAnalogResponse::Group32Var1),
	mEventCounter(EventCounterResponse::Group22Var1)
{}

}


