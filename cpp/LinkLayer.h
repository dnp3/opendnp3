
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
#ifndef __LINK_LAYER_H_
#define __LINK_LAYER_H_

#include <queue>

#include "AsyncLayerInterfaces.h"
#include "IExecutor.h"
#include "ILinkContext.h"
#include "LinkFrame.h"
#include "LinkConfig.h"

namespace opendnp3
{

class ILinkRouter;
class PriStateBase;
class SecStateBase;

//	@section desc Implements the contextual state of DNP3 Data Link Layer
class LinkLayer : public ILowerLayer, public ILinkContext
{
public:

	LinkLayer(Logger*, IExecutor*, const LinkConfig& arConfig);

	void SetRouter(ILinkRouter*);

	// ILinkContext interface
	void OnLowerLayerUp();
	void OnLowerLayerDown();

	// IFrameSink interface
	void Ack(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc);
	void Nack(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc);
	void LinkStatus(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc);
	void NotSupported (bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc);
	void TestLinkStatus(bool aIsMaster, bool aFcb, uint16_t aDest, uint16_t aSrc);
	void ResetLinkStates(bool aIsMaster, uint16_t aDest, uint16_t aSrc);
	void RequestLinkStatus(bool aIsMaster, uint16_t aDest, uint16_t aSrc);
	void ConfirmedUserData(bool aIsMaster, bool aFcb, uint16_t aDest, uint16_t aSrc, const uint8_t* apData, size_t aDataLength);
	void UnconfirmedUserData(bool aIsMaster, uint16_t aDest, uint16_t aSrc, const uint8_t* apData, size_t aDataLength);

	// Functions called by the primary and secondary station states
	void ChangeState(PriStateBase*);
	void ChangeState(SecStateBase*);

	Logger* GetLogger() {
		return mpLogger;
	}

	void DoDataUp(const uint8_t* apData, size_t aLength) {
		if(mpUpperLayer) mpUpperLayer->OnReceive(apData, aLength);
	}

	void DoSendSuccess() {
		if(mpUpperLayer) mpUpperLayer->OnSendSuccess();
	}

	void DoSendFailure() {
		if(mpUpperLayer) mpUpperLayer->OnSendFailure();
	}

	void ResetReadFCB() {
		mNextReadFCB = true;
	}
	void ToggleReadFCB() {
		mNextReadFCB = !mNextReadFCB;
	}
	bool NextReadFCB() {
		return mNextReadFCB;
	}

	void ResetWriteFCB() {
		mNextWriteFCB = true;
	}
	void ToggleWriteFCB() {
		mNextWriteFCB = !mNextWriteFCB;
	}
	bool NextWriteFCB() {
		return mNextWriteFCB;
	}

	// Helpers for sending frames
	void SendAck();
	void SendLinkStatus();
	void SendResetLinks();
	void SendUnconfirmedUserData(const uint8_t* apData, size_t aLength);
	void SendDelayedUserData(bool aFCB);

	void StartTimer();
	void CancelTimer();

	const LinkConfig mCONFIG;

	//Retry Count
	void ResetRetry();
	bool Retry();
	size_t RetryRemaining() {
		return mRetryRemaining;
	}

	LinkFrame mPriFrame;
	LinkFrame mSecFrame;
	LinkFrame mDelayedPriFrame;

private:

	void Transmit(const LinkFrame&);

	size_t mRetryRemaining;

	IExecutor* mpExecutor;
	ITimer* mpTimer;

	// callback from the active timer
	void OnTimeout();

	bool mNextReadFCB;
	bool mNextWriteFCB;
	bool mIsOnline;

	bool Validate(bool aIsMaster, uint16_t aSrc, uint16_t aDest);

	/* Events - NVII delegates from ILayerDown and Events produced internally */
	void _Send(const uint8_t*, size_t);

	std::string SendString() {
		return "~>";
	}

	ILinkRouter* mpRouter;
	PriStateBase* mpPriState;
	SecStateBase* mpSecState;
};

}

#endif

