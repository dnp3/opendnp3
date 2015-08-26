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

LinkLayer::LinkLayer(openpal::LogRoot& root, openpal::IExecutor& executor, opendnp3::ILinkListener& listener, const LinkConfig& config) :
	ctx(root, executor, listener, config)
{}

void LinkLayer::SetRouter(ILinkRouter& router)
{
	assert(ctx.pRouter == nullptr);
	ctx.pRouter = &router;
}

void LinkLayer::PostStatusCallback(opendnp3::LinkStatus status)
{	
	auto callback = [this, status]()
	{
		this->ctx.pListener->OnStateChange(status);
	};

	ctx.pExecutor->PostLambda(callback);
}

void LinkLayer::CompleteSendOperation(bool success)
{
	this->ctx.pSegments = nullptr;

	if (pUpperLayer)
	{		
		auto callback = [this, success]() 
		{
			this->pUpperLayer->OnSendResult(success);
		};

		ctx.pExecutor->PostLambda(callback);
	}	
}

bool LinkLayer::Validate(bool isMaster, uint16_t src, uint16_t dest)
{
	if (!ctx.isOnline)
	{
		SIMPLE_LOG_BLOCK(ctx.logger, flags::ERR, "Layer is not online");
		return false;
	}

	if (isMaster == ctx.config.IsMaster)
	{			
		SIMPLE_LOG_BLOCK_WITH_CODE(ctx.logger, flags::WARN, DLERR_WRONG_MASTER_BIT,
			(isMaster ? "Master frame received for master" : "Outstation frame received for outstation"));			            
			
		return false;
	}
	
	if (dest != ctx.config.LocalAddr)
	{
		SIMPLE_LOG_BLOCK_WITH_CODE(ctx.logger, flags::WARN, DLERR_UNKNOWN_DESTINATION, "Frame for unknown destintation");
		return false;
	}

	if (src != ctx.config.RemoteAddr)
	{
		SIMPLE_LOG_BLOCK_WITH_CODE(ctx.logger, flags::WARN, DLERR_UNKNOWN_SOURCE, "Frame from unknwon source");
		return false;		
	}

	return true;
}

////////////////////////////////
// ILowerLayer
////////////////////////////////

void LinkLayer::Send(ITransportSegment& segments)
{
	if (!ctx.isOnline)
	{
		SIMPLE_LOG_BLOCK(ctx.logger, flags::ERR, "Layer is not online");
		return;
	}

	if (ctx.pSegments)
	{
		SIMPLE_LOG_BLOCK(ctx.logger, flags::ERR, "Already transmitting a segment");
		return;
	}

	this->ctx.pSegments = &segments;
	this->TryStartTransmission();
}

void LinkLayer::TryStartTransmission()
{
	if (ctx.keepAliveTimeout)
	{
		ctx.pPriState = &ctx.pPriState->TrySendRequestLinkStatus(*this);
	}

	if (ctx.pSegments)
	{		
		ctx.pPriState = (ctx.config.UseConfirms) ? &ctx.pPriState->TrySendConfirmed(*this, *ctx.pSegments) : &ctx.pPriState->TrySendUnconfirmed(*this, *ctx.pSegments);
	}		
}

void LinkLayer::FailKeepAlive(bool timeout)
{
	this->ctx.keepAliveTimeout = false;

	if (timeout)
	{
		this->ctx.pListener->OnKeepAliveFailure();
	}
}

void LinkLayer::CompleteKeepAlive()
{
	this->ctx.keepAliveTimeout = false;
}

void LinkLayer::PushDataUp(const openpal::RSlice& data)
{
	if (pUpperLayer)
	{
		pUpperLayer->OnReceive(data);
	}
}

////////////////////////////////
// ILinkSession
////////////////////////////////

bool LinkLayer::OnLowerLayerUp()
{
	auto ret = ctx.OnLowerLayerUp();
	if (ret)
	{
		ctx.keepAliveTimer.Start(ctx.config.KeepAliveTimeout, [this]() { this->OnKeepAliveTimeout(); });

		if (pUpperLayer)
		{
			pUpperLayer->OnLowerLayerUp();
		}

		this->PostStatusCallback(opendnp3::LinkStatus::UNRESET);
	}
	return ret;	
}

bool LinkLayer::OnLowerLayerDown()
{
	auto ret = ctx.OnLowerLayerDown();
	
	if (ret)
	{
		if (pUpperLayer)
		{
			pUpperLayer->OnLowerLayerDown();
		}

		this->PostStatusCallback(opendnp3::LinkStatus::UNRESET);
	}

	return ret;
}

