
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
#ifndef __TRANSPORT_LAYER_H_
#define __TRANSPORT_LAYER_H_

#include "TransportRx.h"
#include "TransportTx.h"
#include "AsyncLayerInterfaces.h"

#include <opendnp3/DNPConstants.h>
#include <openpal/Visibility.h>

namespace opendnp3
{

class TLS_Base;

/** Implements the DNP3 transport layer as a generic
asynchronous protocol stack layer
*/
class DLL_LOCAL TransportLayer : public IUpperLayer, public ILowerLayer
{
public:

	TransportLayer(Logger* apLogger, size_t aFragSize = DEFAULT_FRAG_SIZE);
	virtual ~TransportLayer() {}

	/* Actions - Taken by the states/transmitter/receiver in response to events */

	void ThisLayerUp();
	void ThisLayerDown();
	void ChangeState(TLS_Base* apNewState);

	void TransmitAPDU(const uint8_t* apData, size_t aNumBytes);
	void TransmitTPDU(const uint8_t* apData, size_t aNumBytes);
	void ReceiveAPDU(const uint8_t* apData, size_t aNumBytes);
	void ReceiveTPDU(const uint8_t* apData, size_t aNumBytes);

	bool ContinueSend(); // return true if
	void SignalSendSuccess();
	void SignalSendFailure();
#ifndef OPENDNP3_STRIP_LOG_MESSAGES
	/* Events - NVII delegates from ILayerUp/ILayerDown and Events produced internally */
	static std::string ToString(uint8_t aHeader);
#endif

private:

	//delegated to the states
	void _Send(const uint8_t*, size_t); //Implement ILowerLayer
	void _OnReceive(const uint8_t*, size_t); //Implement IUpperLayer
	void _OnLowerLayerUp();
	void _OnLowerLayerDown();
	void _OnSendSuccess();
	void _OnSendFailure();
	void _OnLowerLayerShutdown();

	/* Members and Helpers */
	TLS_Base* mpState;

	const size_t M_FRAG_SIZE;

	/* Transmitter and Receiver Classes */
	TransportRx mReceiver;
	TransportTx mTransmitter;

	bool mThisLayerUp;
};

}

#endif

