#include "LinkLayer.h"

#include <assert.h>

#include <openpal/Exception.h>
#include <opendnp3/DNPConstants.h>

#include <openpal/LoggableMacros.h>

#include "ILinkRouter.h"
#include "PriLinkLayerStates.h"
#include "SecLinkLayerStates.h"

using namespace openpal;


namespace opendnp3
{

LinkLayer::LinkLayer(Logger aLogger, openpal::IExecutor* apExecutor, const LinkConfig& arConfig) :
	Loggable(aLogger),
	ILowerLayer(aLogger),
	mCONFIG(arConfig),
	mRetryRemaining(0),
	mpExecutor(apExecutor),
	mpTimer(nullptr),
	mNextReadFCB(false),
	mNextWriteFCB(false),
	mIsOnline(false),
	mpRouter(nullptr),
	mpPriState(PLLS_SecNotReset::Inst()),
	mpSecState(SLLS_NotReset::Inst())
{}

void LinkLayer::SetRouter(ILinkRouter* apRouter)
{
	assert(mpRouter == nullptr); assert(apRouter != nullptr);
	mpRouter = apRouter;
}

void LinkLayer::ChangeState(PriStateBase* apState)
{
	mpPriState = apState;
}

void LinkLayer::ChangeState(SecStateBase* apState)
{
	mpSecState = apState;
}

bool LinkLayer::Validate(bool aIsMaster, uint16_t aSrc, uint16_t aDest)
{
	if(!mIsOnline)
		MACRO_THROW_EXCEPTION(InvalidStateException, "LowerLayerDown");

	if(aIsMaster == mCONFIG.IsMaster) {
		ERROR_BLOCK(LogLevel::Warning,
		            (aIsMaster ? "Master frame received for master" : "Slave frame received for slave"),
		            DLERR_MASTER_BIT_MATCH);
		return false;
	}

	if(aDest != mCONFIG.LocalAddr) {
		ERROR_BLOCK(LogLevel::Warning, "Frame for unknown destintation", DLERR_UNKNOWN_DESTINATION);
		return false;
	}

	if(aSrc != mCONFIG.RemoteAddr) {
		ERROR_BLOCK(LogLevel::Warning, "Frame from unknwon source", DLERR_UNKNOWN_SOURCE);
		return false;
	}

	return true;
}

////////////////////////////////
// ILinkContext
////////////////////////////////

void LinkLayer::OnLowerLayerUp()
{
	if(mIsOnline)
		MACRO_THROW_EXCEPTION(InvalidStateException, "LowerLayerUp");
	mIsOnline = true;
	if(mpUpperLayer) mpUpperLayer->OnLowerLayerUp();
}

void LinkLayer::OnLowerLayerDown()
{
	if(!mIsOnline)
		MACRO_THROW_EXCEPTION(InvalidStateException, "LowerLayerDown");

	if(mpTimer != nullptr) this->CancelTimer();
	mIsOnline = false;
	mpPriState = PLLS_SecNotReset::Inst();
	mpSecState = SLLS_NotReset::Inst();

	if(mpUpperLayer) mpUpperLayer->OnLowerLayerDown();
}

void LinkLayer::Transmit(const LinkFrame& arFrame)
{
	mpRouter->Transmit(arFrame);
}

void LinkLayer::SendAck()
{
	mSecFrame.FormatAck(mCONFIG.IsMaster, false, mCONFIG.RemoteAddr, mCONFIG.LocalAddr);
	this->Transmit(mSecFrame);
}

void LinkLayer::SendLinkStatus()
{
	mSecFrame.FormatLinkStatus(mCONFIG.IsMaster, false, mCONFIG.RemoteAddr, mCONFIG.LocalAddr);
	this->Transmit(mSecFrame);
}

void LinkLayer::SendResetLinks()
{
	mPriFrame.FormatResetLinkStates(mCONFIG.IsMaster, mCONFIG.RemoteAddr, mCONFIG.LocalAddr);
	this->Transmit(mPriFrame);
}

void LinkLayer::SendUnconfirmedUserData(const ReadOnlyBuffer& arBuffer)
{
	mPriFrame.FormatUnconfirmedUserData(mCONFIG.IsMaster, mCONFIG.RemoteAddr, mCONFIG.LocalAddr, arBuffer, arBuffer.Size());
	this->Transmit(mPriFrame);
	this->DoSendSuccess();
}

void LinkLayer::SendDelayedUserData(bool aFCB)
{
	mDelayedPriFrame.ChangeFCB(aFCB);
	this->Transmit(mDelayedPriFrame);
}

void LinkLayer::StartTimer()
{
	assert(mpTimer == nullptr);
	mpTimer = this->mpExecutor->Start(TimeDuration(mCONFIG.Timeout), std::bind(&LinkLayer::OnTimeout, this));
}

void LinkLayer::CancelTimer()
{
	assert(mpTimer);
	mpTimer->Cancel();
	mpTimer = nullptr;
}

void LinkLayer::ResetRetry()
{
	this->mRetryRemaining = mCONFIG.NumRetry;
}

bool LinkLayer::Retry()
{
	if(mRetryRemaining > 0) {
		--mRetryRemaining;
		return true;
	}
	else return false;
}

////////////////////////////////
// IFrameSink
////////////////////////////////

void LinkLayer::Ack(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc)
{
	if(this->Validate(aIsMaster, aSrc, aDest))
		mpPriState->Ack(this, aIsRcvBuffFull);
}

void LinkLayer::Nack(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc)
{
	if(this->Validate(aIsMaster, aSrc, aDest))
		mpPriState->Nack(this, aIsRcvBuffFull);
}

void LinkLayer::LinkStatus(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc)
{
	if(this->Validate(aIsMaster, aSrc, aDest))
		mpPriState->LinkStatus(this, aIsRcvBuffFull);
}

void LinkLayer::NotSupported (bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc)
{
	if(this->Validate(aIsMaster, aSrc, aDest))
		mpPriState->NotSupported(this, aIsRcvBuffFull);
}

void LinkLayer::TestLinkStatus(bool aIsMaster, bool aFcb, uint16_t aDest, uint16_t aSrc)
{
	if(this->Validate(aIsMaster, aSrc, aDest))
		mpSecState->TestLinkStatus(this, aFcb);
}

void LinkLayer::ResetLinkStates(bool aIsMaster, uint16_t aDest, uint16_t aSrc)
{
	if(this->Validate(aIsMaster, aSrc, aDest))
		mpSecState->ResetLinkStates(this);
}

void LinkLayer::RequestLinkStatus(bool aIsMaster, uint16_t aDest, uint16_t aSrc)
{
	if(this->Validate(aIsMaster, aSrc, aDest))
		mpSecState->RequestLinkStatus(this);
}

void LinkLayer::ConfirmedUserData(bool aIsMaster, bool aFcb, uint16_t aDest, uint16_t aSrc, const ReadOnlyBuffer& arBuffer)
{
	if(this->Validate(aIsMaster, aSrc, aDest))
		mpSecState->ConfirmedUserData(this, aFcb, arBuffer);
}

void LinkLayer::UnconfirmedUserData(bool aIsMaster, uint16_t aDest, uint16_t aSrc, const ReadOnlyBuffer& arBuffer)
{
	if(this->Validate(aIsMaster, aSrc, aDest))
		mpSecState->UnconfirmedUserData(this, arBuffer);
}

////////////////////////////////
// ILowerLayer
////////////////////////////////

void LinkLayer::_Send(const ReadOnlyBuffer& arBuffer)
{
	if(!mIsOnline)
		MACRO_THROW_EXCEPTION(InvalidStateException, "LowerLayerDown");
	if(mCONFIG.UseConfirms) mpPriState->SendConfirmed(this, arBuffer);
	else mpPriState->SendUnconfirmed(this, arBuffer);
}

void LinkLayer::OnTimeout()
{
	assert(mpTimer);
	mpTimer = nullptr;
	mpPriState->OnTimeout(this);
}

}

