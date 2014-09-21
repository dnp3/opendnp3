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

LinkLayer::LinkLayer(openpal::LogRoot& root, openpal::IExecutor* pExecutor_, const LinkConfig& config_) :
	logger(root.GetLogger()),
	config(config_),
	pSegments(nullptr),
	txMode(TransmitMode::Idle),
	numRetryRemaining(0),
	pExecutor(pExecutor_),
	pTimer(nullptr),
	nextReadFCB(false),
	nextWriteFCB(false),
	isOnline(false),
	pRouter(nullptr),
	pPriState(PLLS_SecNotReset::Inst()),
	pSecState(SLLS_NotReset::Inst())
{

}

void LinkLayer::SetRouter(ILinkRouter& router)
{
	assert(pRouter == nullptr);
	pRouter = &router;
}

void LinkLayer::ChangeState(PriStateBase* pState)
{
	pPriState = pState;
}

void LinkLayer::ChangeState(SecStateBase* pState)
{
	pSecState = pState;
}

void LinkLayer::PostSendResult(bool isSuccess)
{
	auto lambda = [this, isSuccess]() { this->DoSendResult(isSuccess); };
	pExecutor->PostLambda(lambda);
}

bool LinkLayer::Validate(bool aIsMaster, uint16_t aSrc, uint16_t aDest)
{
	if (isOnline)
	{
		if (aIsMaster == config.IsMaster)
		{			
			SIMPLE_LOG_BLOCK_WITH_CODE(logger, flags::WARN, DLERR_WRONG_MASTER_BIT,
				(aIsMaster ? "Master frame received for master" : "Outstation frame received for outstation"));			            
			
			return false;
		}
		else
		{
			if (aDest == config.LocalAddr)
			{
				if (aSrc == config.RemoteAddr)
				{
					return true;
				}
				else
				{
					SIMPLE_LOG_BLOCK_WITH_CODE(logger, flags::WARN, DLERR_UNKNOWN_SOURCE, "Frame from unknwon source");
					return false;
				}
			}
			else
			{
				SIMPLE_LOG_BLOCK_WITH_CODE(logger, flags::WARN, DLERR_UNKNOWN_DESTINATION, "Frame for unknown destintation");
				return false;
			}
		}
	}
	else
	{
		SIMPLE_LOG_BLOCK(logger, flags::ERR, "Layer is not online");
		return false;
	}
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
			pPriState->SendConfirmed(this, segments);
		}
		else
		{
			pPriState->SendUnconfirmed(this, segments);
		}
	}
	else
	{
		SIMPLE_LOG_BLOCK(logger, flags::ERR, "Layer is not online");
	}
}

////////////////////////////////
// ILinkContext
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

		if (pTimer)
		{
			this->CancelTimer();
		}

		pPriState = PLLS_SecNotReset::Inst();
		pSecState = SLLS_NotReset::Inst();

		if (pUpperLayer)
		{
			pUpperLayer->OnLowerLayerDown();
		}
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
			pPriState->OnTransmitResult(this, success);
		}
		else
		{
			pSecState->OnTransmitResult(this, success);
		}		
	}			
}

void LinkLayer::CheckPendingTx(openpal::Settable<ReadOnlyBuffer>& pending, bool primary)
{
	if (txMode == TransmitMode::Idle && pending.IsSet())
	{
		pRouter->BeginTransmit(pending.Get(), this);
		pending.Clear();
		this->txMode = primary ? TransmitMode::Primary : TransmitMode::Secondary;
	}
}

openpal::ReadOnlyBuffer LinkLayer::FormatPrimaryBufferWithConfirmed(const openpal::ReadOnlyBuffer& tpdu, bool FCB)
{
	auto buffer = WriteBuffer(priTxBuffer, LPDU_MAX_FRAME_SIZE);
	auto output = LinkFrame::FormatConfirmedUserData(buffer, config.IsMaster, FCB, config.RemoteAddr, config.LocalAddr, tpdu, tpdu.Size(), &logger);
	FORMAT_HEX_BLOCK(logger, flags::LINK_TX_HEX, output, 10, 18);
	return output;	
}

ReadOnlyBuffer LinkLayer::FormatPrimaryBufferWithUnconfirmed(const openpal::ReadOnlyBuffer& tpdu)
{
	auto buffer = WriteBuffer(priTxBuffer, LPDU_MAX_FRAME_SIZE);
	auto output = LinkFrame::FormatUnconfirmedUserData(buffer, config.IsMaster, config.RemoteAddr, config.LocalAddr, tpdu, tpdu.Size(), &logger);
	FORMAT_HEX_BLOCK(logger, flags::LINK_TX_HEX, output, 10, 18);
	return output;
}

