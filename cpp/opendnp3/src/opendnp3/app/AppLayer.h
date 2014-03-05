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

#include <queue>

#include <openpal/AsyncLayerInterfaces.h>

#include "opendnp3/app/IAppUser.h"
#include "opendnp3/app/IAppLayer.h"

#include "opendnp3/app/SolicitedChannel.h"
#include "opendnp3/app/UnsolicitedChannel.h"
#include "opendnp3/app/APDUHeaderParser.h"
#include "opendnp3/app/APDURequest.h"
#include "opendnp3/app/APDUResponse.h"
#include "opendnp3/app/AppConfig.h"

#include <openpal/StaticArray.h>

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
class AppLayer : public IUpperLayer, public IAppLayer
{
	friend class AppLayerChannel;
	friend class SolicitedChannel;
	friend class UnsolicitedChannel;

public:

	AppLayer(openpal::Logger aLogger, openpal::IExecutor*, AppConfig aAppCfg);

	void SetUser(IAppUser*);

	/////////////////////////////////
	// Implement IAppLayer
	/////////////////////////////////

	void SendUnsolicited(APDUResponse &);
	void SendResponse(APDUResponse &);
	void SendRequest(APDURequest &);
	void CancelResponse();

private:

	////////////////////
	// External Events
	////////////////////

	// Parse the header of the incoming APDU and direct to the appropriate
	// internal event handler
	void _OnReceive(const openpal::ReadOnlyBuffer&);

	void _OnLowerLayerUp();
	void _OnLowerLayerDown();
	void _OnSendSuccess();
	void _OnSendFailure();

	void OnSendResult(bool aSuccess);

	////////////////////
	// Internal Events
	////////////////////
	
	void OnResponse(const APDUResponseRecord&);
	void OnUnsolResponse(const APDUResponseRecord&);	
	void OnRequest(const APDURecord&);
	void OnConfirm(const AppControlField& aControl, size_t aDataSize);

	void LogParseError(APDUHeaderParser::Result error, bool aIsResponse);

	////////////////////
	// State
	////////////////////

	typedef std::deque<APDUWrapper> SendQueue;	

	bool mSending;						// State of send operation to the lower layer
	bool mConfirmSending;
	bool mIsMaster;						// True, if the application user is a master
	SendQueue mSendQueue;				// Buffer of send operations

	IAppUser* mpUser;					// Interface for dispatching callbacks

	SolicitedChannel mSolicited;			// Channel used for solicited communications
	UnsolicitedChannel mUnsolicited;		// Channel used for unsolicited communications
	size_t mNumRetry;


	// a 2 byter buffer and wrapper for the confirms
	uint8_t confirmBuffer[2];
	APDUWrapper confirmAPDU;


	////////////////////
	// Helpers
	////////////////////

	size_t GetRetries(FunctionCode aCode);
	void QueueConfirm(bool aUns, int aSeq);
	void QueueFrame(const APDUWrapper& apdu);
	void CheckForSend();
	
};

}

#endif
