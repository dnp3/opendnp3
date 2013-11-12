
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
// you under the terms of the License.
//

#ifndef __APP_LAYER_H_
#define __APP_LAYER_H_

#include <queue>

#include <openpal/AsyncLayerInterfaces.h>

#include "APDU.h"
#include "AppInterfaces.h"
#include "SolicitedChannel.h"
#include "UnsolicitedChannel.h"

#include <opendnp3/AppConfig.h>

namespace openpal {
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
class DLL_LOCAL AppLayer : public IUpperLayer, public IAppLayer
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
	void SendUnsolicited(APDU&);
	void SendResponse(APDU&);
	void SendRequest(APDU&);
	void CancelResponse();

private:

	////////////////////
	// External Events
	////////////////////

	// Parse the header of the incoming APDU and direct to the appropriate
	// internal event handler
	void _OnReceive(const uint8_t*, size_t);

	void _OnLowerLayerUp();
	void _OnLowerLayerDown();
	void _OnSendSuccess();
	void _OnSendFailure();

	void OnSendResult(bool aSuccess);

	////////////////////
	// Internal Events
	////////////////////

	void OnUnknownObject(FunctionCodes aCode, const AppControlField&);
	void OnResponse(const AppControlField&, APDU&);
	void OnUnsolResponse(const AppControlField&, APDU&);
	void OnConfirm(const AppControlField&, APDU&);
	void OnRequest(const AppControlField&, APDU&);

	////////////////////
	// State
	////////////////////

	typedef std::deque<const APDU*> SendQueue;

	APDU mIncoming;						// Fragment used to parse all incoming requests
	APDU mConfirm;						// Fragment used to do confirms

	bool mSending;						// State of send operation to the lower layer
	bool mConfirmSending;
	SendQueue mSendQueue;				// Buffer of send operations

	IAppUser* mpUser;				// Interface for dispatching callbacks

	SolicitedChannel mSolicited;			// Channel used for solicited communications
	UnsolicitedChannel mUnsolicited;		// Channel used for unsolicited communications
	size_t mNumRetry;


	////////////////////
	// Helpers
	////////////////////

	void QueueConfirm(bool aUns, int aSeq);
	void QueueFrame(const APDU& arAPDU);
	void CheckForSend();
	size_t GetRetries(FunctionCodes aCode);

	void Validate(const AppControlField& arCtrl, bool aMaster, bool aRequireFIRFIN, bool aAllowCON, bool aUNS);
};

}

#endif
