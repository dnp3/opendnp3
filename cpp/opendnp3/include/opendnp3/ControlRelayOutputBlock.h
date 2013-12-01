
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
#ifndef __CONTROL_RELAY_OUTPUT_BLOCK_H_
#define __CONTROL_RELAY_OUTPUT_BLOCK_H_

#include "gen/CommandStatus.h"

namespace opendnp3
{

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

/**
* Converts an integer to a control code
*
* @param aField The integer to convert
* @return ControlCode enumeration corresponding to the integer
*/
ControlCode IntToControlCode(int aField);

/**
* Converts a control code to string representation
*
* @param aCode The control code to convert
* @return string representation of the code
*/
std::string ControlCodeToString(ControlCode aType);


/**
 * Describes an incoming control request from the master. It is the
 * applications responsibility to handle the request and return an
 * approiate status code.The PULSE_CLOSE and PULSE_TRIP ControlCodes
 * require setting the mOnTimeMS, mOffTimeMS and mCount variables, otherwise
 * just use the defaults.
 */
class ControlRelayOutputBlock
{
public:

	ControlRelayOutputBlock(ControlCode aCode = CC_LATCH_ON, uint8_t aCount = 1, uint32_t aOnTime = 100, uint32_t aOffTime = 100);

	/// @return the ControlCode enumeration corresponding to the raw code
	ControlCode GetCode() const;

	/// allows matching of exact code
	uint8_t mRawCode;
	/// the number of times to repeat the operation
	uint8_t mCount;
	/// the 'on' time for the pulse train
	uint32_t mOnTimeMS;
	/// the 'off' time for the pulse train
	uint32_t mOffTimeMS;
	/// status of the resulting operation
	CommandStatus mStatus;

	/// @return string representation of the CROB
	std::string ToString() const;

	/**
	* @param arRHS the CROB to compare to this object
	* @return true if all fields of arRHS match
	*/
	bool operator==(const ControlRelayOutputBlock& arRHS) const {
		return (mRawCode == arRHS.mRawCode) && (mCount == arRHS.mCount) && (mOnTimeMS == arRHS.mOnTimeMS) && (mOffTimeMS == arRHS.mOffTimeMS);
	}
};


}

/* vim: set ts=4 sw=4: */

#endif
