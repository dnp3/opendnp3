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

PriStateBase& PriStateBase::OnAck(LinkLayer& link, bool receiveBuffFull)
{
	SIMPLE_LOG_BLOCK_WITH_CODE(link.GetLogger(), flags::WARN, DLERR_UNEXPECTED_LPDU, "Frame context not understood");
	return *this;
}

PriStateBase& PriStateBase::OnNack(LinkLayer& link, bool receiveBuffFull)
{
	SIMPLE_LOG_BLOCK_WITH_CODE(link.GetLogger(), flags::WARN, DLERR_UNEXPECTED_LPDU, "Frame context not understood");
	return *this;
}

PriStateBase& PriStateBase::OnLinkStatus(LinkLayer& link, bool receiveBuffFull)
{
	SIMPLE_LOG_BLOCK_WITH_CODE(link.GetLogger(), flags::WARN, DLERR_UNEXPECTED_LPDU, "Frame context not understood");
	return *this;
}

PriStateBase& PriStateBase::OnNotSupported(LinkLayer& link, bool receiveBuffFull)
{
	SIMPLE_LOG_BLOCK_WITH_CODE(link.GetLogger(), flags::WARN, DLERR_UNEXPECTED_LPDU, "Frame context not understood");
	return *this;
}

PriStateBase& PriStateBase::OnTransmitResult(LinkLayer& link, bool success)
{
	FORMAT_LOG_BLOCK(link.GetLogger(), flags::ERR, "Invalid action for state: %s", this->Name());	
	return *this;
}

PriStateBase& PriStateBase::OnTimeout(LinkLayer& link)
{
	FORMAT_LOG_BLOCK(link.GetLogger(), flags::ERR, "Invalid action for state: %s", this->Name());
	return *this;
}

PriStateBase& PriStateBase::TrySendConfirmed(LinkLayer& link, ITransportSegment&)
{	
	return *this;
}

PriStateBase& PriStateBase::TrySendUnconfirmed(LinkLayer& link, ITransportSegment&)
{	
	return *this;
}

////////////////////////////////////////////////////////
//	Class PLLS_SecNotResetIdle
////////////////////////////////////////////////////////

PLLS_SecNotReset PLLS_SecNotReset::instance;

PriStateBase& PLLS_SecNotReset::TrySendUnconfirmed(LinkLayer& link, ITransportSegment& segments)
{	
	auto first = segments.GetSegment();
	auto output = link.FormatPrimaryBufferWithUnconfirmed(first);	
	link.QueueTransmit(output, true);
	return PLLS_SendUnconfirmedTransmitWait<PLLS_SecNotReset>::Instance();	
}

PriStateBase& PLLS_SecNotReset::TrySendConfirmed(LinkLayer& link, ITransportSegment& segments)
{
	link.ResetRetry();
	link.QueueResetLinks();
	return PLLS_LinkResetTransmitWait::Instance();
}

/////////////////////////////////////////////////////////////////////////////
//  Wait for the link layer to transmit the reset links
/////////////////////////////////////////////////////////////////////////////

PLLS_LinkResetTransmitWait PLLS_LinkResetTransmitWait::instance;

PriStateBase& PLLS_LinkResetTransmitWait::OnTransmitResult(LinkLayer& link, bool success)
{
	if (success)
	{
		// now we're waiting for an ACK
		link.StartTimer();
		return PLLS_ResetLinkWait::Instance();
	}
	else
	{		
		link.CompleteSendOperation(success);
		return PLLS_SecNotReset::Instance();
	}
}

/////////////////////////////////////////////////////////////////////////////
//  Wait for the link layer to transmit confirmed user data
/////////////////////////////////////////////////////////////////////////////

PLLS_ConfUserDataTransmitWait PLLS_ConfUserDataTransmitWait::instance;

PriStateBase& PLLS_ConfUserDataTransmitWait::OnTransmitResult(LinkLayer& link, bool success)
{
	if (success)
	{
		// now we're waiting on an ACK
		link.StartTimer();
		return PLLS_ConfDataWait::Instance();
	}
	else
	{		
		link.CompleteSendOperation(false);
		return PLLS_SecReset::Instance();
	}
}


////////////////////////////////////////////////////////
//	Class PLLS_SecReset
////////////////////////////////////////////////////////

PLLS_SecReset PLLS_SecReset::instance;

