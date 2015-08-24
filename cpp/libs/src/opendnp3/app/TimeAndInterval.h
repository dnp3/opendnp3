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
#ifndef OPENDNP3_TIMEANDINTERVAL_H
#define OPENDNP3_TIMEANDINTERVAL_H

#include "opendnp3/gen/IntervalUnits.h"
#include "opendnp3/gen/StaticTimeAndIntervalVariation.h"
#include "opendnp3/gen/StaticTypeBitmask.h"

#include "opendnp3/app/EventMetadata.h"

#include "opendnp3/Types.h"

namespace opendnp3
{

/**
	Maps to Group50Var4

	This class is a bit of an out lier as an indexed type and is really only used in the DNP3 PV profile
*/
class TimeAndInterval
{
public:

	TimeAndInterval();

	TimeAndInterval(DNPTime time, uint32_t interval, uint8_t units);

	TimeAndInterval(DNPTime time, uint32_t interval, IntervalUnits units);
	
	IntervalUnits GetUnitsEnum() const;
	
	DNPTime time;
	uint32_t interval;	
	uint8_t units;	

	const static StaticTypeBitmask StaticTypeEnum = StaticTypeBitmask::TimeAndInterval;
	const static StaticTimeAndIntervalVariation DefaultStaticVariation = StaticTimeAndIntervalVariation::Group50Var4;

	typedef StaticTimeAndIntervalVariation StaticVariation;
	typedef EmptyMetadata<TimeAndInterval> MetadataType;
};

}

#endif
