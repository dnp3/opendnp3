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

#include "secauth/StatThresholds.h"

namespace secauth
{
const uint32_t StatThresholds::DEFAULTS[AuthConstants::NUM_SECURITY_STATS] =
{
	3,
	5,
	5,
	3,
	3,
	100,
	100,
	100,
	100,
	10,
	2,
	10,
	100,
	10,
	5,
	1,
	1,
	3
};

StatThresholds::StatThresholds()
{
	for (auto i = 0; i < AuthConstants::NUM_SECURITY_STATS; ++i)
	{
		thresholds[i] = DEFAULTS[i];
	}
}

void StatThresholds::Set(opendnp3::SecurityStatIndex index, uint32_t threshold)
{
	thresholds[static_cast<int>(index)] = threshold;
}

uint32_t StatThresholds::GetDeadband(uint16_t index) const
{
	// deadband is always 1 less than the threshold
	return (index < AuthConstants::NUM_SECURITY_STATS && thresholds[index] > 0) ? (thresholds[index] - 1) : 0;
}


}


