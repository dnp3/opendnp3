//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one
// or more contributor license agreements. See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  Green Enery Corp licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
//
#include "CommandTypes.h"
#include "PackingUnpacking.h"
#include "Exception.h"

#include <assert.h>
#include <sstream>

namespace apl
{

#define MACRO_CASE_DECLARE(type) case(type): return type;

CommandStatus ByteToCommandStatus(uint8_t aField)
{
	switch(aField) {
		MACRO_CASE_DECLARE(CS_SUCCESS)
		MACRO_CASE_DECLARE(CS_TIMEOUT)
		MACRO_CASE_DECLARE(CS_NO_SELECT)
		MACRO_CASE_DECLARE(CS_FORMAT_ERROR)
		MACRO_CASE_DECLARE(CS_NOT_SUPPORTED)
		MACRO_CASE_DECLARE(CS_ALREADY_ACTIVE)
		MACRO_CASE_DECLARE(CS_HARDWARE_ERROR)
		MACRO_CASE_DECLARE(CS_LOCAL)
		MACRO_CASE_DECLARE(CS_TOO_MANY_OPS)
		MACRO_CASE_DECLARE(CS_NOT_AUTHORIZED)

	default:
		return CS_UNDEFINED;
	}

}

ControlCode ByteToControlCode(uint8_t aField)
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

std::string ToString(CommandStatus aType)
{
	switch(aType) {
		TO_STRING_CASE(CS_SUCCESS)
		TO_STRING_CASE(CS_TIMEOUT)
		TO_STRING_CASE(CS_NO_SELECT)
		TO_STRING_CASE(CS_FORMAT_ERROR)
		TO_STRING_CASE(CS_NOT_SUPPORTED)
		TO_STRING_CASE(CS_ALREADY_ACTIVE)
		TO_STRING_CASE(CS_HARDWARE_ERROR)
		TO_STRING_CASE(CS_LOCAL)
		TO_STRING_CASE(CS_TOO_MANY_OPS)
		TO_STRING_CASE(CS_NOT_AUTHORIZED)
	default:
		return "Unknown";
	}
}
std::string ToString(ControlCode aType)
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

std::string ToString(CommandTypes aType)
{
	switch(aType) {
	case(CT_BINARY_OUTPUT):
		return "BinaryOutput";
	case(CT_SETPOINT):
		return "Setpoint";
	default:
		return "Unknown";
	}
}

CommandRequest::CommandRequest(CommandTypes aType) :
	mStatus(CS_SUCCESS),
	mType(aType)
{}

CommandRequest::CommandRequest(const CommandRequest& arCopy)
{
	mType = arCopy.mType;
	mStatus = arCopy.mStatus;
}

BinaryOutput::BinaryOutput() :
	CommandRequest(CT_BINARY_OUTPUT),
	mRawCode(CC_NULL),
	mCount(1),
	mOnTimeMS(100),
	mOffTimeMS(100)
{

}

BinaryOutput::BinaryOutput(ControlCode aCode, uint8_t aCount, uint32_t aOnTime, uint32_t aOffTime) :
	CommandRequest(CT_BINARY_OUTPUT),
	mRawCode(aCode),
	mCount(aCount),
	mOnTimeMS(aOnTime),
	mOffTimeMS(aOffTime)
{

}

ControlCode BinaryOutput::GetCode() const
{
	return ByteToControlCode(mRawCode);
}

std::string BinaryOutput::ToString() const
{
	std::ostringstream oss;
	oss << "BinaryOutput - function: " << apl::ToString(static_cast<ControlCode>(mRawCode));
	oss << " count: " << static_cast<size_t>(mCount) << " on: " << mOnTimeMS << " off: " << mOffTimeMS;
	return oss.str();
}

Setpoint::Setpoint() : CommandRequest(CT_SETPOINT), mEncodingType(SPET_UNSET) {}

Setpoint::Setpoint(int16_t aValue) :
	CommandRequest(CT_SETPOINT),
	mValue(aValue),
	mEncodingType(SPET_AUTO_INT)
{}

Setpoint::Setpoint(int32_t aValue) :
	CommandRequest(CT_SETPOINT),
	mValue(aValue),
	mEncodingType(SPET_AUTO_INT)
{}

Setpoint::Setpoint(double aValue) :
	CommandRequest(CT_SETPOINT),
	mValue(aValue),
	mEncodingType(SPET_AUTO_DOUBLE)
{}

std::string Setpoint::ToString() const
{
	std::ostringstream oss;
	oss << "Setpoint - value: " << mValue;
	return oss.str();
}

void Setpoint::SetValue(double aValue)
{
	mValue = aValue;
	if(mEncodingType == SPET_UNSET) {
		mEncodingType = SPET_AUTO_DOUBLE;
	}
}
void Setpoint::SetValue(int32_t aValue)
{
	mValue = static_cast<double>(aValue);
	if(mEncodingType == SPET_UNSET) {
		mEncodingType = SPET_AUTO_INT;
	}
}

double Setpoint::GetValue() const
{
	assert(mEncodingType != SPET_UNSET);
	return mValue;
}

SetpointEncodingType Setpoint::GetOptimalEncodingType() const
{
	assert(mEncodingType != SPET_UNSET);
	if(mEncodingType == SPET_AUTO_INT) {
		if(mValue <= Int16LE::Max && mValue >= Int16LE::Min) return SPET_INT16;
		return SPET_INT32;
	}
	if(mEncodingType == SPET_AUTO_DOUBLE) {
		if(mValue <= SingleFloat::Max && mValue >= SingleFloat::Min) return SPET_FLOAT;
		return SPET_DOUBLE;
	}
	// if its not one of the auto types that means its been explictly set
	// so we should use that type.
	return mEncodingType;
}


}
