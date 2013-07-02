
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the terms of the License.
//

#include "PriLinkLayerStates.h"

#include <opendnp3/Exception.h>
#include <opendnp3/Logger.h>
#include <opendnp3/DNPConstants.h>

#include "Loggable.h"
#include "LoggableMacros.h"
#include "LinkLayer.h"

namespace opendnp3
{

////////////////////////////////////////
// PriStateBase
////////////////////////////////////////

void PriStateBase::Ack(LinkLayer* apLL, bool aIsRcvBuffFull)
{
	ERROR_LOGGER_BLOCK(apLL->GetLogger(), LEV_WARNING, "Frame context not understood", DLERR_UNEXPECTED_FRAME);
}
void PriStateBase::Nack(LinkLayer* apLL, bool aIsRcvBuffFull)
{
	ERROR_LOGGER_BLOCK(apLL->GetLogger(), LEV_WARNING, "Frame context not understood", DLERR_UNEXPECTED_FRAME);
}
void PriStateBase::LinkStatus(LinkLayer* apLL, bool aIsRcvBuffFull)
{
	ERROR_LOGGER_BLOCK(apLL->GetLogger(), LEV_WARNING, "Frame context not understood", DLERR_UNEXPECTED_FRAME);
}
void PriStateBase::NotSupported (LinkLayer* apLL, bool aIsRcvBuffFull)
{
	ERROR_LOGGER_BLOCK(apLL->GetLogger(), LEV_WARNING, "Frame context not understood", DLERR_UNEXPECTED_FRAME);
}

void PriStateBase::OnTimeout(LinkLayer* apLL)
{
	MACRO_THROW_EXCEPTION(InvalidStateException, this->Name());
}

void PriStateBase::SendConfirmed(LinkLayer*, const uint8_t*, size_t)
{
	MACRO_THROW_EXCEPTION(InvalidStateException, this->Name());
}

void PriStateBase::SendUnconfirmed(LinkLayer*, const uint8_t*, size_t)
{
	MACRO_THROW_EXCEPTION(InvalidStateException, this->Name());
}

////////////////////////////////////////////////////////
//	Class PLLS_SecNotResetIdle
////////////////////////////////////////////////////////

PLLS_SecNotReset PLLS_SecNotReset::mInstance;

void PLLS_SecNotReset::SendUnconfirmed(LinkLayer* apLL, const uint8_t* apData, size_t aLength)
{
	apLL->SendUnconfirmedUserData(apData, aLength);

}

void PLLS_SecNotReset::SendConfirmed(LinkLayer* apLL, const uint8_t* apData, size_t aLength)
{
	apLL->ResetRetry();
	apLL->StartTimer();
	apLL->ChangeState(PLLS_ResetLinkWait::Inst());

	// what we'll send if we successfully reset link state
	apLL->mDelayedPriFrame.FormatConfirmedUserData(apLL->mCONFIG.IsMaster, true, apLL->mCONFIG.RemoteAddr, apLL->mCONFIG.LocalAddr, apData, aLength);
	apLL->SendResetLinks();
}

////////////////////////////////////////////////////////
//	Class PLLS_SecReset
////////////////////////////////////////////////////////

PLLS_SecReset PLLS_SecReset::mInstance;

void PLLS_SecReset::SendUnconfirmed(LinkLayer* apLL, const uint8_t* apData, size_t aLength)
{
	apLL->SendUnconfirmedUserData(apData, aLength);
}

void PLLS_SecReset::SendConfirmed(LinkLayer* apLL, const uint8_t* apData, size_t aLength)
{
	apLL->ResetRetry();
	apLL->StartTimer();
	apLL->ChangeState(PLLS_ConfDataWait::Inst());

	apLL->mDelayedPriFrame.FormatConfirmedUserData(apLL->mCONFIG.IsMaster, apLL->NextWriteFCB(), apLL->mCONFIG.RemoteAddr, apLL->mCONFIG.LocalAddr, apData, aLength);
	apLL->SendDelayedUserData(apLL->NextWriteFCB());
}

////////////////////////////////////////////////////////
//	Class PLLS_ResetLinkWait
////////////////////////////////////////////////////////

PLLS_ResetLinkWait PLLS_ResetLinkWait::mInstance;

void PLLS_ResetLinkWait::Ack(LinkLayer* apLL, bool aIsRcvBuffFull)
{
	apLL->ResetWriteFCB();
	apLL->CancelTimer();
	apLL->StartTimer();
	apLL->ChangeState(PLLS_ConfDataWait::Inst());
	apLL->SendDelayedUserData(apLL->NextWriteFCB());
}

void PLLS_ResetLinkWait::OnTimeout(LinkLayer* apLL)
{
	if(apLL->Retry()) {
		ERROR_LOGGER_BLOCK(apLL->GetLogger(), LEV_WARNING, "Confirmed data timeout, retrying, " << apLL->RetryRemaining() << " remaining", DLERR_TIMEOUT_RETRY);
		apLL->StartTimer();
		apLL->SendResetLinks();
	}
	else {
		ERROR_LOGGER_BLOCK(apLL->GetLogger(), LEV_WARNING, "Confirmed data final timeout", DLERR_TIMEOUT_NO_RETRY);
		apLL->ChangeState(PLLS_SecNotReset::Inst());
		apLL->DoSendFailure();
	}
}

void PLLS_ResetLinkWait::Failure(LinkLayer* apLL)
{
	apLL->CancelTimer();
	apLL->ChangeState(PLLS_SecNotReset::Inst());
	apLL->DoSendFailure();
}

////////////////////////////////////////////////////////
//	Class PLLS_ConfDataWait
////////////////////////////////////////////////////////

PLLS_ConfDataWait PLLS_ConfDataWait::mInstance;

void PLLS_ConfDataWait::Ack(LinkLayer* apLL, bool aIsRcvBuffFull)
{
	apLL->ToggleWriteFCB();
	apLL->CancelTimer();
	apLL->ChangeState(PLLS_SecReset::Inst());
	apLL->DoSendSuccess();
}

void PLLS_ConfDataWait::Nack(LinkLayer* apLL, bool aIsRcvBuffFull) {
		if(aIsRcvBuffFull) Failure(apLL);
		else {
			apLL->ResetRetry();
			apLL->CancelTimer();
			apLL->StartTimer();
			apLL->ChangeState(PLLS_ResetLinkWait::Inst());
			apLL->SendResetLinks();
		}
}

void PLLS_ConfDataWait::Failure(LinkLayer* apLL)
{
	apLL->CancelTimer();
	apLL->ChangeState(PLLS_SecNotReset::Inst());
	apLL->DoSendFailure();
}

void PLLS_ConfDataWait::OnTimeout(LinkLayer* apLL)
{
	if(apLL->Retry()) {
		ERROR_LOGGER_BLOCK(apLL->GetLogger(), LEV_WARNING, "Retry confirmed data", DLERR_TIMEOUT_RETRY);
		apLL->StartTimer();
		apLL->ChangeState(PLLS_ConfDataWait::Inst());
		apLL->SendDelayedUserData(apLL->NextWriteFCB());
	}
	else {
		ERROR_LOGGER_BLOCK(apLL->GetLogger(), LEV_WARNING, "Confirmed data timeout", DLERR_TIMEOUT_NO_RETRY);
		apLL->ChangeState(PLLS_SecNotReset::Inst());
		apLL->DoSendFailure();
	}
}

} //end namepsace

