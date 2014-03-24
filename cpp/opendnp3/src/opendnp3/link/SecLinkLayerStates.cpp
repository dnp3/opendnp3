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


#include <openpal/LoggableMacros.h>

#include "opendnp3/DNPErrorCodes.h"
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
	LOGGER_BLOCK(apLL->GetLogger(), levels::ERR, "Invalid event for state: " << this->Name());
}

////////////////////////////////////////////////////////
//	Class SLLS_NotReset
////////////////////////////////////////////////////////
SLLS_NotReset SLLS_NotReset::mInstance;

void SLLS_NotReset::TestLinkStatus(LinkLayer* apLL, bool aFcb)
{
	ERROR_LOGGER_BLOCK(apLL->GetLogger(), levels::WARN, "TestLinkStatus ignored", DLERR_UNEXPECTED_FRAME);
}

void SLLS_NotReset::ConfirmedUserData(LinkLayer* apLL, bool aFcb, const openpal::ReadOnlyBuffer&)
{
	ERROR_LOGGER_BLOCK(apLL->GetLogger(), levels::WARN, "ConfirmedUserData ignored", DLERR_UNEXPECTED_FRAME);
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
SLLS_Reset SLLS_Reset::mInstance;

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
		// This is a pain in the pass to implement.
		// TODO - see if this function is deprecated or not
		ERROR_LOGGER_BLOCK(apLL->GetLogger(), levels::WARN, "TestLinkStatus with invalid FCB", DLERR_WRONG_FCB_ON_TEST);
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
		ERROR_LOGGER_BLOCK(apLL->GetLogger(), levels::WARN, "Confirmed data w/ wrong FCB", DLERR_WRONG_FCB_ON_RECEIVE_DATA);
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
SLLS_TransmitWaitReset SLLS_TransmitWaitReset::mInstance;

////////////////////////////////////////////////////////
//	Class SLLS_TransmitWaitNotReset
////////////////////////////////////////////////////////
SLLS_TransmitWaitNotReset SLLS_TransmitWaitNotReset::mInstance;

} //end namepsace

