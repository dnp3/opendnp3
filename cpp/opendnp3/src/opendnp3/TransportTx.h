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

