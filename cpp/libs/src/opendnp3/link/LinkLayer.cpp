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
	ctx(root, executor, listener, *this, config)
{}

void LinkLayer::SetUpperLayer(IUpperLayer& upperLayer)
{
	ctx.SetUpperLayer(upperLayer);
}

void LinkLayer::SetRouter(ILinkRouter& router)
{
	assert(ctx.pRouter == nullptr);
	ctx.pRouter = &router;
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
	this->ctx.TryStartTransmission();
}

////////////////////////////////
// ILinkSession
////////////////////////////////

bool LinkLayer::OnLowerLayerUp()
{
	return ctx.OnLowerLayerUp();	
}

bool LinkLayer::OnLowerLayerDown()
{
	return ctx.OnLowerLayerDown();	
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
	ctx.TryPendingTx(ctx.pendingSecTx, false);
	ctx.TryPendingTx(ctx.pendingPriTx, true);

	// now dispatch the completion event to the correct state handler
	if (isPrimary)
	{
		ctx.pPriState = &ctx.pPriState->OnTransmitResult(ctx, success);
	}
	else
	{
		ctx.pSecState = &ctx.pSecState->OnTransmitResult(ctx, success);
	}	

	this->ctx.TryStartTransmission();
	return true;
}

////////////////////////////////
// IFrameSink
////////////////////////////////

bool LinkLayer::OnFrame(LinkFunction func, bool isMaster, bool fcb, bool fcvdfc, uint16_t dest, uint16_t source, const openpal::RSlice& userdata)
{
	auto ret = this->ctx.OnFrame(func, isMaster, fcb, fcvdfc, dest, source, userdata);
	this->ctx.TryStartTransmission();
	return ret;
}

}

