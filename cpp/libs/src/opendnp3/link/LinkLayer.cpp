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
	timer(executor),
	nextReadFCB(false),
	nextWriteFCB(false),
	isOnline(false),
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

		timer.Cancel();

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
	timer.Start(
		TimeDuration(config.Timeout), 
		[this]() 
		{ 
			this->pPriState = &(this->pPriState->OnTimeout(*this));
		}
	);	
}

void LinkLayer::CancelTimer()
{
	timer.Cancel();
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

void LinkLayer::Ack(bool isMaster, bool rxBuffFull, uint16_t dest, uint16_t src)
{
	if (this->Validate(isMaster, src, dest))
	{
		pPriState = &pPriState->OnAck(*this, rxBuffFull);
	}
}

void LinkLayer::Nack(bool isMaster, bool rxBuffFull, uint16_t dest, uint16_t src)
{
	if (this->Validate(isMaster, src, dest))
	{
		pPriState = &pPriState->OnNack(*this, rxBuffFull);
	}
}

void LinkLayer::LinkStatus(bool isMaster, bool rxBuffFull, uint16_t dest, uint16_t src)
{
	if (this->Validate(isMaster, src, dest))
	{
		pPriState = &pPriState->OnLinkStatus(*this, rxBuffFull);
	}
}

void LinkLayer::NotSupported (bool isMaster, bool rxBuffFull, uint16_t dest, uint16_t src)
{
	if (this->Validate(isMaster, src, dest))
	{
		pPriState = &pPriState->OnNotSupported(*this, rxBuffFull);
	}
}

void LinkLayer::TestLinkStatus(bool isMaster, bool aFcb, uint16_t dest, uint16_t src)
{
	if (this->Validate(isMaster, src, dest))
	{
		pSecState = &pSecState->OnTestLinkStatus(*this, aFcb);
	}
}

void LinkLayer::ResetLinkStates(bool isMaster, uint16_t dest, uint16_t src)
{
	if (this->Validate(isMaster, src, dest))
	{
		pSecState = &pSecState->OnResetLinkStates(*this);
	}
}

void LinkLayer::RequestLinkStatus(bool isMaster, uint16_t dest, uint16_t src)
{
	if (this->Validate(isMaster, src, dest))
	{
		pSecState = &pSecState->OnRequestLinkStatus(*this);
	}
}

void LinkLayer::ConfirmedUserData(bool isMaster, bool aFcb, uint16_t dest, uint16_t src, const RSlice& input)
{
	if (this->Validate(isMaster, src, dest))
	{
		pSecState = &pSecState->OnConfirmedUserData(*this, aFcb, input);
	}
}

void LinkLayer::UnconfirmedUserData(bool isMaster, uint16_t dest, uint16_t src, const RSlice& input)
{
	if (this->Validate(isMaster, src, dest))
	{
		this->DoDataUp(input);
	}
}

}

