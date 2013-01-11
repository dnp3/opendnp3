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
#ifndef __CONTROL_TYPES_H_
#define __CONTROL_TYPES_H_

#include "Types.h"
#include <string>
#include <math.h>

namespace apl
{

enum CommandModes {
	CM_SBO_ONLY,	   	   // Point must be selected before operation
	CM_DO_ONLY,			   // select not allowed
	CM_SBO_OR_DO		   // Point supports either
};

enum CommandTypes {
	CT_BINARY_OUTPUT,
	CT_SETPOINT,
	CT_NONE
};

/**
 * When a command is recieved from a master the application sends a code to
 * indicate if it was successfull or if not what class of error was
 * encountered.  Each code has a description that indicates its customary
 * meaning.
 */
enum CommandStatus {
	CS_SUCCESS = 0,			//!< command was successfully recieved and handled
	CS_TIMEOUT = 1,			//!< command timedout before completing
	CS_NO_SELECT = 2,		//!< command requires being selected before operate, configuration issue
	CS_FORMAT_ERROR = 3,	//!< bad control code or timing values
	CS_NOT_SUPPORTED = 4,	//!< command is not implemented
	CS_ALREADY_ACTIVE = 5,	//!< command is allready in progress or its allready in that mode
	CS_HARDWARE_ERROR = 6,	//!< something is stopping the command, often a local/remote interlock
	CS_LOCAL = 7,			//!< the function goverened by the control is in local only control
	CS_TOO_MANY_OPS = 8,	//!< the command has been done too often and has been throttled
	CS_NOT_AUTHORIZED = 9,	//!< the command was rejected because the device denied it or an RTU intercepted it
	CS_UNDEFINED = 127		//!< 10 to 126 are currently reserved
};

CommandStatus ByteToCommandStatus(uint8_t aField);
std::string ToString(CommandStatus aType);

/**
 * There are a number of types of controls. The best way to understand this
 * difference is to think about the hardware controls the communication
 * protocols are emulating. The most common to use are CC_PULSE,
 * CC_LATCH_ON and CC_LATCH_OFF.
 *
 * NOTE: Current implementation doesn't support queue/clear.
 */
enum ControlCode {
	CC_NULL = 0,			//!< illegal command code (used internally)
	CC_PULSE = 0x01,		//!< a 'push-button' interface, can only be pressed one way (reset button on pedometer)
	CC_LATCH_ON = 0x03,		//!< a 'light-switch' moved to the ON position
	CC_LATCH_OFF = 0x04,	//!< a 'light-switch' moved to the OFF position
	CC_PULSE_CLOSE = 0x41,	//!< a 'doorbell' that rings while the button is depressed
	CC_PULSE_TRIP = 0x81,	//!< a 'doorbell' that stops ringing (is normally on) while depreseed
	CC_UNDEFINED = 0xFF		//!< undefined command (used by DNP standard)
};

ControlCode ByteToControlCode(uint8_t aField);
std::string ToString(ControlCode aType);
std::string ToString(CommandTypes aType);

class CommandRequest
{
public:
	CommandTypes GetType() const  {
		return mType;
	}
	CommandStatus mStatus;

protected:
	//only invokable from super class
	CommandRequest(CommandTypes);
	CommandRequest(const CommandRequest&);

private:
	CommandRequest();
	CommandTypes mType;
};

/**
 * Describes an incoming control request from the master. It is the
 * applications responsibility to handle the request and return an
 * approiate status code.The PULSE_CLOSE and PULSE_TRIP ControlCodes
 * require setting the mOnTimeMS, mOffTimeMS and mCount variables, otherwise
 * just use the defaults.
 */
class BinaryOutput : public CommandRequest
{
public:

	BinaryOutput();

	BinaryOutput(ControlCode aCode, uint8_t aCount = 1, uint32_t aOnTime = 100, uint32_t aOffTime = 100);

	ControlCode GetCode() const;

	uint8_t mRawCode; // allows matching of exact code
	uint8_t mCount;
	uint32_t mOnTimeMS;
	uint32_t mOffTimeMS;

	std::string ToString() const;

	bool operator==(const BinaryOutput& arRHS) const {
		return (mRawCode == arRHS.mRawCode) && (mCount == arRHS.mCount) && (mOnTimeMS == arRHS.mOnTimeMS) && (mOffTimeMS == arRHS.mOffTimeMS);
	}

	static const CommandTypes EnumType = CT_BINARY_OUTPUT;
};

/**
 * This encoding type describes exactly what the range and valid values for
 * a setpoint are, all encodings are signed.
 */
enum SetpointEncodingType {
	SPET_INT16,			//!< 16bit floating point (dnp Object41var1)
	SPET_INT32,			//!< 32bit signed integer (dnp Object41var2)
	SPET_FLOAT,			//!< 32bit floating point (dnp Object41var3)
	SPET_DOUBLE,		//!< 64bit floating point (dnp Object41var4)
	SPET_AUTO_INT,		//!< automatically choose smallest valid int type
	SPET_AUTO_DOUBLE,	//!< automatically choose smallest valid double type
	SPET_UNSET,			//!< means no type has been guessed or set yet
};

/**
 * The object to represent a setpoint request from the master. Think of
 * this like turning a dial on the front of a machine to desired setting.
 * A setpoint is natively repersented as a double but can be used to send
 * both floating point and integer values. The key field is mEncodingType
 * which informs the protocol buffers how to treat and format the values.
 * There are smart defaults and behaviors to automatically determine the
 * correct type of encoding to use in most cases (by default uses smallest
 * type that can handle the number). This can be overridden with the
 * SetEncodingType() function.
 */
class Setpoint : public CommandRequest
{
public:

	/**
	 * Creates a new Setpoint instance based on a 16-bit integer
	 * value.  This constructor is necessary to stop the compiler from
	 * having to guess which upcast to use.
	 */
	Setpoint(int16_t aValue);

	/**
	 * Creates a new Setpoint instance based on a 32-bit integer
	 * value.  This constructor is necessary to stop the compiler from
	 * having to guess which upcast to use.
	 */
	Setpoint(int32_t aValue);

	/**
	 * Creates a new Setpoint instance based on a double value.  This
	 * constructor is necessary to stop the compiler from having to
	 * guess which upcast to use.
	 */
	Setpoint(double aValue);

	/**
	 * Creates a new Setpoint instance with no initial value.  Care
	 * must be used with objects created by this method, as an
	 * exception will be thrown if someone tries to access the
	 * encoding type or value if one hasn't been set yet.
	 */
	Setpoint();

	std::string ToString() const;

	bool operator==(const Setpoint& arRHS) const {
		return fabs(mValue - arRHS.mValue) < 1E-6;
	}

	static const CommandTypes EnumType = CT_SETPOINT;

	int32_t GetIntValue() const {
		return static_cast<int32_t>(GetValue());
	}
	double GetValue() const;

	void SetValue(double aValue);
	void SetValue(int32_t aValue);

	SetpointEncodingType GetOptimalEncodingType() const;

	SetpointEncodingType GetEncodingType() const {
		return mEncodingType;
	}
	void SetEncodingType(SetpointEncodingType aEncodingType) {
		mEncodingType = aEncodingType;
	}

private:
	double mValue;

	SetpointEncodingType mEncodingType;

};

class CommandResponse
{
public:
	CommandResponse() {}
	CommandResponse(CommandStatus aStatus) : mResult(aStatus) {}

	CommandStatus mResult;
};

}

/* vim: set ts=4 sw=4: */

#endif
