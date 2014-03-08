/**
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */
#include "TransportTx.h"



#include "TransportLayer.h"

#include <openpal/LoggableMacros.h>

#include <assert.h>
#include <sstream>
#include <memory.h>

using namespace std;
using namespace openpal;

namespace opendnp3
{

TransportTx::TransportTx(Logger& arLogger, TransportLayer* apContext, uint32_t fragSize) :
	Loggable(arLogger),
	mpContext(apContext),
	underlying(),
	apduBuffer(underlying.GetWriteBuffer().Truncate(fragSize)),
	tpduBuffer(),
	numBytesSent(0),
	numBytesToSend(0),
	sequence(0)
{}

void TransportTx::Send(const ReadOnlyBuffer& output)
{
	assert(output.IsNotEmpty());
	assert(output.Size() <= apduBuffer.Size());
	output.CopyTo(apduBuffer);	
	numBytesToSend = output.Size();
	numBytesSent = 0;

	this->CheckForSend();
}

bool TransportTx::CheckForSend()
{
	size_t remainder = this->BytesRemaining();

	if(remainder > 0) 
	{
		uint32_t numToSend = remainder < TL_MAX_TPDU_PAYLOAD ? remainder : TL_MAX_TPDU_PAYLOAD;
		memcpy(tpduBuffer.Buffer() + 1, apduBuffer + numBytesSent, numToSend);

		bool fir = (numBytesSent == 0);
		numBytesSent += numToSend;
		bool fin = (numBytesSent == numBytesToSend);

		tpduBuffer[0] = GetHeader(fir, fin, sequence);
		LOG_BLOCK(LogLevel::Interpret, "-> " << TransportLayer::ToString(tpduBuffer[0]));
		ReadOnlyBuffer buffer(tpduBuffer.Buffer(), numToSend + 1);
		mpContext->TransmitTPDU(buffer);
		return false;
	}
	else {
		numBytesSent = numBytesToSend = 0;
		return true;
	}
}

bool TransportTx::SendSuccess()
{
	sequence = (sequence + 1) % 64;

	return this->CheckForSend();
}

uint8_t TransportTx::GetHeader(bool fir, bool fin, uint8_t sequence)
{
	uint8_t hdr = 0;
	if(fir) hdr |= TL_HDR_FIR;
	if(fin) hdr |= TL_HDR_FIN;

	// Only the lower 6 bits of the sequence number
	hdr |= TL_HDR_SEQ & sequence;

	return hdr;
}

}

