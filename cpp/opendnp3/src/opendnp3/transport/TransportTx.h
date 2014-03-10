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
#ifndef __TRANSPORT_TX_H_
#define __TRANSPORT_TX_H_


#include <openpal/Loggable.h>
#include <openpal/BufferWrapper.h>
#include <openpal/StaticBuffer.h>

#include "opendnp3/transport/TransportConstants.h"
#include "opendnp3/StaticSizeConfiguration.h"

namespace opendnp3
{

class TransportLayer;

/**
State/validation for the DNP3 transport layer's send channel.
*/
class TransportTx : public openpal::Loggable
{
public:
	TransportTx(openpal::Logger&, TransportLayer*, uint32_t aFragSize);


	void Send(const openpal::ReadOnlyBuffer& output); // A fresh call to Send() will reset the state
	bool SendSuccess();

	static uint8_t GetHeader(bool fir, bool fin, uint8_t sequence);

private:

	bool CheckForSend();

	TransportLayer* mpContext;

	openpal::StaticBuffer<sizes::MAX_APDU_BUFFER_SIZE> underlying;
	openpal::WriteBuffer apduBuffer;
	openpal::StaticBuffer<TL_MAX_TPDU_LENGTH> tpduBuffer;

	uint32_t numBytesSent;
	uint32_t numBytesToSend;

	uint8_t sequence;

	uint32_t BytesRemaining() const
	{
		return numBytesToSend - numBytesSent;
	}
};

}

#endif

