#include "ProtocolUtil.h"

namespace opendnp3
{

// Given a buffer and max packet size, calculcate the
// maximum number of packets the buffer can hold
size_t CalcMaxPackets(size_t aBuffer, size_t aPayload)
{
	size_t remain = aBuffer % aPayload;
	size_t num = aBuffer / aPayload;
	return (remain == 0) ? num : num + 1;
}

size_t CalcLastPacketSize(size_t aBuffer, size_t aPayload)
{
	size_t remain = aBuffer % aPayload;
	return (remain == 0) ? aPayload : remain;
}

} //end namespace
