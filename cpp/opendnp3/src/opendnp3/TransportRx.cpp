
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
// you under the terms of the License.
//

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

void TransportRx::HandleReceive(const uint8_t* apData, size_t aNumBytes)
{
	if(aNumBytes < 2)
	{
		ERROR_BLOCK(LEV_WARNING, "Received tpdu with no payload, size: " << aNumBytes, TLERR_NO_PAYLOAD);
	}
	else if(aNumBytes > TL_MAX_TPDU_LENGTH) 
	{
		ERROR_BLOCK(LEV_WARNING, "Illegal arg: " << aNumBytes << " exceeds max tpdu size of " << TL_MAX_TPDU_LENGTH, TLERR_TOO_MUCH_DATA);

	} else {

		uint8_t hdr = apData[0];
		LOG_BLOCK(LEV_INTERPRET, "<- " << TransportLayer::ToString(hdr));
		bool first = (hdr & TL_HDR_FIR) != 0;
		bool last = (hdr & TL_HDR_FIN) != 0;
		int seq = hdr & TL_HDR_SEQ;
		size_t payload_len = aNumBytes - 1;

		if(this->ValidateHeader(first, last, seq, payload_len)) {
			if(BufferRemaining() < payload_len) {
				ERROR_BLOCK(LEV_WARNING, "Exceeded the buffer size before a complete fragment was read", TLERR_BUFFER_FULL);
				mNumBytesRead = 0;
			}
			else { //passed all validation
				memcpy(mBuffer + mNumBytesRead, apData + 1, payload_len);
				mNumBytesRead += payload_len;
				mSeq = (mSeq + 1) % 64;

				if(last) {
					size_t tmp = mNumBytesRead;
					mNumBytesRead = 0;
					mpContext->ReceiveAPDU(mBuffer, tmp);
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
			/*  2004-03-29_DNP3_Doc_Library.pdf: 2-2 Page 64.
				When a secondary station receives a frame with the FIR bit set,
				all previously received unterminated frame sequences are discarded. */
			ERROR_BLOCK(LEV_WARNING, "FIR received mid-fragment, discarding: " << mNumBytesRead << "bytes", TLERR_NEW_FIR);
			mNumBytesRead = 0;
		}
	}
	else if(mNumBytesRead == 0) { //non-first packet with 0 prior bytes
		ERROR_BLOCK(LEV_WARNING, "non-FIR packet with 0 prior bytes", TLERR_MESSAGE_WITHOUT_FIR);
		return false;
	}

	if(!aFin && aPayloadSize != TL_MAX_TPDU_PAYLOAD) {
		//if it's not a FIN packet it should have a length of
		ERROR_BLOCK(LEV_WARNING, "Partial non-FIN frame, payload= " << aPayloadSize, TLERR_BAD_LENGTH);
		return false;
	}

	if(aSeq != mSeq) {
		ERROR_BLOCK(LEV_WARNING, "Ignoring bad sequence, got: " << aSeq << " expected: " << mSeq, TLERR_BAD_SEQUENCE);
		return false;
	}



	return true;
}

}

