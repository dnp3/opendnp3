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


#include <openpal/LogMacros.h>

#include "opendnp3/DNPErrorCodes.h"
#include "opendnp3/link/LinkLayer.h"
#include "opendnp3/LogLevels.h"

using namespace openpal;

namespace opendnp3
{

////////////////////////////////////////
// PriStateBase
////////////////////////////////////////

void PriStateBase::Ack(LinkLayer* apLL, bool aIsRcvBuffFull)
{
	SIMPLE_LOG_BLOCK_WITH_CODE(apLL->GetLogger(), flags::WARN, DLERR_UNEXPECTED_FRAME, "Frame context not understood");
}

void PriStateBase::Nack(LinkLayer* apLL, bool aIsRcvBuffFull)
{
	SIMPLE_LOG_BLOCK_WITH_CODE(apLL->GetLogger(), flags::WARN, DLERR_UNEXPECTED_FRAME, "Frame context not understood");
}

void PriStateBase::LinkStatus(LinkLayer* apLL, bool aIsRcvBuffFull)
{
	SIMPLE_LOG_BLOCK_WITH_CODE(apLL->GetLogger(), flags::WARN, DLERR_UNEXPECTED_FRAME, "Frame context not understood");
}

void PriStateBase::NotSupported (LinkLayer* apLL, bool aIsRcvBuffFull)
{
	SIMPLE_LOG_BLOCK_WITH_CODE(apLL->GetLogger(), flags::WARN, DLERR_UNEXPECTED_FRAME, "Frame context not understood");
}

void PriStateBase::OnTransmitResult(LinkLayer* apLL, bool success)
{
	FORMAT_LOG_BLOCK(apLL->GetLogger(), flags::ERR, "Invalid action for state: %s", this->Name());	
}

void PriStateBase::OnTimeout(LinkLayer* apLL)
{
	FORMAT_LOG_BLOCK(apLL->GetLogger(), flags::ERR, "Invalid action for state: %s", this->Name());
}

void PriStateBase::SendConfirmed(LinkLayer* apLL, IBufferSegment&)
{
	FORMAT_LOG_BLOCK(apLL->GetLogger(), flags::ERR, "Invalid action for state: %s", this->Name());
}

void PriStateBase::SendUnconfirmed(LinkLayer* apLL, IBufferSegment&)
{
	FORMAT_LOG_BLOCK(apLL->GetLogger(), flags::ERR, "Invalid action for state: %s", this->Name());
}

////////////////////////////////////////////////////////
//	Class PLLS_SecNotResetIdle
////////////////////////////////////////////////////////

PLLS_SecNotReset PLLS_SecNotReset::mInstance;

void PLLS_SecNotReset::SendUnconfirmed(LinkLayer* apLL, IBufferSegment& segments)
{
	auto output = apLL->FormatPrimaryBufferWithUnconfirmed(segments);
	if (output.IsEmpty())
	{
		SIMPLE_LOG_BLOCK(apLL->GetLogger(), flags::ERR, "upper layer provided more data than can be transmitted");
		apLL->PostSendResult(false);
	}
	else
	{
		apLL->QueueTransmit(output, true);
		apLL->ChangeState(PLLS_SendUnconfirmedTransmitWait<PLLS_SecNotReset>::Inst());
	}
}

void PLLS_SecNotReset::SendConfirmed(LinkLayer* apLL, IBufferSegment& segments)
{
	apLL->ResetRetry();
	apLL->pConfirmedSegments = &segments;
	apLL->QueueResetLinks();
	apLL->ChangeState(PLLS_LinkResetTransmitWait::Inst());
}

/////////////////////////////////////////////////////////////////////////////
//  Wait for the link layer to transmit the reset links
/////////////////////////////////////////////////////////////////////////////

PLLS_LinkResetTransmitWait PLLS_LinkResetTransmitWait::mInstance;

void PLLS_LinkResetTransmitWait::OnTransmitResult(LinkLayer* apLL, bool success)
{
	if (success)
	{
		// now we're waiting for an ACK
		apLL->StartTimer();
		apLL->ChangeState(PLLS_ResetLinkWait::Inst());
	}
	else
	{
		apLL->ChangeState(PLLS_SecNotReset::Inst());
		apLL->DoSendResult(success);
	}
}

/////////////////////////////////////////////////////////////////////////////
//  Wait for the link layer to transmit confirmed user data
/////////////////////////////////////////////////////////////////////////////

PLLS_ConfUserDataTransmitWait PLLS_ConfUserDataTransmitWait::mInstance;

void PLLS_ConfUserDataTransmitWait::OnTransmitResult(LinkLayer* apLL, bool success)
{
	if (success)
	{
		// now we're waiting on an ACK
		apLL->StartTimer();
		apLL->ChangeState(PLLS_ConfDataWait::Inst());
	}
	else
	{
		apLL->ChangeState(PLLS_SecReset::Inst());
		apLL->DoSendResult(false);
	}
}


////////////////////////////////////////////////////////
//	Class PLLS_SecReset
////////////////////////////////////////////////////////

PLLS_SecReset PLLS_SecReset::mInstance;

void PLLS_SecReset::SendUnconfirmed(LinkLayer* apLL, IBufferSegment& segments)
{
	auto output = apLL->FormatPrimaryBufferWithUnconfirmed(segments);
	if (output.IsEmpty())
	{
		SIMPLE_LOG_BLOCK(apLL->GetLogger(), flags::ERR, "upper layer provided more data than can be transmitted");
		apLL->PostSendResult(false);
	}
	else
	{
		apLL->QueueTransmit(output, true);
		apLL->ChangeState(PLLS_SendUnconfirmedTransmitWait<PLLS_SecReset>::Inst());
	}
}

void PLLS_SecReset::SendConfirmed(LinkLayer* apLL, IBufferSegment& segments)
{
	apLL->ResetRetry();
	apLL->pConfirmedSegments = &segments;
	auto buffer = apLL->FormatPrimaryBufferWithConfirmed(segments, apLL->NextWriteFCB());
	apLL->QueueTransmit(buffer, true);
	apLL->ChangeState(PLLS_ConfUserDataTransmitWait::Inst());

}

////////////////////////////////////////////////////////
//	Class PLLS_ResetLinkWait
////////////////////////////////////////////////////////

PLLS_ResetLinkWait PLLS_ResetLinkWait::mInstance;

void PLLS_ResetLinkWait::Ack(LinkLayer* apLL, bool aIsRcvBuffFull)
{
	apLL->ResetWriteFCB();
	apLL->CancelTimer();
	auto buffer = apLL->FormatPrimaryBufferWithConfirmed(*apLL->pConfirmedSegments, apLL->NextWriteFCB());
	apLL->QueueTransmit(buffer, true);
	apLL->ChangeState(PLLS_ConfUserDataTransmitWait::Inst());
}

void PLLS_ResetLinkWait::OnTimeout(LinkLayer* apLL)
{
	if(apLL->Retry())
	{
		FORMAT_LOG_BLOCK_WITH_CODE(apLL->GetLogger(), flags::WARN, DLERR_TIMEOUT_RETRY, "Confirmed data timeout, retrying %i remaining", apLL->RetryRemaining());
		apLL->QueueResetLinks();
		apLL->ChangeState(PLLS_LinkResetTransmitWait::Inst());
	}
	else
	{
		SIMPLE_LOG_BLOCK_WITH_CODE(apLL->GetLogger(), flags::WARN, DLERR_TIMEOUT_NO_RETRY, "Confirmed data final timeout");
		apLL->ChangeState(PLLS_SecNotReset::Inst());
		apLL->DoSendResult(false);
	}
}

void PLLS_ResetLinkWait::Failure(LinkLayer* apLL)
{
	apLL->CancelTimer();
	apLL->ChangeState(PLLS_SecNotReset::Inst());
	apLL->DoSendResult(false);
}

////////////////////////////////////////////////////////
//	Class PLLS_ConfDataWait
////////////////////////////////////////////////////////

PLLS_ConfDataWait PLLS_ConfDataWait::mInstance;

void PLLS_ConfDataWait::Ack(LinkLayer* apLL, bool aIsRcvBuffFull)
{
	apLL->ToggleWriteFCB();
	apLL->CancelTimer();

	if (apLL->pConfirmedSegments->Advance())
	{
		auto buffer = apLL->FormatPrimaryBufferWithConfirmed(*apLL->pConfirmedSegments, apLL->NextWriteFCB());
		apLL->QueueTransmit(buffer, true);
		apLL->ChangeState(PLLS_ConfUserDataTransmitWait::Inst());
	}
	else //we're done!
	{
		apLL->ChangeState(PLLS_SecReset::Inst());
		apLL->DoSendResult(true);
	}
}

void PLLS_ConfDataWait::Nack(LinkLayer* apLL, bool aIsRcvBuffFull)
{
	if (aIsRcvBuffFull)
	{
		Failure(apLL);
	}
	else
	{
		apLL->ResetRetry();
		apLL->CancelTimer();
		apLL->ChangeState(PLLS_LinkResetTransmitWait::Inst());
		apLL->QueueResetLinks();
	}
}

void PLLS_ConfDataWait::Failure(LinkLayer* apLL)
{
	apLL->CancelTimer();
	apLL->ChangeState(PLLS_SecNotReset::Inst());
	apLL->DoSendResult(false);
}

void PLLS_ConfDataWait::OnTimeout(LinkLayer* apLL)
{
	if(apLL->Retry())
	{
		SIMPLE_LOG_BLOCK_WITH_CODE(apLL->GetLogger(), flags::WARN, DLERR_TIMEOUT_RETRY, "Retry confirmed data");
		auto buffer = apLL->FormatPrimaryBufferWithConfirmed(*apLL->pConfirmedSegments, apLL->NextWriteFCB());
		apLL->QueueTransmit(buffer, true);
		apLL->ChangeState(PLLS_ConfUserDataTransmitWait::Inst());
	}
	else
	{
		SIMPLE_LOG_BLOCK_WITH_CODE(apLL->GetLogger(), flags::WARN, DLERR_TIMEOUT_NO_RETRY, "Confirmed data timeout");
		apLL->ChangeState(PLLS_SecNotReset::Inst());
		apLL->DoSendResult(false);
	}
}

} //end namepsace

