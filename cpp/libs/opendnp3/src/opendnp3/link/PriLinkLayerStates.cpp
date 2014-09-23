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

void PriStateBase::Ack(LinkLayer* pLinkLayer, bool receiveBuffFull)
{
	SIMPLE_LOG_BLOCK_WITH_CODE(pLinkLayer->GetLogger(), flags::WARN, DLERR_UNEXPECTED_LPDU, "Frame context not understood");
}

void PriStateBase::Nack(LinkLayer* pLinkLayer, bool receiveBuffFull)
{
	SIMPLE_LOG_BLOCK_WITH_CODE(pLinkLayer->GetLogger(), flags::WARN, DLERR_UNEXPECTED_LPDU, "Frame context not understood");
}

void PriStateBase::LinkStatus(LinkLayer* pLinkLayer, bool receiveBuffFull)
{
	SIMPLE_LOG_BLOCK_WITH_CODE(pLinkLayer->GetLogger(), flags::WARN, DLERR_UNEXPECTED_LPDU, "Frame context not understood");
}

void PriStateBase::NotSupported (LinkLayer* pLinkLayer, bool receiveBuffFull)
{
	SIMPLE_LOG_BLOCK_WITH_CODE(pLinkLayer->GetLogger(), flags::WARN, DLERR_UNEXPECTED_LPDU, "Frame context not understood");
}

void PriStateBase::OnTransmitResult(LinkLayer* pLinkLayer, bool success)
{
	FORMAT_LOG_BLOCK(pLinkLayer->GetLogger(), flags::ERR, "Invalid action for state: %s", this->Name());	
}

void PriStateBase::OnTimeout(LinkLayer* pLinkLayer)
{
	FORMAT_LOG_BLOCK(pLinkLayer->GetLogger(), flags::ERR, "Invalid action for state: %s", this->Name());
}

void PriStateBase::SendConfirmed(LinkLayer* pLinkLayer, ITransportSegment&)
{
	FORMAT_LOG_BLOCK(pLinkLayer->GetLogger(), flags::ERR, "Invalid action for state: %s", this->Name());
}

void PriStateBase::SendUnconfirmed(LinkLayer* pLinkLayer, ITransportSegment&)
{
	FORMAT_LOG_BLOCK(pLinkLayer->GetLogger(), flags::ERR, "Invalid action for state: %s", this->Name());
}

////////////////////////////////////////////////////////
//	Class PLLS_SecNotResetIdle
////////////////////////////////////////////////////////

PLLS_SecNotReset PLLS_SecNotReset::instance;

void PLLS_SecNotReset::SendUnconfirmed(LinkLayer* pLinkLayer, ITransportSegment& segments)
{
	pLinkLayer->pSegments = &segments;
	auto first = segments.GetSegment();
	auto output = pLinkLayer->FormatPrimaryBufferWithUnconfirmed(first);	
	pLinkLayer->QueueTransmit(output, true);
	pLinkLayer->ChangeState(PLLS_SendUnconfirmedTransmitWait<PLLS_SecNotReset>::Inst());	
}

void PLLS_SecNotReset::SendConfirmed(LinkLayer* pLinkLayer, ITransportSegment& segments)
{
	pLinkLayer->ResetRetry();
	pLinkLayer->pSegments = &segments;
	pLinkLayer->QueueResetLinks();
	pLinkLayer->ChangeState(PLLS_LinkResetTransmitWait::Inst());
}

/////////////////////////////////////////////////////////////////////////////
//  Wait for the link layer to transmit the reset links
/////////////////////////////////////////////////////////////////////////////

PLLS_LinkResetTransmitWait PLLS_LinkResetTransmitWait::instance;

void PLLS_LinkResetTransmitWait::OnTransmitResult(LinkLayer* pLinkLayer, bool success)
{
	if (success)
	{
		// now we're waiting for an ACK
		pLinkLayer->StartTimer();
		pLinkLayer->ChangeState(PLLS_ResetLinkWait::Inst());
	}
	else
	{
		pLinkLayer->ChangeState(PLLS_SecNotReset::Inst());
		pLinkLayer->DoSendResult(success);
	}
}

/////////////////////////////////////////////////////////////////////////////
//  Wait for the link layer to transmit confirmed user data
/////////////////////////////////////////////////////////////////////////////

PLLS_ConfUserDataTransmitWait PLLS_ConfUserDataTransmitWait::instance;

void PLLS_ConfUserDataTransmitWait::OnTransmitResult(LinkLayer* pLinkLayer, bool success)
{
	if (success)
	{
		// now we're waiting on an ACK
		pLinkLayer->StartTimer();
		pLinkLayer->ChangeState(PLLS_ConfDataWait::Inst());
	}
	else
	{
		pLinkLayer->ChangeState(PLLS_SecReset::Inst());
		pLinkLayer->DoSendResult(false);
	}
}


////////////////////////////////////////////////////////
//	Class PLLS_SecReset
////////////////////////////////////////////////////////

PLLS_SecReset PLLS_SecReset::instance;

void PLLS_SecReset::SendUnconfirmed(LinkLayer* pLinkLayer, ITransportSegment& segments)
{
	pLinkLayer->pSegments = &segments;
	auto output = pLinkLayer->FormatPrimaryBufferWithUnconfirmed(segments.GetSegment());	
	pLinkLayer->QueueTransmit(output, true);
	pLinkLayer->ChangeState(PLLS_SendUnconfirmedTransmitWait<PLLS_SecReset>::Inst());	
}

