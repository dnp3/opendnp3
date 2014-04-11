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
	maxControls(1),
	disableUnsol(false),
	unsolMask(true, true, true),
	allowTimeSync(false),
	timeSyncPeriod(TimeDuration::Minutes(10)),
	unsolPackDelay(TimeDuration::Milliseconds(200)),
	unsolRetryDelay(TimeDuration::Seconds(2)),
	selectTimeout(TimeDuration::Seconds(5)),
	maxFragSize(sizes::DEFAULT_APDU_BUFFER_SIZE),
	staticBinary(StaticBinaryResponse::Group1Var2),
	staticAnalog(StaticAnalogResponse::Group30Var1),
	staticCounter(StaticCounterResponse::Group20Var1),
	staticFrozenCounter(StaticFrozenCounterResponse::Group21Var1),
	staticBinaryOutputStatus(StaticBinaryOutputStatusResponse::Group10Var2),
	staticAnalogOutputStatus(StaticAnalogOutputStatusResponse::Group40Var1),
	eventBinary(EventBinaryResponse::Group2Var1),
	eventAnalog(EventAnalogResponse::Group32Var1),
	eventCounter(EventCounterResponse::Group22Var1)
{}

}


