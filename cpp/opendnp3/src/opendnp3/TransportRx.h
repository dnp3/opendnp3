
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
#ifndef __TRANSPORT_RX_H_
#define __TRANSPORT_RX_H_

#include <opendnp3/TransportConstants.h>
#include <openpal/Visibility.h>
#include <openpal/Loggable.h>

#include "CopyableBuffer.h"

namespace opendnp3
{

class TransportLayer;

/**
State/validation for the DNP3 transport layer's receive channel.
*/
class DLL_LOCAL TransportRx : public openpal::Loggable
{
public:
	TransportRx(openpal::Logger&, TransportLayer*, size_t aFragSize);

	void HandleReceive(const uint8_t*, size_t);

	void Reset();

private:

	bool ValidateHeader(bool aFir, bool aFin, int aSeq, size_t aPayloadSize);

	TransportLayer* mpContext;

	CopyableBuffer mBuffer;
	size_t mNumBytesRead;
	int mSeq;



	size_t BufferRemaining() {
		return mBuffer.Size() - mNumBytesRead;
	}
};

}

#endif

