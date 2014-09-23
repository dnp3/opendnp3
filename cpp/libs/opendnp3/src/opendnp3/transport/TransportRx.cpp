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
#include "opendnp3/LogLevels.h"

#include <openpal/logging/LogMacros.h>
#include <openpal/util/ToHex.h>

#include <cstring>

using namespace std;
using namespace openpal;

namespace opendnp3
{

TransportRx::TransportRx(const Logger& logger_, uint32_t maxRxFragSize, StackStatistics* pStatistics_) :
	logger(logger_),	
	pStatistics(pStatistics_),
	rxBuffer(maxRxFragSize),	
	numBytesRead(0),
	sequence(0)	
{
	
}

void TransportRx::Reset()
{
	numBytesRead = 0;
	sequence = 0;
}

ReadOnlyBuffer TransportRx::ProcessReceive(const ReadOnlyBuffer& input)
{
	if (input.Size() == 0)
	{
		FORMAT_LOG_BLOCK_WITH_CODE(logger, flags::WARN, TLERR_NO_HEADER, "Received tpdu with no header");
		if (pStatistics) ++pStatistics->numTransportErrorRx;
		return ReadOnlyBuffer::Empty();
	}	
	else
	{
		uint8_t hdr = input[0];
		bool first = (hdr & TL_HDR_FIR) != 0;
		bool last = (hdr & TL_HDR_FIN) != 0;
		int seq = hdr & TL_HDR_SEQ;

		uint32_t payloadLength = input.Size() - 1;

		FORMAT_LOG_BLOCK(logger, flags::TRANSPORT_RX, "FIR: %d FIN: %d SEQ: %u LEN: %u", first, last, seq, payloadLength);

		if (this->ValidateHeader(first, last, seq, payloadLength))
		{
			auto remainder = rxBuffer.Size() - numBytesRead;

			if (remainder < payloadLength)
			{
				if (pStatistics) ++pStatistics->numTransportErrorRx;
				SIMPLE_LOG_BLOCK_WITH_CODE(logger, flags::WARN, TLERR_BUFFER_FULL, "Exceeded the buffer size before a complete fragment was read");
				numBytesRead = 0;
				if (pStatistics) ++pStatistics->numTransportErrorRx;
				return ReadOnlyBuffer::Empty();
			}
			else   //passed all validation
			{
				if (pStatistics) ++pStatistics->numTransportRx;

				memcpy(rxBuffer() + numBytesRead, input + 1, payloadLength);
				numBytesRead += payloadLength;
				sequence = (sequence + 1) % 64;

				if(last)
				{				
					ReadOnlyBuffer ret(rxBuffer(), numBytesRead);
					numBytesRead = 0;					
					return ret;
				}
				else
				{
					return ReadOnlyBuffer::Empty();
				}
			}
		}
		else
		{
			if (pStatistics) ++pStatistics->numTransportErrorRx;
			return ReadOnlyBuffer::Empty();
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
			SIMPLE_LOG_BLOCK_WITH_CODE(logger, flags::WARN, TLERR_NEW_FIR_MID_SEQUENCE, "FIR received mid-fragment, discarding previous bytes");
			numBytesRead = 0;
		}
	}
	else if(numBytesRead == 0)   //non-first packet with 0 prior bytes
	{
		SIMPLE_LOG_BLOCK_WITH_CODE(logger, flags::WARN, TLERR_MESSAGE_WITHOUT_FIR, "non-FIR packet with 0 prior bytes");
		return false;
	}

	if(sequence_ != sequence)
	{
		FORMAT_LOG_BLOCK_WITH_CODE(logger, flags::WARN, TLERR_BAD_SEQUENCE, "Ignoring bad sequence, got %i, expected %i", sequence_, sequence);
		return false;
	}

	return true;
}

}