void PLLS_SecReset::SendConfirmed(LinkLayer* pLinkLayer, ITransportSegment& segments)
{
	pLinkLayer->ResetRetry();
	pLinkLayer->pSegments = &segments;
	auto buffer = pLinkLayer->FormatPrimaryBufferWithConfirmed(segments.GetSegment(), pLinkLayer->NextWriteFCB());
	pLinkLayer->QueueTransmit(buffer, true);
	pLinkLayer->ChangeState(PLLS_ConfUserDataTransmitWait::Inst());

}

////////////////////////////////////////////////////////
//	Class PLLS_ResetLinkWait
////////////////////////////////////////////////////////

PLLS_ResetLinkWait PLLS_ResetLinkWait::instance;

void PLLS_ResetLinkWait::Ack(LinkLayer* pLinkLayer, bool receiveBuffFull)
{
	pLinkLayer->ResetWriteFCB();
	pLinkLayer->CancelTimer();
	auto buffer = pLinkLayer->FormatPrimaryBufferWithConfirmed(pLinkLayer->pSegments->GetSegment(), pLinkLayer->NextWriteFCB());
	pLinkLayer->QueueTransmit(buffer, true);
	pLinkLayer->ChangeState(PLLS_ConfUserDataTransmitWait::Inst());
}

void PLLS_ResetLinkWait::OnTimeout(LinkLayer* pLinkLayer)
{
	if(pLinkLayer->Retry())
	{
		FORMAT_LOG_BLOCK(pLinkLayer->GetLogger(), flags::WARN, "Link reset timeout, retrying %i remaining", pLinkLayer->RetryRemaining());
		pLinkLayer->QueueResetLinks();
		pLinkLayer->ChangeState(PLLS_LinkResetTransmitWait::Inst());
	}
	else
	{
		SIMPLE_LOG_BLOCK(pLinkLayer->GetLogger(), flags::WARN, "Link reset final timeout, no retries remain");
		pLinkLayer->ChangeState(PLLS_SecNotReset::Inst());
		pLinkLayer->DoSendResult(false);
	}
}

void PLLS_ResetLinkWait::Failure(LinkLayer* pLinkLayer)
{
	pLinkLayer->CancelTimer();
	pLinkLayer->ChangeState(PLLS_SecNotReset::Inst());
	pLinkLayer->DoSendResult(false);
}

////////////////////////////////////////////////////////
//	Class PLLS_ConfDataWait
////////////////////////////////////////////////////////

PLLS_ConfDataWait PLLS_ConfDataWait::instance;

void PLLS_ConfDataWait::Ack(LinkLayer* pLinkLayer, bool receiveBuffFull)
{
	pLinkLayer->ToggleWriteFCB();
	pLinkLayer->CancelTimer();

	if (pLinkLayer->pSegments->Advance())
	{
		auto buffer = pLinkLayer->FormatPrimaryBufferWithConfirmed(pLinkLayer->pSegments->GetSegment(), pLinkLayer->NextWriteFCB());
		pLinkLayer->QueueTransmit(buffer, true);
		pLinkLayer->ChangeState(PLLS_ConfUserDataTransmitWait::Inst());
	}
	else //we're done!
	{
		pLinkLayer->ChangeState(PLLS_SecReset::Inst());
		pLinkLayer->DoSendResult(true);
	}
}

void PLLS_ConfDataWait::Nack(LinkLayer* pLinkLayer, bool receiveBuffFull)
{
	if (receiveBuffFull)
	{
		Failure(pLinkLayer);
	}
	else
	{
		pLinkLayer->ResetRetry();
		pLinkLayer->CancelTimer();
		pLinkLayer->ChangeState(PLLS_LinkResetTransmitWait::Inst());
		pLinkLayer->QueueResetLinks();
	}
}

void PLLS_ConfDataWait::Failure(LinkLayer* pLinkLayer)
{
	pLinkLayer->CancelTimer();
	pLinkLayer->ChangeState(PLLS_SecNotReset::Inst());
	pLinkLayer->DoSendResult(false);
}

void PLLS_ConfDataWait::OnTimeout(LinkLayer* pLinkLayer)
{
	if(pLinkLayer->Retry())
	{
		FORMAT_LOG_BLOCK(pLinkLayer->GetLogger(), flags::WARN, "confirmed data timeout, retrying %u remaining", pLinkLayer->RetryRemaining());
		auto buffer = pLinkLayer->FormatPrimaryBufferWithConfirmed(pLinkLayer->pSegments->GetSegment(), pLinkLayer->NextWriteFCB());
		pLinkLayer->QueueTransmit(buffer, true);
		pLinkLayer->ChangeState(PLLS_ConfUserDataTransmitWait::Inst());
	}
	else
	{
		SIMPLE_LOG_BLOCK(pLinkLayer->GetLogger(), flags::WARN, "Confirmed data final timeout, no retries remain");
		pLinkLayer->ChangeState(PLLS_SecNotReset::Inst());
		pLinkLayer->DoSendResult(false);
	}
}

} //end namepsace

