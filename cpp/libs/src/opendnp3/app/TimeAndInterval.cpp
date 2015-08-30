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

#include "opendnp3/app/TimeAndInterval.h"

namespace opendnp3
{

TimeAndInterval::TimeAndInterval() : time(0), interval(0), units(0)
{}

TimeAndInterval::TimeAndInterval(DNPTime time_, uint32_t interval_, uint8_t units_) :
	time(time_),
	interval(interval_),
	units(units_)
{}

TimeAndInterval::TimeAndInterval(DNPTime time_, uint32_t interval_, IntervalUnits units_) :
	time(time_),
	interval(interval_),
	units(static_cast<uint8_t>(units_))
{}

IntervalUnits TimeAndInterval::GetUnitsEnum() const
{
	return IntervalUnitsFromType(units);
}

}
