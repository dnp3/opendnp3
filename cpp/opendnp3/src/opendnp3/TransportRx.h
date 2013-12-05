#ifndef __TRANSPORT_RX_H_
#define __TRANSPORT_RX_H_

#include <opendnp3/TransportConstants.h>

#include <openpal/Visibility.h>
#include <openpal/Loggable.h>
#include <openpal/BufferWrapper.h>

#include "CopyableBuffer.h"

namespace opendnp3
{

class TransportLayer;

/**
State/validation for the DNP3 transport layer's receive channel.
*/
class DLL_LOCAL TransportRx : public openpal::Loggable
{
public:
	TransportRx(openpal::Logger&, TransportLayer*, size_t aFragSize);

	void HandleReceive(const openpal::ReadOnlyBuffer& arBuffer);

	void Reset();

private:

	bool ValidateHeader(bool aFir, bool aFin, int aSeq, size_t aPayloadSize);

	TransportLayer* mpContext;

	CopyableBuffer mBuffer;
	size_t mNumBytesRead;
	int mSeq;



	size_t BufferRemaining() {
		return mBuffer.Size() - mNumBytesRead;
	}
};

}

#endif

