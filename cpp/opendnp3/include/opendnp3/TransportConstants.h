#ifndef __TRANSPORT_CONSTANTS_H_
#define __TRANSPORT_CONSTANTS_H_


#include "DNPConstants.h"

namespace opendnp3
{

/// Transport header bitmasks
enum TransportHeader {
	TL_HDR_FIN = 0x80,
	TL_HDR_FIR = 0x40,
	TL_HDR_SEQ = 0x3F
};

/// Maximum TPDU length
const size_t TL_MAX_TPDU_LENGTH = 250;
/// Maximum TPDU payload size
const size_t TL_MAX_TPDU_PAYLOAD = 249;

}

#endif