bool LinkLayer::OnTransmitResult(bool success)
{
	if (ctx.txMode == LinkTransmitMode::Idle)
	{
		SIMPLE_LOG_BLOCK(ctx.logger, flags::ERR, "Unknown transmission callback");
		return false;
	}
	
	auto isPrimary = (ctx.txMode == LinkTransmitMode::Primary);
	ctx.txMode = LinkTransmitMode::Idle;

	// before we dispatch the transmit result, give any pending transmissions access first
	this->CheckPendingTx(ctx.pendingSecTx, false);
	this->CheckPendingTx(ctx.pendingPriTx, true);

	// now dispatch the completion event to the correct state handler
	if (isPrimary)
	{
		ctx.pPriState = &ctx.pPriState->OnTransmitResult(*this, success);
	}
	else
	{
		ctx.pSecState = &ctx.pSecState->OnTransmitResult(*this, success);
	}	

	this->TryStartTransmission();	
}

void LinkLayer::CheckPendingTx(openpal::Settable<RSlice>& pending, bool primary)
{
	if (ctx.txMode == LinkTransmitMode::Idle && pending.IsSet())
	{
		ctx.pRouter->BeginTransmit(pending.Get(), this);
		pending.Clear();
		this->ctx.txMode = primary ? LinkTransmitMode::Primary : LinkTransmitMode::Secondary;
	}
}

void LinkLayer::OnKeepAliveTimeout()
{
	auto now = this->ctx.pExecutor->GetTime();

	auto elapsed = this->ctx.pExecutor->GetTime().milliseconds - this->ctx.lastMessageTimestamp.milliseconds;

	if (elapsed >= this->ctx.config.KeepAliveTimeout.GetMilliseconds())
	{
		this->ctx.lastMessageTimestamp = now;
		this->ctx.keepAliveTimeout = true;
		this->ctx.pListener->OnKeepAliveTimeout();
	}
	
	// No matter what, reschedule the timer based on last message timestamp
	MonotonicTimestamp expiration(this->ctx.lastMessageTimestamp.milliseconds + ctx.config.KeepAliveTimeout);
	this->ctx.keepAliveTimer.Start(expiration, [this]() { this->OnKeepAliveTimeout(); });
}

void LinkLayer::OnResponseTimeout()
{
	this->ctx.pPriState = &(this->ctx.pPriState->OnTimeout(*this));

	this->TryStartTransmission();
}

void LinkLayer::StartTimer()
{		
	ctx.rspTimeoutTimer.Start(
		TimeDuration(ctx.config.Timeout),
		[this]() 
		{ 
			this->OnResponseTimeout();
		}
	);	
}

void LinkLayer::CancelTimer()
{
	ctx.rspTimeoutTimer.Cancel();
}

////////////////////////////////
// IFrameSink
////////////////////////////////

bool LinkLayer::OnFrame(LinkFunction func, bool isMaster, bool fcb, bool fcvdfc, uint16_t dest, uint16_t source, const openpal::RSlice& userdata)
{
	auto ret = this->OnFrameImpl(func, isMaster, fcb, fcvdfc, dest, source, userdata);
	this->TryStartTransmission();
	return ret;
}

bool LinkLayer::OnFrameImpl(LinkFunction func, bool isMaster, bool fcb, bool fcvdfc, uint16_t dest, uint16_t source, const openpal::RSlice& userdata)
{	
	if (!this->Validate(isMaster, source, dest))
	{
		return false;
	}

	// reset the keep-alive timestamp
	this->ctx.lastMessageTimestamp = this->ctx.pExecutor->GetTime();

	switch (func)
	{
		case(LinkFunction::SEC_ACK) :
			ctx.pPriState = &ctx.pPriState->OnAck(*this, fcvdfc);
			return true;
		case(LinkFunction::SEC_NACK) :
			ctx.pPriState = &ctx.pPriState->OnNack(*this, fcvdfc);
			return true;
		case(LinkFunction::SEC_LINK_STATUS) :
			ctx.pPriState = &ctx.pPriState->OnLinkStatus(*this, fcvdfc);
			return true;
		case(LinkFunction::SEC_NOT_SUPPORTED) :
			ctx.pPriState = &ctx.pPriState->OnNotSupported(*this, fcvdfc);
			return true;
		case(LinkFunction::PRI_TEST_LINK_STATES) :
			ctx.pSecState = &ctx.pSecState->OnTestLinkStatus(*this, fcb);
			return true;
		case(LinkFunction::PRI_RESET_LINK_STATES) :
			ctx.pSecState = &ctx.pSecState->OnResetLinkStates(*this);
			return true;
		case(LinkFunction::PRI_REQUEST_LINK_STATUS) :
			ctx.pSecState = &ctx.pSecState->OnRequestLinkStatus(*this);
			return true;
		case(LinkFunction::PRI_CONFIRMED_USER_DATA) :
			ctx.pSecState = &ctx.pSecState->OnConfirmedUserData(*this, fcb, userdata);
			return true;
		case(LinkFunction::PRI_UNCONFIRMED_USER_DATA) :
			this->PushDataUp(userdata);
			return true;
		default:
			return false;
	}
}

}

