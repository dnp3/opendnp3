#ifndef __CONTROL_RELAY_OUTPUT_BLOCK_H_
#define __CONTROL_RELAY_OUTPUT_BLOCK_H_

#include "gen/CommandStatus.h"
#include "gen/ControlCode.h"

namespace opendnp3
{

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

	ControlRelayOutputBlock(ControlCode aCode = ControlCode::LATCH_ON, uint8_t aCount = 1, uint32_t aOnTime = 100, uint32_t aOffTime = 100);

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
