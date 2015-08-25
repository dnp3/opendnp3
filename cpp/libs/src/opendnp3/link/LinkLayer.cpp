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
#include "LinkLayer.h"

#include <assert.h>

#include <openpal/logging/LogMacros.h>

#include "opendnp3/ErrorCodes.h"
#include "opendnp3/link/ILinkRouter.h"
#include "opendnp3/link/LinkFrame.h"
#include "opendnp3/LogLevels.h"

#include "PriLinkLayerStates.h"
#include "SecLinkLayerStates.h"

using namespace openpal;


namespace opendnp3
{

LinkLayer::LinkLayer(openpal::LogRoot& root, openpal::IExecutor& executor, opendnp3::ILinkListener& linkListener, const LinkConfig& config_) :
	logger(root.GetLogger()),
	config(config_),
	pSegments(nullptr),
	txMode(TransmitMode::Idle),
	numRetryRemaining(0),
	pExecutor(&executor),
	rspTimeoutTimer(executor),
	keepAliveTimer(executor),
	nextReadFCB(false),
	nextWriteFCB(false),
	isOnline(false),
	lastMessageTimestamp(executor.GetTime()),
	pRouter(nullptr),
	pPriState(&PLLS_SecNotReset::Instance()),
	pSecState(&SLLS_NotReset::Instance()),
	pListener(&linkListener)
{}

void LinkLayer::SetRouter(ILinkRouter& router)
{
	assert(pRouter == nullptr);
	pRouter = &router;
}

void LinkLayer::PostStatusCallback(opendnp3::LinkStatus status)
{	
	auto callback = [this, status]()
	{
		this->pListener->OnStateChange(status);
	};

	pExecutor->PostLambda(callback);	
}

void LinkLayer::PostSendResult(bool success)
{
	if (pUpperLayer)
	{		
		auto callback = [this, success]() 
		{
			this->pUpperLayer->OnSendResult(success);
		};

		pExecutor->PostLambda(callback);		
	}	
}

bool LinkLayer::Validate(bool isMaster, uint16_t src, uint16_t dest)
{
	if (!isOnline)
	{
		SIMPLE_LOG_BLOCK(logger, flags::ERR, "Layer is not online");
		return false;
	}

	if (isMaster == config.IsMaster)
	{			
		SIMPLE_LOG_BLOCK_WITH_CODE(logger, flags::WARN, DLERR_WRONG_MASTER_BIT,
			(isMaster ? "Master frame received for master" : "Outstation frame received for outstation"));			            
			
		return false;
	}
	
	if (dest != config.LocalAddr)
	{
		SIMPLE_LOG_BLOCK_WITH_CODE(logger, flags::WARN, DLERR_UNKNOWN_DESTINATION, "Frame for unknown destintation");
		return false;
	}

	if (src != config.RemoteAddr)
	{
		SIMPLE_LOG_BLOCK_WITH_CODE(logger, flags::WARN, DLERR_UNKNOWN_SOURCE, "Frame from unknwon source");
		return false;		
	}

	return true;
}

////////////////////////////////
// ILowerLayer
////////////////////////////////

void LinkLayer::Send(ITransportSegment& segments)
{
	if (isOnline)
	{
		if (config.UseConfirms)
		{
			pPriState = &pPriState->OnSendConfirmed(*this, segments);
		}
		else
		{
			pPriState = &pPriState->OnSendUnconfirmed(*this, segments);
		}
	}
	else
	{
		SIMPLE_LOG_BLOCK(logger, flags::ERR, "Layer is not online");
	}
}

////////////////////////////////
// ILinkSession
////////////////////////////////

void LinkLayer::OnLowerLayerUp()
{
	if (isOnline)
	{
		SIMPLE_LOG_BLOCK(logger, flags::ERR, "Layer already online");
	}
	else
	{
		isOnline = true;

		// no reason to trigger a keep-alive until we've actually expired
		lastMessageTimestamp = this->pExecutor->GetTime();

		keepAliveTimer.Start(config.KeepAliveTimeout, [this]() { this->OnKeepAliveTimeout(); });

		if (pUpperLayer)
		{
			pUpperLayer->OnLowerLayerUp();
		}

		this->PostStatusCallback(opendnp3::LinkStatus::UNRESET);
	}
}

void LinkLayer::OnLowerLayerDown()
{
	if (isOnline)
	{
		isOnline = false;
		txMode = TransmitMode::Idle;
		pendingPriTx.Clear();
		pendingSecTx.Clear();

		rspTimeoutTimer.Cancel();
		keepAliveTimer.Cancel();

		pPriState = &PLLS_SecNotReset::Instance();
		pSecState = &SLLS_NotReset::Instance();

		if (pUpperLayer)
		{
			pUpperLayer->OnLowerLayerDown();
		}

		this->PostStatusCallback(opendnp3::LinkStatus::UNRESET);
	}
	else
	{
		SIMPLE_LOG_BLOCK(logger, flags::ERR, "Layer is not online");
	}
}

void LinkLayer::OnTransmitResult(bool success)
{
	if (txMode == TransmitMode::Idle)
	{
		SIMPLE_LOG_BLOCK(logger, flags::ERR, "Unknown transmission callback");
	}
	else
	{
		auto isPrimary = (txMode == TransmitMode::Primary);
		this->txMode = TransmitMode::Idle;

		// before we dispatch the transmit result, give any pending transmissions access first
		this->CheckPendingTx(pendingSecTx, false);
		this->CheckPendingTx(pendingPriTx, true);

		// now dispatch the completion event to the correct state handler
		if (isPrimary)
		{
			pPriState = &pPriState->OnTransmitResult(*this, success);
		}
		else
		{
			pSecState = &pSecState->OnTransmitResult(*this, success);
		}		
	}			
}

void LinkLayer::CheckPendingTx(openpal::Settable<RSlice>& pending, bool primary)
{
	if (txMode == TransmitMode::Idle && pending.IsSet())
	{
		pRouter->BeginTransmit(pending.Get(), this);
		pending.Clear();
		this->txMode = primary ? TransmitMode::Primary : TransmitMode::Secondary;
	}
}

void LinkLayer::OnKeepAliveTimeout()
{
	auto now = this->pExecutor->GetTime();

	auto elapsed = this->pExecutor->GetTime().milliseconds - this->lastMessageTimestamp.milliseconds;

	if (elapsed >= this->config.KeepAliveTimeout.GetMilliseconds())
	{
		this->lastMessageTimestamp = now;
		this->pListener->OnKeepAliveTimeout();
	}
	
	// No matter what, reschedule the timer based on last message timestamp
	MonotonicTimestamp expiration(this->lastMessageTimestamp.milliseconds + config.KeepAliveTimeout);
	this->keepAliveTimer.Start(expiration, [this]() { this->OnKeepAliveTimeout(); });
}

openpal::RSlice LinkLayer::FormatPrimaryBufferWithConfirmed(const openpal::RSlice& tpdu, bool FCB)
{
	auto buffer = WSlice(priTxBuffer, LPDU_MAX_FRAME_SIZE);
	auto output = LinkFrame::FormatConfirmedUserData(buffer, config.IsMaster, FCB, config.RemoteAddr, config.LocalAddr, tpdu, tpdu.Size(), &logger);
	FORMAT_HEX_BLOCK(logger, flags::LINK_TX_HEX, output, 10, 18);
	return output;	
}

RSlice LinkLayer::FormatPrimaryBufferWithUnconfirmed(const openpal::RSlice& tpdu)
{
	auto buffer = WSlice(priTxBuffer, LPDU_MAX_FRAME_SIZE);
	auto output = LinkFrame::FormatUnconfirmedUserData(buffer, config.IsMaster, config.RemoteAddr, config.LocalAddr, tpdu, tpdu.Size(), &logger);
	FORMAT_HEX_BLOCK(logger, flags::LINK_TX_HEX, output, 10, 18);
	return output;
}

void LinkLayer::QueueTransmit(const RSlice& buffer, bool primary)
{	
	if (txMode == TransmitMode::Idle)
	{
		txMode = primary ? TransmitMode::Primary : TransmitMode::Secondary;
		pRouter->BeginTransmit(buffer, this);
	}
	else
	{
		if (primary)
		{
			pendingPriTx.Set(buffer);
		}
		else
		{
			pendingSecTx.Set(buffer);
		}
	}
}

void LinkLayer::QueueAck()
{
	auto writeTo = WSlice(secTxBuffer, LPDU_HEADER_SIZE);
	auto buffer = LinkFrame::FormatAck(writeTo, config.IsMaster, false, config.RemoteAddr, config.LocalAddr, &logger);
	FORMAT_HEX_BLOCK(logger, flags::LINK_TX_HEX, buffer, 10, 18);
	this->QueueTransmit(buffer, false);
}

void LinkLayer::QueueLinkStatus()
{
	auto writeTo = WSlice(secTxBuffer, LPDU_HEADER_SIZE);
	auto buffer = LinkFrame::FormatLinkStatus(writeTo, config.IsMaster, false, config.RemoteAddr, config.LocalAddr, &logger);
	FORMAT_HEX_BLOCK(logger, flags::LINK_TX_HEX, buffer, 10, 18);
	this->QueueTransmit(buffer, false);
}

void LinkLayer::QueueResetLinks()
{
	auto writeTo = WSlice(priTxBuffer, LPDU_MAX_FRAME_SIZE);
	auto buffer = LinkFrame::FormatResetLinkStates(writeTo, config.IsMaster, config.RemoteAddr, config.LocalAddr, &logger);
	FORMAT_HEX_BLOCK(logger, flags::LINK_TX_HEX, buffer, 10, 18);
	this->QueueTransmit(buffer, true);
}

void LinkLayer::StartTimer()
{		
	rspTimeoutTimer.Start(
		TimeDuration(config.Timeout), 
		[this]() 
		{ 
			this->pPriState = &(this->pPriState->OnTimeout(*this));
		}
	);	
}

void LinkLayer::CancelTimer()
{
	rspTimeoutTimer.Cancel();
}

void LinkLayer::ResetRetry()
{
	this->numRetryRemaining = config.NumRetry;
}

bool LinkLayer::Retry()
{
	if(numRetryRemaining > 0)
	{
		--numRetryRemaining;
		return true;
	}
	else
	{
		return false;
	}
}

////////////////////////////////
// IFrameSink
////////////////////////////////

bool LinkLayer::OnFrame(LinkFunction func, bool isMaster, bool fcb, bool fcvdfc, uint16_t dest, uint16_t source, const openpal::RSlice& userdata)
{
	if (!this->Validate(isMaster, source, dest))
	{
		return false;
	}

	// reset the keep-alive timestamp
	this->lastMessageTimestamp = this->pExecutor->GetTime();

	switch (func)
	{
		case(LinkFunction::SEC_ACK) :
			pPriState = &pPriState->OnAck(*this, fcvdfc);
			return true;
		case(LinkFunction::SEC_NACK) :
			pPriState = &pPriState->OnNack(*this, fcvdfc);
			return true;
		case(LinkFunction::SEC_LINK_STATUS) :
			pPriState = &pPriState->OnLinkStatus(*this, fcvdfc);
			return true;
		case(LinkFunction::SEC_NOT_SUPPORTED) :
			pPriState = &pPriState->OnNotSupported(*this, fcvdfc);
			return true;
		case(LinkFunction::PRI_TEST_LINK_STATES) :
			pSecState = &pSecState->OnTestLinkStatus(*this, fcb);
			return true;
		case(LinkFunction::PRI_RESET_LINK_STATES) :
			pSecState = &pSecState->OnResetLinkStates(*this);
			return true;
		case(LinkFunction::PRI_REQUEST_LINK_STATUS) :
			pSecState = &pSecState->OnRequestLinkStatus(*this);
			return true;
		case(LinkFunction::PRI_CONFIRMED_USER_DATA) :
			pSecState = &pSecState->OnConfirmedUserData(*this, fcb, userdata);
			return true;
		case(LinkFunction::PRI_UNCONFIRMED_USER_DATA) :
			this->PushDataUp(userdata);
			return true;
		default:
			return false;
	}
}

}

