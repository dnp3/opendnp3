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
#include "SecLinkLayerStates.h"


#include <openpal/logging/LogMacros.h>

#include "opendnp3/ErrorCodes.h"
#include "opendnp3/link/LinkLayer.h"

#include "opendnp3/LogLevels.h"

using namespace openpal;

namespace opendnp3
{

////////////////////////////////////////
// SecStateBase
////////////////////////////////////////

SecStateBase& SecStateBase::OnTransmitResult(LinkLayer& link, bool success)
{
	FORMAT_LOG_BLOCK(link.ctx.logger, flags::ERR, "Invalid event for state: %s", this->Name());	
	return *this;
}

////////////////////////////////////////////////////////
//	Class SLLS_NotReset
////////////////////////////////////////////////////////
SLLS_NotReset SLLS_NotReset::instance;

SecStateBase& SLLS_NotReset::OnTestLinkStatus(LinkLayer& link, bool aFcb)
{
	SIMPLE_LOG_BLOCK_WITH_CODE(link.ctx.logger, flags::WARN, DLERR_UNEXPECTED_LPDU, "TestLinkStatus ignored");
	return *this;
}

SecStateBase& SLLS_NotReset::OnConfirmedUserData(LinkLayer& link, bool aFcb, const openpal::RSlice&)
{
	SIMPLE_LOG_BLOCK_WITH_CODE(link.ctx.logger, flags::WARN, DLERR_UNEXPECTED_LPDU, "ConfirmedUserData ignored");
	return *this;
}

SecStateBase& SLLS_NotReset::OnResetLinkStates(LinkLayer& link)
{
	link.ctx.QueueAck(link);
	link.ctx.ResetReadFCB();
	return SLLS_TransmitWaitReset::Instance();
}

SecStateBase& SLLS_NotReset::OnRequestLinkStatus(LinkLayer& link)
{
	link.ctx.QueueLinkStatus(link);
	return SLLS_TransmitWaitNotReset::Instance();
}

////////////////////////////////////////////////////////
//	Class SLLS_Reset
////////////////////////////////////////////////////////
SLLS_Reset SLLS_Reset::instance;

SecStateBase& SLLS_Reset::OnTestLinkStatus(LinkLayer& link, bool fcb)
{
	if(link.ctx.nextReadFCB == fcb)
	{
		link.ctx.QueueAck(link);
		link.ctx.ToggleReadFCB();
		return SLLS_TransmitWaitReset::Instance();
	}
	else
	{
		// "Re-transmit most recent response that contained function code 0 (ACK) or 1 (NACK)."
		// This is a PITA implement
		// TODO - see if this function is deprecated or not
		SIMPLE_LOG_BLOCK(link.ctx.logger, flags::WARN, "Received TestLinkStatus with invalid FCB");
		return *this;
	}
}

SecStateBase& SLLS_Reset::OnConfirmedUserData(LinkLayer& link, bool fcb, const openpal::RSlice& data)
{
	link.ctx.QueueAck(link);

	if (link.ctx.nextReadFCB == fcb)
	{
		link.ctx.ToggleReadFCB();
		link.ctx.PushDataUp(data);
	}
	else
	{
		SIMPLE_LOG_BLOCK(link.ctx.logger, flags::WARN, "Confirmed data w/ wrong FCB");
	}

	return SLLS_TransmitWaitReset::Instance();
}

SecStateBase& SLLS_Reset::OnResetLinkStates(LinkLayer& link)
{
	link.ctx.QueueAck(link);
	link.ctx.ResetReadFCB();
	return SLLS_TransmitWaitReset::Instance();
}

SecStateBase& SLLS_Reset::OnRequestLinkStatus(LinkLayer& link)
{
	link.ctx.QueueLinkStatus(link);
	return SLLS_TransmitWaitReset::Instance();
}

////////////////////////////////////////////////////////
//	Class SLLS_TransmitWaitReset
////////////////////////////////////////////////////////
SLLS_TransmitWaitReset SLLS_TransmitWaitReset::instance;

////////////////////////////////////////////////////////
//	Class SLLS_TransmitWaitNotReset
////////////////////////////////////////////////////////
SLLS_TransmitWaitNotReset SLLS_TransmitWaitNotReset::instance;

} //end namepsace

