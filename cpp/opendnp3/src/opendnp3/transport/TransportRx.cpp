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
#include "TransportRx.h"
#include "TransportConstants.h"
#include "TransportLayer.h"
#include "opendnp3/LogLevels.h"

#include <openpal/LoggableMacros.h>
#include <openpal/ToHex.h>


using namespace std;
using namespace openpal;

namespace opendnp3
{

TransportRx::TransportRx(const Logger& logger, TransportLayer* apContext, uint32_t fragSize) :
	Loggable(logger),
	mpContext(apContext),
	maxFragSize(fragSize),
	numBytesRead(0),
	sequence(0)
{
	if(fragSize > rxBuffer.Size())
	{
		maxFragSize = rxBuffer.Size();
		//LOG_BLOCK(flags::ERR, "specified fragment size exceeds underling buffer size");
	}
}

void TransportRx::Reset()
{
	numBytesRead = 0;
	sequence = 0;
}

void TransportRx::HandleReceive(const openpal::ReadOnlyBuffer& input)
{
	if (input.Size() < 2)
	{
		//ERROR_BLOCK(flags::WARN, "Received tpdu with no payload, size: " << input.Size(), TLERR_NO_PAYLOAD);
	}
	else if (input.Size() > TL_MAX_TPDU_LENGTH)
	{
		//ERROR_BLOCK(flags::WARN, "Illegal arg: " << input.Size() << " exceeds max tpdu size of " << TL_MAX_TPDU_LENGTH, TLERR_TOO_MUCH_DATA);

	}
	else
	{

		uint8_t hdr = input[0];
		//LOG_BLOCK(flags::INTERPRET, "<- " << TransportLayer::ToString(hdr));
		bool first = (hdr & TL_HDR_FIR) != 0;
		bool last = (hdr & TL_HDR_FIN) != 0;
		int seq = hdr & TL_HDR_SEQ;
		uint32_t payloadLength = input.Size() - 1;

		if (this->ValidateHeader(first, last, seq, payloadLength))
		{
			if (BufferRemaining() < payloadLength)
			{
				//ERROR_BLOCK(flags::WARN, "Exceeded the buffer size before a complete fragment was read", TLERR_BUFFER_FULL);
				numBytesRead = 0;
			}
			else   //passed all validation
			{
				memcpy(rxBuffer.Buffer() + numBytesRead, input + 1, payloadLength);
				numBytesRead += payloadLength;
				sequence = (sequence + 1) % 64;

				if(last)
				{
					ReadOnlyBuffer buffer(rxBuffer.Buffer(), numBytesRead);
					numBytesRead = 0;
					mpContext->ReceiveAPDU(buffer);
				}
			}
		}
	}


}

bool TransportRx::ValidateHeader(bool fir, bool fin, uint8_t sequence_, uint32_t payloadSize)
{
	//get the transport byte and parse it

	if(fir)
	{
		sequence = sequence_; //always accept the sequence on FIR
		if(numBytesRead > 0)
		{
			// drop existing received bytes from segment
			//ERROR_BLOCK(flags::WARN, "FIR received mid-fragment, discarding: " << numBytesRead << "bytes", TLERR_NEW_FIR);
			numBytesRead = 0;
		}
	}
	else if(numBytesRead == 0)   //non-first packet with 0 prior bytes
	{
		//ERROR_BLOCK(flags::WARN, "non-FIR packet with 0 prior bytes", TLERR_MESSAGE_WITHOUT_FIR);
		return false;
	}

	if(sequence_ != sequence)
	{
		//ERROR_BLOCK(flags::WARN, "Ignoring bad sequence, got: " << sequence_ << " expected: " << sequence, TLERR_BAD_SEQUENCE);
		return false;
	}

	return true;
}

}

