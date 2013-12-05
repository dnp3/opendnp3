#include "TransportRx.h"


#include <opendnp3/TransportConstants.h>
#include <openpal/Exception.h>

#include <openpal/LoggableMacros.h>

#include "TransportLayer.h"

#include <sstream>
#include <memory.h>

using namespace std;
using namespace openpal;

namespace opendnp3
{

TransportRx::TransportRx(Logger& arLogger, TransportLayer* apContext, size_t aFragSize) :
	Loggable(arLogger),
	mpContext(apContext),
	mBuffer(aFragSize),
	mNumBytesRead(0),
	mSeq(0)
{

}

void TransportRx::Reset()
{
	mNumBytesRead = 0;
	mSeq = 0;
}

void TransportRx::HandleReceive(const openpal::ReadOnlyBuffer& arBuffer)
{
	if(arBuffer.Size() < 2)
	{
		ERROR_BLOCK(LogLevel::Warning, "Received tpdu with no payload, size: " << arBuffer.Size(), TLERR_NO_PAYLOAD);
	}
	else if(arBuffer.Size() > TL_MAX_TPDU_LENGTH) 
	{
		ERROR_BLOCK(LogLevel::Warning, "Illegal arg: " << arBuffer.Size() << " exceeds max tpdu size of " << TL_MAX_TPDU_LENGTH, TLERR_TOO_MUCH_DATA);

	} else {

		uint8_t hdr = arBuffer[0];
		LOG_BLOCK(LogLevel::Interpret, "<- " << TransportLayer::ToString(hdr));
		bool first = (hdr & TL_HDR_FIR) != 0;
		bool last = (hdr & TL_HDR_FIN) != 0;
		int seq = hdr & TL_HDR_SEQ;
		size_t payload_len = arBuffer.Size() - 1;

		if(this->ValidateHeader(first, last, seq, payload_len)) {
			if(BufferRemaining() < payload_len) {
				ERROR_BLOCK(LogLevel::Warning, "Exceeded the buffer size before a complete fragment was read", TLERR_BUFFER_FULL);
				mNumBytesRead = 0;
			}
			else { //passed all validation
				memcpy(mBuffer + mNumBytesRead, arBuffer + 1, payload_len);
				mNumBytesRead += payload_len;
				mSeq = (mSeq + 1) % 64;

				if(last) {
					ReadOnlyBuffer buffer(mBuffer, mNumBytesRead);
					mNumBytesRead = 0;					
					mpContext->ReceiveAPDU(buffer);
				}
			}
		}
	}
	
	
}

bool TransportRx::ValidateHeader(bool aFir, bool aFin, int aSeq, size_t aPayloadSize)
{
	//get the transport byte and parse it

	if(aFir) {
		mSeq = aSeq; //always accept the sequence on FIR
		if(mNumBytesRead > 0) {
			// drop existing received bytes from segment
			ERROR_BLOCK(LogLevel::Warning, "FIR received mid-fragment, discarding: " << mNumBytesRead << "bytes", TLERR_NEW_FIR);
			mNumBytesRead = 0;
		}
	}
	else if(mNumBytesRead == 0) { //non-first packet with 0 prior bytes
		ERROR_BLOCK(LogLevel::Warning, "non-FIR packet with 0 prior bytes", TLERR_MESSAGE_WITHOUT_FIR);
		return false;
	}
	
	if(aSeq != mSeq) {
		ERROR_BLOCK(LogLevel::Warning, "Ignoring bad sequence, got: " << aSeq << " expected: " << mSeq, TLERR_BAD_SEQUENCE);
		return false;
	}

	return true;
}

}

