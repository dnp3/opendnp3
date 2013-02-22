
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
#include <opendnp3/ControlRelayOutputBlock.h>

#include "PackingUnpacking.h"

#include <opendnp3/Exception.h>

#include <assert.h>
#include <sstream>

namespace opendnp3
{

#define MACRO_CASE_DECLARE(type) case(type): return type;

ControlCode IntToControlCode(int aField)
{
	switch(aField) {
		MACRO_CASE_DECLARE(CC_NULL)
		MACRO_CASE_DECLARE(CC_PULSE)
		MACRO_CASE_DECLARE(CC_LATCH_ON)
		MACRO_CASE_DECLARE(CC_LATCH_OFF)
		MACRO_CASE_DECLARE(CC_PULSE_CLOSE)
		MACRO_CASE_DECLARE(CC_PULSE_TRIP)
		MACRO_CASE_DECLARE(CC_UNDEFINED)

	default:
		return CC_UNDEFINED;
	}
}

#define TO_STRING_CASE(c) case (c): return #c;

std::string ControlCodeToString(ControlCode aType)
{
	switch(aType) {
		TO_STRING_CASE(CC_NULL)
		TO_STRING_CASE(CC_PULSE)
		TO_STRING_CASE(CC_LATCH_ON)
		TO_STRING_CASE(CC_LATCH_OFF)
		TO_STRING_CASE(CC_PULSE_CLOSE)
		TO_STRING_CASE(CC_PULSE_TRIP)
	default:
		return "Unknown";
	}
}

ControlRelayOutputBlock::ControlRelayOutputBlock(ControlCode aCode, uint8_t aCount, uint32_t aOnTime, uint32_t aOffTime) :
	mRawCode(aCode),
	mCount(aCount),
	mOnTimeMS(aOnTime),
	mOffTimeMS(aOffTime),
	mStatus(CS_SUCCESS)
{

}

ControlCode ControlRelayOutputBlock::GetCode() const
{
	return IntToControlCode(mRawCode);
}

std::string ControlRelayOutputBlock::ToString() const
{
	std::ostringstream oss;
	oss << "ControlRelayOutputBlock - function: " << ControlCodeToString(static_cast<ControlCode>(mRawCode));
	oss << " count: " << static_cast<size_t>(mCount) << " on: " << mOnTimeMS << " off: " << mOffTimeMS;
	return oss.str();
}


}

