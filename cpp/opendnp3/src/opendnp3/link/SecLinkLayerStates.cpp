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

#include "opendnp3/DNPConstants.h"
#include "opendnp3/link/LinkLayer.h"

using namespace openpal;

namespace opendnp3
{

////////////////////////////////////////
// SecStateBase
////////////////////////////////////////

void SecStateBase::ResetLinkStates(LinkLayer* apLL)
{
	apLL->ResetReadFCB();
	apLL->SendAck();
	apLL->ChangeState(SLLS_Reset::Inst());
}

void SecStateBase::RequestLinkStatus(LinkLayer* apLL)
{
	apLL->SendLinkStatus();
}

void SecStateBase::UnconfirmedUserData(LinkLayer* apLL, const ReadOnlyBuffer& arBuffer)
{
	apLL->DoDataUp(arBuffer);
}

////////////////////////////////////////////////////////
//	Class SLLS_NotReset
////////////////////////////////////////////////////////
SLLS_NotReset SLLS_NotReset::mInstance;

void SLLS_NotReset::TestLinkStatus(LinkLayer* apLL, bool aFcb)
{
	ERROR_LOGGER_BLOCK(apLL->GetLogger(), LogLevel::Warning, "TestLinkStatus ignored", DLERR_UNEXPECTED_FRAME);
}

void SLLS_NotReset::ConfirmedUserData(LinkLayer* apLL, bool aFcb, const openpal::ReadOnlyBuffer&)
{
	ERROR_LOGGER_BLOCK(apLL->GetLogger(), LogLevel::Warning, "ConfirmedUserData ignored", DLERR_UNEXPECTED_FRAME);
}

////////////////////////////////////////////////////////
//	Class SLLS_Reset
////////////////////////////////////////////////////////
SLLS_Reset SLLS_Reset::mInstance;

void SLLS_Reset::TestLinkStatus(LinkLayer* apLL, bool aFcb)
{
	if(apLL->NextReadFCB() == aFcb) {
		apLL->ToggleReadFCB();
		apLL->SendAck();
	}
	else {
		// "Re-transmit most recent response that contained function code 0 (ACK) or 1 (NACK)."
		// This is a pain in the pass to implement.
		ERROR_LOGGER_BLOCK(apLL->GetLogger(), LogLevel::Warning, "TestLinkStatus with invalid FCB", DLERR_WRONG_FCB_ON_TEST);
	}
}

void SLLS_Reset::ConfirmedUserData(LinkLayer* apLL, bool aFcb, const openpal::ReadOnlyBuffer& arBuffer)
{
	apLL->SendAck();

	if(apLL->NextReadFCB() == aFcb) {
		apLL->ToggleReadFCB();
		apLL->DoDataUp(arBuffer);
	}
	else {
		ERROR_LOGGER_BLOCK(apLL->GetLogger(), LogLevel::Warning, "Confirmed data w/ wrong FCB", DLERR_WRONG_FCB_ON_RECEIVE_DATA);
	}
}

} //end namepsace