PriStateBase& PLLS_SecReset::TrySendUnconfirmed(LinkLayer& link, ITransportSegment& segments)
{		
	auto first = segments.GetSegment();
	auto output = link.FormatPrimaryBufferWithUnconfirmed(first);	
	link.QueueTransmit(output, true);
	return PLLS_SendUnconfirmedTransmitWait<PLLS_SecReset>::Instance();	
}

PriStateBase& PLLS_SecReset::TrySendConfirmed(LinkLayer& link, ITransportSegment& segments)
{
	link.ResetRetry();
	auto buffer = link.FormatPrimaryBufferWithConfirmed(segments.GetSegment(), link.NextWriteFCB());
	link.QueueTransmit(buffer, true);
	return PLLS_ConfUserDataTransmitWait::Instance();
}

////////////////////////////////////////////////////////
//	Class PLLS_ResetLinkWait
////////////////////////////////////////////////////////

PLLS_ResetLinkWait PLLS_ResetLinkWait::instance;

PriStateBase& PLLS_ResetLinkWait::OnAck(LinkLayer& link, bool receiveBuffFull)
{
	link.ResetWriteFCB();
	link.CancelTimer();
	auto buffer = link.FormatPrimaryBufferWithConfirmed(link.pSegments->GetSegment(), link.NextWriteFCB());
	link.QueueTransmit(buffer, true);	
	link.PostStatusCallback(opendnp3::LinkStatus::RESET);
	return PLLS_ConfUserDataTransmitWait::Instance();
}

PriStateBase& PLLS_ResetLinkWait::OnTimeout(LinkLayer& link)
{
	if(link.Retry())
	{
		FORMAT_LOG_BLOCK(link.GetLogger(), flags::WARN, "Link reset timeout, retrying %i remaining", link.RetryRemaining());
		link.QueueResetLinks();
		return PLLS_LinkResetTransmitWait::Instance();
	}
	else
	{
		SIMPLE_LOG_BLOCK(link.GetLogger(), flags::WARN, "Link reset final timeout, no retries remain");		
		link.CompleteSendOperation(false);
		return PLLS_SecNotReset::Instance();
	}
}

PriStateBase& PLLS_ResetLinkWait::Failure(LinkLayer& link)
{
	link.CancelTimer();	
	link.CompleteSendOperation(false);
	return PLLS_SecNotReset::Instance();
}

////////////////////////////////////////////////////////
//	Class PLLS_ConfDataWait
////////////////////////////////////////////////////////

PLLS_ConfDataWait PLLS_ConfDataWait::instance;

PriStateBase& PLLS_ConfDataWait::OnAck(LinkLayer& link, bool receiveBuffFull)
{
	link.ToggleWriteFCB();
	link.CancelTimer();

	if (link.pSegments->Advance())
	{
		auto buffer = link.FormatPrimaryBufferWithConfirmed(link.pSegments->GetSegment(), link.NextWriteFCB());
		link.QueueTransmit(buffer, true);
		return PLLS_ConfUserDataTransmitWait::Instance();
	}
	else //we're done!
	{		
		link.CompleteSendOperation(true);
		return PLLS_SecReset::Instance();
	}
}

PriStateBase& PLLS_ConfDataWait::OnNack(LinkLayer& link, bool receiveBuffFull)
{
	link.PostStatusCallback(opendnp3::LinkStatus::UNRESET);

	if (receiveBuffFull)
	{
		return Failure(link);
	}
	else
	{
		link.ResetRetry();
		link.CancelTimer();		
		link.QueueResetLinks();
		return PLLS_LinkResetTransmitWait::Instance();
	}
	
}

PriStateBase& PLLS_ConfDataWait::Failure(LinkLayer& link)
{
	link.CancelTimer();	
	link.CompleteSendOperation(false);
	return PLLS_SecNotReset::Instance();
}

PriStateBase& PLLS_ConfDataWait::OnTimeout(LinkLayer& link)
{
	if(link.Retry())
	{
		FORMAT_LOG_BLOCK(link.GetLogger(), flags::WARN, "confirmed data timeout, retrying %u remaining", link.RetryRemaining());
		auto buffer = link.FormatPrimaryBufferWithConfirmed(link.pSegments->GetSegment(), link.NextWriteFCB());
		link.QueueTransmit(buffer, true);
		return PLLS_ConfUserDataTransmitWait::Instance();		
	}
	else
	{
		SIMPLE_LOG_BLOCK(link.GetLogger(), flags::WARN, "Confirmed data final timeout, no retries remain");
		link.PostStatusCallback(opendnp3::LinkStatus::UNRESET);
		link.CompleteSendOperation(false);
		return PLLS_SecNotReset::Instance();
	}
}

} //end namepsace

