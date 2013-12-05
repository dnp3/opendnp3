#include "MockFrameSink.h"

using namespace openpal;

namespace opendnp3
{

MockFrameSink::MockFrameSink() : mNumFrames(0), mLowerOnline(false)
{}

void MockFrameSink::OnLowerLayerUp()
{
	mLowerOnline = true;
}

void MockFrameSink::OnLowerLayerDown()
{
	mLowerOnline = false;
}

void MockFrameSink::Reset()
{
	this->ClearBuffer();
	mNumFrames = 0;
}

bool MockFrameSink::CheckLast(LinkFunction aCode, bool aIsMaster, uint16_t aDest, uint16_t aSrc)
{
	return (mCode == aCode) && (aIsMaster == mIsMaster) && (mSrc == aSrc) && (mDest == aDest);
}

bool MockFrameSink::CheckLastWithFCB(LinkFunction aCode, bool aIsMaster, bool aFcb, uint16_t aDest, uint16_t aSrc)
{
	return (mFcb == aFcb) && CheckLast(aCode, aIsMaster, aDest, aSrc);
}

bool MockFrameSink::CheckLastWithDFC(LinkFunction aCode, bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc)
{
	return  (mIsRcvBuffFull == aIsRcvBuffFull) && CheckLast(aCode, aIsMaster, aDest, aSrc);
}

//	Sec to Pri

void MockFrameSink::Ack(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc)
{
	mIsRcvBuffFull = aIsRcvBuffFull;
	this->Update(LinkFunction::SEC_ACK, aIsMaster, aDest, aSrc);
}

void MockFrameSink::Nack(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc)
{
	mIsRcvBuffFull = aIsRcvBuffFull;
	this->Update(LinkFunction::SEC_NACK, aIsMaster, aDest, aSrc);
}

void MockFrameSink::LinkStatus(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc)
{
	mIsRcvBuffFull = aIsRcvBuffFull;
	this->Update(LinkFunction::SEC_LINK_STATUS, aIsMaster, aDest, aSrc);
}

void MockFrameSink::NotSupported (bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc)
{
	mIsRcvBuffFull = aIsRcvBuffFull;
	this->Update(LinkFunction::SEC_NOT_SUPPORTED, aIsMaster, aDest, aSrc);
}

//	Pri to Sec

void MockFrameSink::TestLinkStatus(bool aIsMaster, bool aFcb, uint16_t aDest, uint16_t aSrc)
{
	mFcb = aFcb;
	this->Update(LinkFunction::PRI_TEST_LINK_STATES, aIsMaster, aDest, aSrc);
}

void MockFrameSink::ResetLinkStates(bool aIsMaster, uint16_t aDest, uint16_t aSrc)
{
	this->Update(LinkFunction::PRI_RESET_LINK_STATES, aIsMaster, aDest, aSrc);
}

void MockFrameSink::RequestLinkStatus(bool aIsMaster, uint16_t aDest, uint16_t aSrc)
{
	this->Update(LinkFunction::PRI_REQUEST_LINK_STATUS, aIsMaster, aDest, aSrc);
}

void MockFrameSink::ConfirmedUserData(bool aIsMaster, bool aFcb, uint16_t aDest, uint16_t aSrc, const openpal::ReadOnlyBuffer& arBuffer)
{
	mFcb = aFcb;
	this->WriteToBuffer(arBuffer);
	this->Update(LinkFunction::PRI_CONFIRMED_USER_DATA, aIsMaster, aDest, aSrc);
}

void MockFrameSink::UnconfirmedUserData(bool aIsMaster, uint16_t aDest, uint16_t aSrc, const openpal::ReadOnlyBuffer& arBuffer)
{
	this->WriteToBuffer(arBuffer);
	this->Update(LinkFunction::PRI_UNCONFIRMED_USER_DATA, aIsMaster, aDest, aSrc);
}

void MockFrameSink::AddAction(std::function<void ()> aFunc)
{
	mActions.push_back(aFunc);
}

void MockFrameSink::ExecuteAction()
{
	if(mActions.size() > 0) {
		auto f = mActions.front();
		mActions.pop_front();
		f();
	}
}

void MockFrameSink::Update(LinkFunction aCode, bool aIsMaster, uint16_t aDest, uint16_t aSrc)
{
	++mNumFrames;
	mCode = aCode;
	mIsMaster = aIsMaster;
	mDest = aDest;
	mSrc = aSrc;
	this->ExecuteAction();
}

}

