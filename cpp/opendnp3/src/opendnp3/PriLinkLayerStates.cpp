#include "PriLinkLayerStates.h"

#include <openpal/Exception.h>
#include <opendnp3/DNPConstants.h>

#include <openpal/LoggableMacros.h>

#include "LinkLayer.h"

using namespace openpal;

namespace opendnp3
{

////////////////////////////////////////
// PriStateBase
////////////////////////////////////////

void PriStateBase::Ack(LinkLayer* apLL, bool aIsRcvBuffFull)
{
	ERROR_LOGGER_BLOCK(apLL->GetLogger(), LogLevel::Warning, "Frame context not understood", DLERR_UNEXPECTED_FRAME);
}
void PriStateBase::Nack(LinkLayer* apLL, bool aIsRcvBuffFull)
{
	ERROR_LOGGER_BLOCK(apLL->GetLogger(), LogLevel::Warning, "Frame context not understood", DLERR_UNEXPECTED_FRAME);
}
void PriStateBase::LinkStatus(LinkLayer* apLL, bool aIsRcvBuffFull)
{
	ERROR_LOGGER_BLOCK(apLL->GetLogger(), LogLevel::Warning, "Frame context not understood", DLERR_UNEXPECTED_FRAME);
}
void PriStateBase::NotSupported (LinkLayer* apLL, bool aIsRcvBuffFull)
{
	ERROR_LOGGER_BLOCK(apLL->GetLogger(), LogLevel::Warning, "Frame context not understood", DLERR_UNEXPECTED_FRAME);
}

void PriStateBase::OnTimeout(LinkLayer* apLL)
{
	MACRO_THROW_EXCEPTION(InvalidStateException, this->Name());
}

void PriStateBase::SendConfirmed(LinkLayer*, const ReadOnlyBuffer&)
{
	MACRO_THROW_EXCEPTION(InvalidStateException, this->Name());
}

void PriStateBase::SendUnconfirmed(LinkLayer*, const ReadOnlyBuffer&)
{
	MACRO_THROW_EXCEPTION(InvalidStateException, this->Name());
}

////////////////////////////////////////////////////////
//	Class PLLS_SecNotResetIdle
////////////////////////////////////////////////////////

PLLS_SecNotReset PLLS_SecNotReset::mInstance;

void PLLS_SecNotReset::SendUnconfirmed(LinkLayer* apLL, const ReadOnlyBuffer& arBuffer)
{
	apLL->SendUnconfirmedUserData(arBuffer);

}

void PLLS_SecNotReset::SendConfirmed(LinkLayer* apLL, const ReadOnlyBuffer& arBuffer)
{
	apLL->ResetRetry();
	apLL->StartTimer();
	apLL->ChangeState(PLLS_ResetLinkWait::Inst());

	// what we'll send if we successfully reset link state
	apLL->mDelayedPriFrame.FormatConfirmedUserData(apLL->mCONFIG.IsMaster, true, apLL->mCONFIG.RemoteAddr, apLL->mCONFIG.LocalAddr, arBuffer, arBuffer.Size());
	apLL->SendResetLinks();
}

////////////////////////////////////////////////////////
//	Class PLLS_SecReset
////////////////////////////////////////////////////////

PLLS_SecReset PLLS_SecReset::mInstance;

void PLLS_SecReset::SendUnconfirmed(LinkLayer* apLL, const ReadOnlyBuffer& arBuffer)
{
	apLL->SendUnconfirmedUserData(arBuffer);
}

void PLLS_SecReset::SendConfirmed(LinkLayer* apLL, const ReadOnlyBuffer& arBuffer)
{
	apLL->ResetRetry();
	apLL->StartTimer();
	apLL->ChangeState(PLLS_ConfDataWait::Inst());

	apLL->mDelayedPriFrame.FormatConfirmedUserData(apLL->mCONFIG.IsMaster, apLL->NextWriteFCB(), apLL->mCONFIG.RemoteAddr, apLL->mCONFIG.LocalAddr, arBuffer, arBuffer.Size());
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
		ERROR_LOGGER_BLOCK(apLL->GetLogger(), LogLevel::Warning, "Confirmed data timeout, retrying, " << apLL->RetryRemaining() << " remaining", DLERR_TIMEOUT_RETRY);
		apLL->StartTimer();
		apLL->SendResetLinks();
	}
	else {
		ERROR_LOGGER_BLOCK(apLL->GetLogger(), LogLevel::Warning, "Confirmed data final timeout", DLERR_TIMEOUT_NO_RETRY);
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

void PLLS_ConfDataWait::Nack(LinkLayer* apLL, bool aIsRcvBuffFull)
{
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
		ERROR_LOGGER_BLOCK(apLL->GetLogger(), LogLevel::Warning, "Retry confirmed data", DLERR_TIMEOUT_RETRY);
		apLL->StartTimer();
		apLL->ChangeState(PLLS_ConfDataWait::Inst());
		apLL->SendDelayedUserData(apLL->NextWriteFCB());
	}
	else {
		ERROR_LOGGER_BLOCK(apLL->GetLogger(), LogLevel::Warning, "Confirmed data timeout", DLERR_TIMEOUT_NO_RETRY);
		apLL->ChangeState(PLLS_SecNotReset::Inst());
		apLL->DoSendFailure();
	}
}

} //end namepsace

