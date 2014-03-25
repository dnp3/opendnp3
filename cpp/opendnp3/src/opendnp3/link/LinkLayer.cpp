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

#include <openpal/LoggableMacros.h>

#include "opendnp3/DNPErrorCodes.h"
#include "opendnp3/link/ILinkRouter.h"
#include "opendnp3/link/LinkFrame.h"
#include "opendnp3/LogLevels.h"

#include "PriLinkLayerStates.h"
#include "SecLinkLayerStates.h"

using namespace openpal;


namespace opendnp3
{

LinkLayer::LinkLayer(const openpal::Logger& logger, openpal::IExecutor* apExecutor, const LinkConfig& config_) :
	Loggable(logger),	
	config(config_),
	pConfirmedSegments(nullptr),
	mRetryRemaining(0),
	mpExecutor(apExecutor),
	mpTimer(nullptr),
	mNextReadFCB(false),
	mNextWriteFCB(false),
	mIsOnline(false),
	mpRouter(nullptr),
	mpPriState(PLLS_SecNotReset::Inst()),
	mpSecState(SLLS_NotReset::Inst())
{

}

void LinkLayer::SetRouter(ILinkRouter* apRouter)
{
	assert(mpRouter == nullptr); assert(apRouter != nullptr);
	mpRouter = apRouter;
}

void LinkLayer::ChangeState(PriStateBase* apState)
{
	mpPriState = apState;
}

void LinkLayer::ChangeState(SecStateBase* apState)
{
	mpSecState = apState;
}

bool LinkLayer::Validate(bool aIsMaster, uint16_t aSrc, uint16_t aDest)
{
	if (mIsOnline)
	{
		if (aIsMaster == config.IsMaster)
		{
			ERROR_BLOCK(flags::WARN,
				(aIsMaster ? "Master frame received for master" : "Slave frame received for slave"),
				DLERR_MASTER_BIT_MATCH);
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
					ERROR_BLOCK(flags::WARN, "Frame from unknwon source", DLERR_UNKNOWN_SOURCE);
					return false;
				}				
			}
			else
			{
				ERROR_BLOCK(flags::WARN, "Frame for unknown destintation", DLERR_UNKNOWN_DESTINATION);
				return false;			
			}			
		}		
	}
	else
	{
		LOG_BLOCK(flags::ERR, "Layer is not online");
		return false;
	}	
}

////////////////////////////////
// ILowerLayer
////////////////////////////////

void LinkLayer::Send(IBufferSegment& segments)
{
	if (mIsOnline)
	{
		if (config.UseConfirms)
		{
			mpPriState->SendConfirmed(this, segments);
		}
		else
		{
			mpPriState->SendUnconfirmed(this, segments);
		}
	}
	else
	{
		LOG_BLOCK(flags::ERR, "Layer is not online");
	}
}

////////////////////////////////
// ILinkContext
////////////////////////////////

void LinkLayer::OnLowerLayerUp()
{
	if (mIsOnline)
	{
		LOG_BLOCK(flags::ERR, "Layer already online");
	}
	else
	{
		mIsOnline = true;

		if (pUpperLayer)
		{
			pUpperLayer->OnLowerLayerUp();
		}
	}
}

void LinkLayer::OnLowerLayerDown()
{
	if (mIsOnline)
	{
		mIsOnline = false;

		if (mpTimer)
		{
			this->CancelTimer();
		}
		
		mpPriState = PLLS_SecNotReset::Inst();
		mpSecState = SLLS_NotReset::Inst();

		if (pUpperLayer)
		{
			pUpperLayer->OnLowerLayerDown();
		}
	}
	else
	{
		LOG_BLOCK(flags::ERR, "Layer is not online");
	}
}

void LinkLayer::OnTransmitResult(bool primary, bool success)
{
	if (primary)
	{
		mpPriState->OnTransmitResult(this, success);
	}
	else
	{
		mpSecState->OnTransmitResult(this, success);
	}
}

openpal::ReadOnlyBuffer LinkLayer::FormatPrimaryBufferWithConfirmed(IBufferSegment& segments, bool FCB)
{
	auto buffer = primaryBuffer.GetWriteBuffer();
	if (segments.HasValue())
	{
		auto seg = segments.GetSegment();
		return LinkFrame::FormatConfirmedUserData(buffer, config.IsMaster, FCB, config.RemoteAddr, config.LocalAddr, seg, seg.Size());
	}
	else
	{
		return ReadOnlyBuffer();
	}
}

