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
#ifndef __LINK_LAYER_H_
#define __LINK_LAYER_H_

#include <queue>


#include <openpal/AsyncLayerInterfaces.h>
#include <openpal/IExecutor.h>
#include <openpal/Loggable.h>

#include "ILinkContext.h"
#include "LinkFrame.h"

#include "LinkConfig.h"

namespace opendnp3
{

class ILinkRouter;
class PriStateBase;
class SecStateBase;

//	@section desc Implements the contextual state of DNP3 Data Link Layer
class LinkLayer : public openpal::ILowerLayer, public ILinkContext, private openpal::Loggable
{
public:

	LinkLayer(openpal::Logger, openpal::IExecutor*, const LinkConfig& arConfig);

	void SetRouter(ILinkRouter*);

	// ILinkContext interface
	virtual void OnLowerLayerUp() override final;
	virtual void OnLowerLayerDown() override final;
	virtual void OnTransmitResult(bool primary, bool success) override final;

	// IFrameSink interface
	virtual void Ack(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc) override final;
	virtual void Nack(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc) override final;
	virtual void LinkStatus(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc) override final;
	virtual void NotSupported(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc) override final;
	virtual void TestLinkStatus(bool aIsMaster, bool aFcb, uint16_t aDest, uint16_t aSrc) override final;
	virtual void ResetLinkStates(bool aIsMaster, uint16_t aDest, uint16_t aSrc) override final;
	virtual void RequestLinkStatus(bool aIsMaster, uint16_t aDest, uint16_t aSrc) override final;
	virtual void ConfirmedUserData(bool aIsMaster, bool aFcb, uint16_t aDest, uint16_t aSrc, const openpal::ReadOnlyBuffer& arBuffer) override final;
	virtual void UnconfirmedUserData(bool aIsMaster, uint16_t aDest, uint16_t aSrc, const openpal::ReadOnlyBuffer& arBuffer) override final;

	// ------------- ILowerLayer --------------------
	virtual void Send(const openpal::ReadOnlyBuffer& arBuffer) override final;

	// Functions called by the primary and secondary station states
	void ChangeState(PriStateBase*);
	void ChangeState(SecStateBase*);

	openpal::Logger& GetLogger()
	{
		return logger;
	}

	void DoDataUp(const openpal::ReadOnlyBuffer& arBuffer)
	{
		if(pUpperLayer) pUpperLayer->OnReceive(arBuffer);
	}

	void DoSendSuccess()
	{
		if (pUpperLayer) pUpperLayer->OnSendResult(true);
	}

	void DoSendFailure()
	{
		if (pUpperLayer) pUpperLayer->OnSendResult(false);
	}

	void ResetReadFCB()
	{
		mNextReadFCB = true;
	}
	void ToggleReadFCB()
	{
		mNextReadFCB = !mNextReadFCB;
	}
	bool NextReadFCB()
	{
		return mNextReadFCB;
	}

	void ResetWriteFCB()
	{
		mNextWriteFCB = true;
	}
	void ToggleWriteFCB()
	{
		mNextWriteFCB = !mNextWriteFCB;
	}
	bool NextWriteFCB()
	{
		return mNextWriteFCB;
	}

	// Helpers for sending frames
	void QueueAck();
	void QueueLinkStatus();
	void QueueResetLinks();
	void QueueUnconfirmedUserData(const openpal::ReadOnlyBuffer&);
	void QueueDelayedUserData(bool aFCB);

	void StartTimer();
	void CancelTimer();

	const LinkConfig mCONFIG;

	//Retry Count
	void ResetRetry();
	bool Retry();
	size_t RetryRemaining()
	{
		return mRetryRemaining;
	}

	LinkFrame mPriFrame;
	LinkFrame mSecFrame;
	LinkFrame mDelayedPriFrame;

private:

	void QueueTransmit(const openpal::ReadOnlyBuffer& buffer, bool primary);

	size_t mRetryRemaining;

	openpal::IExecutor* mpExecutor;
	openpal::ITimer* mpTimer;

	// callback from the active timer
	void OnTimeout();

	bool mNextReadFCB;
	bool mNextWriteFCB;
	bool mIsOnline;

	bool Validate(bool aIsMaster, uint16_t aSrc, uint16_t aDest);	

	std::string SendString()
	{
		return "~>";
	}

	ILinkRouter* mpRouter;
	PriStateBase* mpPriState;
	SecStateBase* mpSecState;
};

}

#endif

