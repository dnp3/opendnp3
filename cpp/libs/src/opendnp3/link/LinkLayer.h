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
#ifndef OPENDNP3_LINKLAYER_H
#define OPENDNP3_LINKLAYER_H

#include <openpal/executor/IExecutor.h>
#include <openpal/logging/LogRoot.h>
#include <openpal/container/Settable.h>
#include <openpal/executor/TimerRef.h>

#include "opendnp3/gen/LinkStatus.h"

#include "opendnp3/link/ILinkLayer.h"
#include "opendnp3/link/ILinkSession.h"
#include "opendnp3/link/LinkLayerConstants.h"
#include "opendnp3/link/LinkConfig.h"
#include "opendnp3/link/ILinkListener.h"

namespace opendnp3
{

class ILinkRouter;
class PriStateBase;
class SecStateBase;

//	@section desc Implements the contextual state of DNP3 Data Link Layer
class LinkLayer final : public ILinkLayer, public ILinkSession, public HasUpperLayer
{
	enum class TransmitMode : uint8_t
	{
		Idle,
		Primary,
		Secondary
	};	

public:

	LinkLayer(openpal::LogRoot&, openpal::IExecutor&, opendnp3::ILinkListener&, const LinkConfig&);

	void SetRouter(ILinkRouter&);

	// ILinkSession interface
	virtual void OnLowerLayerUp() override;
	virtual void OnLowerLayerDown() override;
	virtual void OnTransmitResult(bool success) override;

	// IFrameSink interface
	virtual bool OnFrame(LinkFunction func, bool isMaster, bool fcb, bool fcvdfc, uint16_t dest, uint16_t source, const openpal::RSlice& userdata = openpal::RSlice()) override;

	// ------------- ILinkLayer --------------------
	virtual void Send(ITransportSegment& segments) override;

	// Functions called by the primary and secondary station states	
	void PostStatusCallback(opendnp3::LinkStatus status);	
	void CompleteSendOperation(bool success);
	void TryStartTransmission();

	openpal::Logger& GetLogger()
	{
		return logger;
	}
	
	void PushDataUp(const openpal::RSlice& data)
	{
		if (pUpperLayer)
		{
			pUpperLayer->OnReceive(data);
		}
	}	

	void ResetReadFCB()
	{
		nextReadFCB = true;
	}

	void ToggleReadFCB()
	{
		nextReadFCB = !nextReadFCB;
	}

	bool NextReadFCB()
	{
		return nextReadFCB;
	}

	void ResetWriteFCB()
	{
		nextWriteFCB = true;
	}

	void ToggleWriteFCB()
	{
		nextWriteFCB = !nextWriteFCB;
	}

	bool NextWriteFCB() const
	{
		return nextWriteFCB;
	}

	// Helpers for sending frames
	void QueueAck();
	void QueueLinkStatus();
	void QueueResetLinks();
	void QueueRequestLinkStatus();

	void StartTimer();
	void CancelTimer();

	openpal::Logger logger;
	const LinkConfig config;

	//Retry Count
	void ResetRetry();
	bool Retry();

	uint32_t RetryRemaining() const
	{
		return numRetryRemaining;
	}

	void QueueTransmit(const openpal::RSlice& buffer, bool primary);	

	// buffers used for primary and secondary requests	
	uint8_t priTxBuffer[LPDU_MAX_FRAME_SIZE];
	uint8_t secTxBuffer[LPDU_HEADER_SIZE];
	
	openpal::RSlice FormatPrimaryBufferWithUnconfirmed(const openpal::RSlice& tpdu);

	openpal::RSlice FormatPrimaryBufferWithConfirmed(const openpal::RSlice& tpdu, bool FCB);

	ITransportSegment* pSegments;
	bool isRemoteReset;

private:	

	bool OnFrameImpl(LinkFunction func, bool isMaster, bool fcb, bool fcvdfc, uint16_t dest, uint16_t source, const openpal::RSlice& userdata);
	void CheckPendingTx(openpal::Settable<openpal::RSlice>& pending, bool primary);
	void OnKeepAliveTimeout();
	void OnResponseTimeout();
	bool Validate(bool isMaster, uint16_t src, uint16_t dest);

	TransmitMode txMode;
	openpal::Settable<openpal::RSlice> pendingPriTx;
	openpal::Settable<openpal::RSlice> pendingSecTx;
	uint32_t numRetryRemaining;
	openpal::IExecutor* pExecutor;
	openpal::TimerRef rspTimeoutTimer;
	openpal::TimerRef keepAliveTimer;
	bool nextReadFCB;
	bool nextWriteFCB;
	bool isOnline;
	bool keepAliveTimeout;	
	openpal::MonotonicTimestamp lastMessageTimestamp;		
	ILinkRouter* pRouter;
	PriStateBase* pPriState;
	SecStateBase* pSecState;	
	ILinkListener* pListener;
};

}

#endif

