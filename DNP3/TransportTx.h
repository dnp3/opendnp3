
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
#ifndef __TRANSPORT_TX_H_
#define __TRANSPORT_TX_H_


#include <APL/Types.h>
#include <APL/Loggable.h>
#include <APL/CopyableBuffer.h>

#include "TransportConstants.h"

namespace apl
{
namespace dnp
{

class TransportLayer;

/**
State/validation for the DNP3 transport layer's send channel.
*/
class TransportTx : public Loggable
{
public:
	TransportTx(Logger*, TransportLayer*, size_t aFragSize);


	void Send(const uint8_t*, size_t); // A fresh call to Send() will reset the state
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
}

#endif
