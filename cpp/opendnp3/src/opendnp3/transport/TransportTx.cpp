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

TransportTx::TransportTx() : sequence(0), tpduCount(0)
{

}

void TransportTx::Configure(const openpal::ReadOnlyBuffer& output)
{
	assert(output.IsNotEmpty());
	this->apdu = output;
	this->tpduCount = 0;
}

bool TransportTx::HasNext() const
{
	return apdu.Size() > 0;
}

openpal::ReadOnlyBuffer TransportTx::Next()
{
	assert(apdu.Size() > 0);
	uint8_t numToSend = apdu.Size() < TL_MAX_TPDU_PAYLOAD ? static_cast<uint8_t>(apdu.Size()) : TL_MAX_TPDU_PAYLOAD;
	memcpy(tpduBuffer.Buffer() + 1, apdu, numToSend);
	apdu.Advance(numToSend);

	bool fir = (tpduCount == 0);	
	bool fin = apdu.IsEmpty();
	++tpduCount;
	tpduBuffer[0] = GetHeader(fir, fin, sequence);
	sequence = (sequence + 1) % 64;

	return ReadOnlyBuffer(tpduBuffer.Buffer(), numToSend + 1);
}	

uint8_t TransportTx::GetHeader(bool fir, bool fin, uint8_t sequence)
{
	uint8_t hdr = 0;
	
	if (fir)
	{
		hdr |= TL_HDR_FIR;
	}

	if (fin)
	{
		hdr |= TL_HDR_FIN;
	}

	// Only the lower 6 bits of the sequence number
	hdr |= TL_HDR_SEQ & sequence;

	return hdr;
}

}

