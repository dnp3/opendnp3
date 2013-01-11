
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
// you under the GNU Affero General Public License Version 3.0 
// (the "Additional License"). You may not use these modifications except in 
// compliance with the additional license. You may obtain a copy of the Additional 
// License at
//
// http://www.gnu.org/licenses/agpl.html
//
// Contact Automatak, LLC for a commercial license to these modifications
//
#include "TransportTx.h"

#include "TransportLayer.h"

#include <APL/Exception.h>
#include <APL/Logger.h>
#include <APL/LoggableMacros.h>

#include <assert.h>
#include <sstream>
#include <memory.h>

using namespace std;

namespace apl
{
namespace dnp
{

TransportTx::TransportTx(Logger* apLogger, TransportLayer* apContext, size_t aFragSize) :
	Loggable(apLogger),
	mpContext(apContext),
	mBufferAPDU(aFragSize),
	mBufferTPDU(TL_MAX_TPDU_LENGTH),
	mNumBytesSent(0),
	mNumBytesToSend(0),
	mSeq(0)
{}

void TransportTx::Send(const uint8_t* apData, size_t aNumBytes)
{
	assert(aNumBytes > 0);
	assert(aNumBytes <= mBufferAPDU.Size());

	memcpy(mBufferAPDU, apData, aNumBytes);
	mNumBytesToSend = aNumBytes;
	mNumBytesSent = 0;

	this->CheckForSend();
}

bool TransportTx::CheckForSend()
{
	size_t remainder = this->BytesRemaining();

	if(remainder > 0) {
		size_t num_to_send = remainder < TL_MAX_TPDU_PAYLOAD ? remainder : TL_MAX_TPDU_PAYLOAD;
		memcpy(mBufferTPDU + 1, mBufferAPDU + mNumBytesSent, num_to_send);

		bool fir = (mNumBytesSent == 0);
		mNumBytesSent += num_to_send;
		bool fin = (mNumBytesSent == mNumBytesToSend);

		mBufferTPDU[0] = GetHeader(fir, fin, mSeq);
		LOG_BLOCK(LEV_INTERPRET, "-> " << TransportLayer::ToString(mBufferTPDU[0]));
		mpContext->TransmitTPDU(mBufferTPDU, num_to_send + 1);
		return false;
	}
	else {
		mNumBytesSent = mNumBytesToSend = 0;
		return true;
	}
}

bool TransportTx::SendSuccess()
{
	mSeq = (mSeq + 1) % 64;

	return this->CheckForSend();
}

uint8_t TransportTx::GetHeader(bool aFir, bool aFin, int aSeq)
{
	uint8_t hdr = 0;
	if(aFir) hdr |= TL_HDR_FIR;
	if(aFin) hdr |= TL_HDR_FIN;

	// Only the lower 6 bits of the sequence number
	hdr |= TL_HDR_SEQ & aSeq;

	return hdr;
}

}
}