void LinkLayer::QueueTransmit(const ReadOnlyBuffer& buffer, bool primary)
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
	auto writeTo = WriteBuffer(secTxBuffer, LPDU_HEADER_SIZE);
	auto buffer = LinkFrame::FormatAck(writeTo, config.IsMaster, false, config.RemoteAddr, config.LocalAddr, &logger);
	FORMAT_HEX_BLOCK(logger, flags::LINK_TX_HEX, buffer, 10, 18);
	this->QueueTransmit(buffer, false);
}

void LinkLayer::QueueLinkStatus()
{
	auto writeTo = WriteBuffer(secTxBuffer, LPDU_HEADER_SIZE);
	auto buffer = LinkFrame::FormatLinkStatus(writeTo, config.IsMaster, false, config.RemoteAddr, config.LocalAddr, &logger);
	FORMAT_HEX_BLOCK(logger, flags::LINK_TX_HEX, buffer, 10, 18);
	this->QueueTransmit(buffer, false);
}

void LinkLayer::QueueResetLinks()
{
	auto writeTo = WriteBuffer(priTxBuffer, LPDU_MAX_FRAME_SIZE);
	auto buffer = LinkFrame::FormatResetLinkStates(writeTo, config.IsMaster, config.RemoteAddr, config.LocalAddr, &logger);
	FORMAT_HEX_BLOCK(logger, flags::LINK_TX_HEX, buffer, 10, 18);
	this->QueueTransmit(buffer, true);
}

void LinkLayer::StartTimer()
{
	assert(pTimer == nullptr);
	auto lambda = [this]() { this->OnTimeout(); };
	pTimer = this->pExecutor->Start(TimeDuration(config.Timeout), Action0::Bind(lambda));
}

void LinkLayer::CancelTimer()
{
	assert(pTimer);
	pTimer->Cancel();
	pTimer = nullptr;
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

void LinkLayer::Ack(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc)
{
	if (this->Validate(aIsMaster, aSrc, aDest))
	{
		pPriState->Ack(this, aIsRcvBuffFull);
	}
}

void LinkLayer::Nack(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc)
{
	if (this->Validate(aIsMaster, aSrc, aDest))
	{
		pPriState->Nack(this, aIsRcvBuffFull);
	}
}

void LinkLayer::LinkStatus(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc)
{
	if (this->Validate(aIsMaster, aSrc, aDest))
	{
		pPriState->LinkStatus(this, aIsRcvBuffFull);
	}
}

void LinkLayer::NotSupported (bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc)
{
	if (this->Validate(aIsMaster, aSrc, aDest))
	{
		pPriState->NotSupported(this, aIsRcvBuffFull);
	}
}

void LinkLayer::TestLinkStatus(bool aIsMaster, bool aFcb, uint16_t aDest, uint16_t aSrc)
{
	if (this->Validate(aIsMaster, aSrc, aDest))
	{
		pSecState->TestLinkStatus(this, aFcb);
	}
}

void LinkLayer::ResetLinkStates(bool aIsMaster, uint16_t aDest, uint16_t aSrc)
{
	if (this->Validate(aIsMaster, aSrc, aDest))
	{
		pSecState->ResetLinkStates(this);
	}
}

void LinkLayer::RequestLinkStatus(bool aIsMaster, uint16_t aDest, uint16_t aSrc)
{
	if (this->Validate(aIsMaster, aSrc, aDest))
	{
		pSecState->RequestLinkStatus(this);
	}
}

void LinkLayer::ConfirmedUserData(bool aIsMaster, bool aFcb, uint16_t aDest, uint16_t aSrc, const ReadOnlyBuffer& input)
{
	if (this->Validate(aIsMaster, aSrc, aDest))
	{
		pSecState->ConfirmedUserData(this, aFcb, input);
	}
}

void LinkLayer::UnconfirmedUserData(bool aIsMaster, uint16_t aDest, uint16_t aSrc, const ReadOnlyBuffer& input)
{
	if (this->Validate(aIsMaster, aSrc, aDest))
	{
		this->DoDataUp(input);
	}
}

void LinkLayer::OnTimeout()
{
	assert(pTimer);
	pTimer = nullptr;
	pPriState->OnTimeout(this);
}

}

