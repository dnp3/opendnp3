#ifndef __LINK_HEADER_H_
#define __LINK_HEADER_H_

#include <openpal/Visibility.h>

#include "LinkLayerConstants.h"
#include "gen/LinkFunction.h"

namespace opendnp3
{

// Class for dealing with all aspects of FT3 Frame headers
struct DLL_LOCAL LinkHeader {
	LinkHeader();

	// Setter

	void Set(uint8_t aLen, uint16_t aSrc, uint16_t aDest, bool aFromMaster, bool aFcvDfc, bool aFcb, LinkFunction aCode);

	void ChangeFCB(bool aFCB);

	uint8_t GetControl() const {
		return ctrl;
	}

	// Getters
	uint8_t GetLength() const {
		return length;
	}
	uint16_t GetDest() const {
		return dest;
	}
	uint16_t GetSrc() const {
		return src;
	}
	bool IsFromMaster() const {
		return (ctrl & MASK_DIR) != 0;
	}
	bool IsPriToSec() const {
		return (ctrl & MASK_PRM) != 0;
	}
	bool IsFcbSet() const {
		return (ctrl & MASK_FCB) != 0;
	}
	bool IsFcvDfcSet() const {
		return (ctrl & MASK_FCV) != 0;
	}
	uint8_t GetFuncByte() const {
		return ctrl & MASK_FUNC;
	}
	LinkFunction GetFuncEnum() const {
		return LinkFunctionFromType(ctrl & MASK_FUNC_OR_PRM);
	}

	bool ValidLength() {
		return length > 4;
	}

	/** Reads the header, setting all the fields. Does NOT validate 0x0564 or CRC
	@param apBuff Buffer of at least 10 bytes */
	void Read(const uint8_t* apBuff);

	/** Writes header to buffer including, 0x0564 and CRC
	@param apBuff Buffer of at least 10 bytes */
	void Write(uint8_t* apBuff) const;

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
	std::string ToString() const;
#endif

	static uint8_t ControlByte(bool aIsMaster, bool aFcb, bool aFcvDfc, LinkFunction aFunc);

private:

	// Fields read directly from the header
	uint8_t length;	// Length of field, range [5,255] valid
	uint16_t src;	// Where the frame originated
	uint16_t dest;	// Where the frame is going
	uint8_t ctrl;	// Control octet, individual fields accessed using accessors below
};

}

#endif

