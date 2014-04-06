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
#ifndef __APP_LAYER_H_
#define __APP_LAYER_H_

#include <openpal/AsyncLayerInterfaces.h>
#include <openpal/Loggable.h>

#include "opendnp3/app/IAppUser.h"
#include "opendnp3/app/IAppLayer.h"

#include "opendnp3/app/SolicitedChannel.h"
#include "opendnp3/app/UnsolicitedChannel.h"
#include "opendnp3/app/APDUHeaderParser.h"
#include "opendnp3/app/APDURequest.h"
#include "opendnp3/app/APDUResponse.h"
#include "opendnp3/app/AppConfig.h"

#include <openpal/StaticArray.h>
#include <openpal/StaticQueue.h>

namespace openpal
{
class IExecutor;
class ITimer;
}


namespace opendnp3
{

class AppTransactionStateBase;

/**
Implements the sequencing/confirm/response logic for the DNP3 application layer.

Allows for canceling response transactions, as dictated by the spec.
*/
class AppLayer : public IUpperLayer, public IAppLayer, private openpal::Loggable
{
	friend class AppLayerChannel;
	friend class SolicitedChannel;
	friend class UnsolicitedChannel;

public:

	AppLayer(const openpal::Logger& logger, openpal::IExecutor*,  const AppConfig&);

	void SetUser(IAppUser*);

	void SetTransportLayer(openpal::ILowerLayer* pTransportLayer_);

	/////////////////////////////////
	// IAppLayer
	/////////////////////////////////

	virtual void SendUnsolicited(APDUResponse&) override final;
	virtual void SendResponse(APDUResponse&) override final;
	virtual void SendRequest(APDURequest&) override final;
	virtual void CancelResponse() override final;


	////////////////////
	// IUpperLayer
	////////////////////

	virtual void OnReceive(const openpal::ReadOnlyBuffer&) override final;
	virtual void OnLowerLayerUp() override final;
	virtual void OnLowerLayerDown() override final;
	virtual void OnSendResult(bool isSuccess) override final;

private:

	////////////////////
	// Internal Events
	////////////////////

	void OnResponse(const APDUResponseRecord&);
	void OnUnsolResponse(const APDUResponseRecord&);
	void OnRequest(const APDURecord&);
	void OnConfirm(const AppControlField& aControl, uint32_t aDataSize);

	void LogParseError(APDUHeaderParser::Result error, bool aIsResponse);

	////////////////////
	// State
	////////////////////	

	bool isOnline;
	bool isSending;						// State of send operation to the lower layer
	bool isConfirmSending;
	bool isMaster;						// True, if the application user is a master

	openpal::StaticQueue<APDUWrapper, uint16_t, 4>	sendQueue; // Buffer of send operations - TODO - decide reasonable size

	IAppUser* mpUser;						// Interface for dispatching callbacks
	openpal::ILowerLayer* pTransportLayer;
	

	SolicitedChannel mSolicited;			// Channel used for solicited communications
	UnsolicitedChannel mUnsolicited;		// Channel used for unsolicited communications
	uint32_t numRetry;


	// a 2 byter buffer and wrapper for the confirms
	uint8_t confirmBuffer[2];
	APDUWrapper confirmAPDU;


	////////////////////
	// Helpers
	////////////////////

	uint32_t GetRetries(FunctionCode aCode);
	void QueueConfirm(bool aUns, int aSeq);
	void QueueFrame(const APDUWrapper& apdu);
	void CheckForSend();

};

}

#endif