ReadOnlyBuffer LinkLayer::FormatPrimaryBufferWithUnconfirmed(IBufferSegment& segments)
{
	auto buffer = primaryBuffer.GetWriteBuffer();

	uint32_t size = 0;

	while (segments.HasValue())
	{
		auto seg = segments.GetSegment();
		segments.Advance();
		if(buffer.Size() >= LinkFrame::CalcFrameSize(seg.Size())) 
		{			
			size += LinkFrame::FormatUnconfirmedUserData(buffer, config.IsMaster, config.RemoteAddr, config.LocalAddr, seg, seg.Size()).Size();
		}
		else
		{
			return ReadOnlyBuffer(); //can't write the full amount
		}
	}

	return primaryBuffer.ToReadOnly().Truncate(size);
}

void LinkLayer::QueueTransmit(const ReadOnlyBuffer& buffer, bool primary)
{
	mpRouter->QueueTransmit(buffer, this, primary);
}

void LinkLayer::QueueAck()
{
	auto buffer = LinkFrame::FormatAck(primaryBuffer.GetWriteBuffer(), config.IsMaster, false, config.RemoteAddr, config.LocalAddr);
	this->QueueTransmit(buffer, false);
}

void LinkLayer::QueueLinkStatus()
{
	
	auto buffer = LinkFrame::FormatLinkStatus(primaryBuffer.GetWriteBuffer(), config.IsMaster, false, config.RemoteAddr, config.LocalAddr);
	this->QueueTransmit(buffer, false);	
}

void LinkLayer::QueueResetLinks()
{	
	auto buffer = LinkFrame::FormatResetLinkStates(primaryBuffer.GetWriteBuffer(), config.IsMaster, config.RemoteAddr, config.LocalAddr);
	this->QueueTransmit(buffer, true);	
}

void LinkLayer::StartTimer()
{
	assert(mpTimer == nullptr);
	mpTimer = this->mpExecutor->Start(TimeDuration(config.Timeout), std::bind(&LinkLayer::OnTimeout, this));
}

void LinkLayer::CancelTimer()
{
	assert(mpTimer);
	mpTimer->Cancel();
	mpTimer = nullptr;
}

void LinkLayer::ResetRetry()
{
	this->mRetryRemaining = config.NumRetry;
}

bool LinkLayer::Retry()
{
	if(mRetryRemaining > 0)
	{
		--mRetryRemaining;
		return true;
	}
	else return false;
}

////////////////////////////////
// IFrameSink
////////////////////////////////

void LinkLayer::Ack(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc)
{
	if (this->Validate(aIsMaster, aSrc, aDest))
	{
		mpPriState->Ack(this, aIsRcvBuffFull);
	}
}

void LinkLayer::Nack(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc)
{
	if (this->Validate(aIsMaster, aSrc, aDest))
	{
		mpPriState->Nack(this, aIsRcvBuffFull);
	}
}

void LinkLayer::LinkStatus(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc)
{
	if (this->Validate(aIsMaster, aSrc, aDest))
	{
		mpPriState->LinkStatus(this, aIsRcvBuffFull);
	}
}

void LinkLayer::NotSupported (bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc)
{
	if (this->Validate(aIsMaster, aSrc, aDest))
	{
		mpPriState->NotSupported(this, aIsRcvBuffFull);
	}
}

void LinkLayer::TestLinkStatus(bool aIsMaster, bool aFcb, uint16_t aDest, uint16_t aSrc)
{
	if (this->Validate(aIsMaster, aSrc, aDest))
	{
		mpSecState->TestLinkStatus(this, aFcb);
	}
}

void LinkLayer::ResetLinkStates(bool aIsMaster, uint16_t aDest, uint16_t aSrc)
{
	if (this->Validate(aIsMaster, aSrc, aDest))
	{
		mpSecState->ResetLinkStates(this);
	}
}

void LinkLayer::RequestLinkStatus(bool aIsMaster, uint16_t aDest, uint16_t aSrc)
{
	if (this->Validate(aIsMaster, aSrc, aDest))
	{
		mpSecState->RequestLinkStatus(this);
	}
}

void LinkLayer::ConfirmedUserData(bool aIsMaster, bool aFcb, uint16_t aDest, uint16_t aSrc, const ReadOnlyBuffer& input)
{
	if (this->Validate(aIsMaster, aSrc, aDest))
	{
		mpSecState->ConfirmedUserData(this, aFcb, input);
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
	assert(mpTimer);
	mpTimer = nullptr;
	mpPriState->OnTimeout(this);
}

}

