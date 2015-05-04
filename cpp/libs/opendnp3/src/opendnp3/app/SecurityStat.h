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
#ifndef OPENDNP3_SECURITYSTATISTIC_H
#define OPENDNP3_SECURITYSTATISTIC_H


#include "opendnp3/Types.h"
#include "opendnp3/app/EventType.h"
#include "opendnp3/gen/StaticSecurityStatVariation.h"
#include "opendnp3/gen/EventSecurityStatVariation.h"

#include "opendnp3/gen/StaticTypeBitmask.h"

namespace opendnp3
{

/**
	SA security statistic object as used by the API.
*/
class SecurityStat
{
public:

	SecurityStat();

	SecurityStat(uint8_t quality, uint16_t assocId, uint32_t count);

	SecurityStat(uint8_t quality, uint16_t assocId, uint32_t count, DNPTime time);

	uint8_t quality;	//	bitfield that stores type specific quality flags
	uint16_t assocId;	//	the association that the stat came from
	uint32_t count;		//	the actualy value of the statistic
	DNPTime time;		//	timestamp associated with the measurement (may not be set)

	const static EventType EventTypeEnum = EventType::SecurityStat;
	const static StaticTypeBitmask StaticTypeEnum = StaticTypeBitmask::SecurityStat;
	const static EventSecurityStatVariation DefaultEventVariation = EventSecurityStatVariation::Group122Var1;
	const static StaticSecurityStatVariation DefaultStaticVariation = StaticSecurityStatVariation::Group121Var1;
		
	typedef uint32_t ValueType;
	typedef EventSecurityStatVariation EventVariation;
	typedef StaticSecurityStatVariation StaticVariation;	
};

}

#endif
