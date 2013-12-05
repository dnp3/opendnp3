#ifndef __PROTOCOL_UTIL_H_
#define __PROTOCOL_UTIL_H_

#include <stddef.h>

#include <openpal/Visibility.h>


namespace opendnp3
{

// Given a buffer and max packet size, calculcate the
// maximum number of packets the buffer can hold
size_t DLL_LOCAL CalcMaxPackets(size_t aBuffer, size_t aPayload);


// Given a buffer and max packet size, calculcate the
// size of the last packet.
size_t DLL_LOCAL CalcLastPacketSize(size_t aBuffer, size_t aPayload);


}

#endif

