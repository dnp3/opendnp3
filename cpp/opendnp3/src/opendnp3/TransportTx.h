#ifndef __TRANSPORT_TX_H_
#define __TRANSPORT_TX_H_

#include <openpal/Visibility.h>
#include <openpal/Loggable.h>
#include <openpal/BufferWrapper.h>

#include <opendnp3/TransportConstants.h>
#include "CopyableBuffer.h"

namespace opendnp3
{

class TransportLayer;

/**
State/validation for the DNP3 transport layer's send channel.
*/
class DLL_LOCAL TransportTx : public openpal::Loggable
{
public:
	TransportTx(openpal::Logger&, TransportLayer*, size_t aFragSize);


	void Send(const openpal::ReadOnlyBuffer &arBuffer); // A fresh call to Send() will reset the state
	bool SendSuccess();


	static uint8_t GetHeader(bool aFir, bool aFin, int aSeq);

private:

	bool CheckForSend();

	TransportLayer* mpContext;

	CopyableBuffer mBufferAPDU;
	CopyableBuffer mBufferTPDU;

	size_t mNumBytesSent;
	size_t mNumBytesToSend;
	int mSeq;

	size_t BytesRemaining() {
		return mNumBytesToSend - mNumBytesSent;
	}
};

}

#endif

