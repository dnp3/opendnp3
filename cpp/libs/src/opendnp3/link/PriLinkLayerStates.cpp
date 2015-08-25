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
#include "PriLinkLayerStates.h"


#include <openpal/logging/LogMacros.h>

#include "opendnp3/ErrorCodes.h"
#include "opendnp3/link/LinkLayer.h"
#include "opendnp3/LogLevels.h"

using namespace openpal;

namespace opendnp3
{

////////////////////////////////////////
// PriStateBase
////////////////////////////////////////

void PriStateBase::Ack(LinkLayer& link, bool receiveBuffFull)
{
	SIMPLE_LOG_BLOCK_WITH_CODE(link.GetLogger(), flags::WARN, DLERR_UNEXPECTED_LPDU, "Frame context not understood");
}

void PriStateBase::Nack(LinkLayer& link, bool receiveBuffFull)
{
	SIMPLE_LOG_BLOCK_WITH_CODE(link.GetLogger(), flags::WARN, DLERR_UNEXPECTED_LPDU, "Frame context not understood");
}

void PriStateBase::LinkStatus(LinkLayer& link, bool receiveBuffFull)
{
	SIMPLE_LOG_BLOCK_WITH_CODE(link.GetLogger(), flags::WARN, DLERR_UNEXPECTED_LPDU, "Frame context not understood");
}

void PriStateBase::NotSupported (LinkLayer& link, bool receiveBuffFull)
{
	SIMPLE_LOG_BLOCK_WITH_CODE(link.GetLogger(), flags::WARN, DLERR_UNEXPECTED_LPDU, "Frame context not understood");
}

void PriStateBase::OnTransmitResult(LinkLayer& link, bool success)
{
	FORMAT_LOG_BLOCK(link.GetLogger(), flags::ERR, "Invalid action for state: %s", this->Name());	
}

void PriStateBase::OnTimeout(LinkLayer& link)
{
	FORMAT_LOG_BLOCK(link.GetLogger(), flags::ERR, "Invalid action for state: %s", this->Name());
}

void PriStateBase::SendConfirmed(LinkLayer& link, ITransportSegment&)
{
	FORMAT_LOG_BLOCK(link.GetLogger(), flags::ERR, "Invalid action for state: %s", this->Name());
}

void PriStateBase::SendUnconfirmed(LinkLayer& link, ITransportSegment&)
{
	FORMAT_LOG_BLOCK(link.GetLogger(), flags::ERR, "Invalid action for state: %s", this->Name());
}

////////////////////////////////////////////////////////
//	Class PLLS_SecNotResetIdle
////////////////////////////////////////////////////////

PLLS_SecNotReset PLLS_SecNotReset::instance;

void PLLS_SecNotReset::SendUnconfirmed(LinkLayer& link, ITransportSegment& segments)
{
	link.pSegments = &segments;
	auto first = segments.GetSegment();
	auto output = link.FormatPrimaryBufferWithUnconfirmed(first);	
	link.QueueTransmit(output, true);
	link.ChangeState(PLLS_SendUnconfirmedTransmitWait<PLLS_SecNotReset>::Inst());	
}

void PLLS_SecNotReset::SendConfirmed(LinkLayer& link, ITransportSegment& segments)
{
	link.ResetRetry();
	link.pSegments = &segments;
	link.QueueResetLinks();
	link.ChangeState(PLLS_LinkResetTransmitWait::Inst());
}

/////////////////////////////////////////////////////////////////////////////
//  Wait for the link layer to transmit the reset links
/////////////////////////////////////////////////////////////////////////////

PLLS_LinkResetTransmitWait PLLS_LinkResetTransmitWait::instance;

void PLLS_LinkResetTransmitWait::OnTransmitResult(LinkLayer& link, bool success)
{
	if (success)
	{
		// now we're waiting for an ACK
		link.StartTimer();
		link.ChangeState(PLLS_ResetLinkWait::Inst());
	}
	else
	{
		link.ChangeState(PLLS_SecNotReset::Inst());
		link.DoSendResult(success);
	}
}

/////////////////////////////////////////////////////////////////////////////
//  Wait for the link layer to transmit confirmed user data
/////////////////////////////////////////////////////////////////////////////

PLLS_ConfUserDataTransmitWait PLLS_ConfUserDataTransmitWait::instance;

void PLLS_ConfUserDataTransmitWait::OnTransmitResult(LinkLayer& link, bool success)
{
	if (success)
	{
		// now we're waiting on an ACK
		link.StartTimer();
		link.ChangeState(PLLS_ConfDataWait::Inst());
	}
	else
	{
		link.ChangeState(PLLS_SecReset::Inst());
		link.DoSendResult(false);
	}
}


////////////////////////////////////////////////////////
//	Class PLLS_SecReset
////////////////////////////////////////////////////////

PLLS_SecReset PLLS_SecReset::instance;

void PLLS_SecReset::SendUnconfirmed(LinkLayer& link, ITransportSegment& segments)
{
	link.pSegments = &segments;
	auto output = link.FormatPrimaryBufferWithUnconfirmed(segments.GetSegment());	
	link.QueueTransmit(output, true);
	link.ChangeState(PLLS_SendUnconfirmedTransmitWait<PLLS_SecReset>::Inst());	
}

void PLLS_SecReset::SendConfirmed(LinkLayer& link, ITransportSegment& segments)
{
	link.ResetRetry();
	link.pSegments = &segments;
	auto buffer = link.FormatPrimaryBufferWithConfirmed(segments.GetSegment(), link.NextWriteFCB());
	link.QueueTransmit(buffer, true);
	link.ChangeState(PLLS_ConfUserDataTransmitWait::Inst());

}

////////////////////////////////////////////////////////
//	Class PLLS_ResetLinkWait
////////////////////////////////////////////////////////

PLLS_ResetLinkWait PLLS_ResetLinkWait::instance;

void PLLS_ResetLinkWait::Ack(LinkLayer& link, bool receiveBuffFull)
{
	link.ResetWriteFCB();
	link.CancelTimer();
	auto buffer = link.FormatPrimaryBufferWithConfirmed(link.pSegments->GetSegment(), link.NextWriteFCB());
	link.QueueTransmit(buffer, true);
	link.ChangeState(PLLS_ConfUserDataTransmitWait::Inst());
	link.CallStatusCallback(opendnp3::LinkStatus::RESET);
}

void PLLS_ResetLinkWait::OnTimeout(LinkLayer& link)
{
	if(link.Retry())
	{
		FORMAT_LOG_BLOCK(link.GetLogger(), flags::WARN, "Link reset timeout, retrying %i remaining", link.RetryRemaining());
		link.QueueResetLinks();
		link.ChangeState(PLLS_LinkResetTransmitWait::Inst());
	}
	else
	{
		SIMPLE_LOG_BLOCK(link.GetLogger(), flags::WARN, "Link reset final timeout, no retries remain");
		link.ChangeState(PLLS_SecNotReset::Inst());
		link.DoSendResult(false);
	}
}

void PLLS_ResetLinkWait::Failure(LinkLayer& link)
{
	link.CancelTimer();
	link.ChangeState(PLLS_SecNotReset::Inst());
	link.DoSendResult(false);
}

////////////////////////////////////////////////////////
//	Class PLLS_ConfDataWait
////////////////////////////////////////////////////////

PLLS_ConfDataWait PLLS_ConfDataWait::instance;

void PLLS_ConfDataWait::Ack(LinkLayer& link, bool receiveBuffFull)
{
	link.ToggleWriteFCB();
	link.CancelTimer();

	if (link.pSegments->Advance())
	{
		auto buffer = link.FormatPrimaryBufferWithConfirmed(link.pSegments->GetSegment(), link.NextWriteFCB());
		link.QueueTransmit(buffer, true);
		link.ChangeState(PLLS_ConfUserDataTransmitWait::Inst());
	}
	else //we're done!
	{
		link.ChangeState(PLLS_SecReset::Inst());
		link.DoSendResult(true);
	}
}

void PLLS_ConfDataWait::Nack(LinkLayer& link, bool receiveBuffFull)
{
	if (receiveBuffFull)
	{
		Failure(link);
	}
	else
	{
		link.ResetRetry();
		link.CancelTimer();
		link.ChangeState(PLLS_LinkResetTransmitWait::Inst());
		link.QueueResetLinks();
	}
	link.CallStatusCallback(opendnp3::LinkStatus::UNRESET);
}

void PLLS_ConfDataWait::Failure(LinkLayer& link)
{
	link.CancelTimer();
	link.ChangeState(PLLS_SecNotReset::Inst());
	link.DoSendResult(false);
}

void PLLS_ConfDataWait::OnTimeout(LinkLayer& link)
{
	if(link.Retry())
	{
		FORMAT_LOG_BLOCK(link.GetLogger(), flags::WARN, "confirmed data timeout, retrying %u remaining", link.RetryRemaining());
		auto buffer = link.FormatPrimaryBufferWithConfirmed(link.pSegments->GetSegment(), link.NextWriteFCB());
		link.QueueTransmit(buffer, true);
		link.ChangeState(PLLS_ConfUserDataTransmitWait::Inst());
	}
	else
	{
		SIMPLE_LOG_BLOCK(link.GetLogger(), flags::WARN, "Confirmed data final timeout, no retries remain");
		link.ChangeState(PLLS_SecNotReset::Inst());
		link.DoSendResult(false);
		link.CallStatusCallback(opendnp3::LinkStatus::UNRESET);
	}
}

} //end namepsace

