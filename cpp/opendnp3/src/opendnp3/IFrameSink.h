#ifndef __I_FRAME_SINK_H_
#define __I_FRAME_SINK_H_

#include <openpal/Visibility.h>
#include <openpal/BufferWrapper.h>

#include <stddef.h>
#include <cstdint>

namespace opendnp3
{

// @section DESCRIPTION Interface that receives parsed FT3 frame information
class DLL_LOCAL IFrameSink
{
public:

	virtual ~IFrameSink() {}
	//	Sec to Pri

	virtual void Ack(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc) = 0;
	virtual void Nack(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc) = 0;
	virtual void LinkStatus(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc) = 0;
	virtual void NotSupported (bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc) = 0;


	//	Pri to Sec

	virtual void TestLinkStatus(bool aIsMaster, bool aFcb, uint16_t aDest, uint16_t aSrc) = 0;
	virtual void ResetLinkStates(bool aIsMaster, uint16_t aDest, uint16_t aSrc) = 0;
	virtual void RequestLinkStatus(bool aIsMaster, uint16_t aDest, uint16_t aSrc) = 0;
	virtual void ConfirmedUserData(bool aIsMaster, bool aFcb, uint16_t aDest, uint16_t aSrc, const openpal::ReadOnlyBuffer& arBuffer) = 0;
	virtual void UnconfirmedUserData(bool aIsMaster, uint16_t aDest, uint16_t aSrc, const openpal::ReadOnlyBuffer& arBuffer) = 0;
};

}

#endif

