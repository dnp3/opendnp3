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

void SecStateBase::OnTransmitResult(LinkLayer* apLL, bool success)
{
	FORMAT_LOG_BLOCK(apLL->GetLogger(), flags::ERR, "Invalid event for state: %s", this->Name());
}

////////////////////////////////////////////////////////
//	Class SLLS_NotReset
////////////////////////////////////////////////////////
SLLS_NotReset SLLS_NotReset::instance;

void SLLS_NotReset::TestLinkStatus(LinkLayer* apLL, bool aFcb)
{
	SIMPLE_LOG_BLOCK_WITH_CODE(apLL->GetLogger(), flags::WARN, DLERR_UNEXPECTED_LPDU, "TestLinkStatus ignored");
}

void SLLS_NotReset::ConfirmedUserData(LinkLayer* apLL, bool aFcb, const openpal::ReadOnlyBuffer&)
{
	SIMPLE_LOG_BLOCK_WITH_CODE(apLL->GetLogger(), flags::WARN, DLERR_UNEXPECTED_LPDU, "ConfirmedUserData ignored");
}

void SLLS_NotReset::ResetLinkStates(LinkLayer* apLL)
{
	apLL->QueueAck();
	apLL->ResetReadFCB();
	apLL->ChangeState(SLLS_TransmitWaitReset::Inst());
}

void SLLS_NotReset::RequestLinkStatus(LinkLayer* apLL)
{
	apLL->QueueLinkStatus();
	apLL->ChangeState(SLLS_TransmitWaitNotReset::Inst());
}

////////////////////////////////////////////////////////
//	Class SLLS_Reset
////////////////////////////////////////////////////////
SLLS_Reset SLLS_Reset::instance;

void SLLS_Reset::TestLinkStatus(LinkLayer* apLL, bool aFcb)
{
	if(apLL->NextReadFCB() == aFcb)
	{
		apLL->QueueAck();
		apLL->ToggleReadFCB();
		apLL->ChangeState(SLLS_TransmitWaitReset::Inst());
	}
	else
	{
		// "Re-transmit most recent response that contained function code 0 (ACK) or 1 (NACK)."
		// This is a PITA implement
		// TODO - see if this function is deprecated or not
		SIMPLE_LOG_BLOCK(apLL->GetLogger(), flags::WARN, "Received TestLinkStatus with invalid FCB");
	}
}

void SLLS_Reset::ConfirmedUserData(LinkLayer* apLL, bool aFcb, const openpal::ReadOnlyBuffer& arBuffer)
{
	apLL->QueueAck();

	if (apLL->NextReadFCB() == aFcb)
	{
		apLL->ToggleReadFCB();
		apLL->DoDataUp(arBuffer);
	}
	else
	{
		SIMPLE_LOG_BLOCK(apLL->GetLogger(), flags::WARN, "Confirmed data w/ wrong FCB");
	}

	apLL->ChangeState(SLLS_TransmitWaitReset::Inst());
}

void SLLS_Reset::ResetLinkStates(LinkLayer* apLL)
{
	apLL->QueueAck();
	apLL->ResetReadFCB();
	apLL->ChangeState(SLLS_TransmitWaitReset::Inst());
}

void SLLS_Reset::RequestLinkStatus(LinkLayer* apLL)
{
	apLL->QueueLinkStatus();
	apLL->ChangeState(SLLS_TransmitWaitReset::Inst());
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

