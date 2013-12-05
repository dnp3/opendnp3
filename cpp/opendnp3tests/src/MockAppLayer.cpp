#include "MockAppLayer.h"

#include <openpal/LoggableMacros.h>
#include <openpal/ToHex.h>

using namespace openpal;

namespace opendnp3
{

MockAppLayer::MockAppLayer(openpal::Logger aLogger) :
	Loggable(aLogger),
	mNumCancel(0),
	mpUser(nullptr),
	mAutoSendCallback(true),
	mIsSuccess(true)
{

}

void MockAppLayer::SetUser(IAppUser* apUser)
{
	mpUser = apUser;
}

void MockAppLayer::EnableAutoSendCallback(bool aIsSuccess)
{
	mAutoSendCallback = true;
	mIsSuccess = aIsSuccess;
}

void MockAppLayer::DisableAutoSendCallback()
{
	mAutoSendCallback = false;
}

void MockAppLayer::DoSendUnsol()
{
	if(mAutoSendCallback) {
		assert(mpUser != nullptr);
		if(mIsSuccess) mpUser->OnUnsolSendSuccess();
		else mpUser->OnUnsolFailure();
	}
}

void MockAppLayer::DoSendSol()
{
	if(mAutoSendCallback) {
		assert(mpUser != nullptr);
		if(mIsSuccess) mpUser->OnSolSendSuccess();
		else mpUser->OnSolFailure();
	}
}

void MockAppLayer::SendResponse(APDU& arAPDU)
{
	LOG_BLOCK(LogLevel::Comm, "=> " << toHex(arAPDU.ToReadOnly(), true));
	LOG_BLOCK(LogLevel::Interpret, "=> " << arAPDU.ToString());
	mFragments.push_back(arAPDU);
	this->DoSendSol();

}

void MockAppLayer::SendUnsolicited(APDU& arAPDU)
{
	LOG_BLOCK(LogLevel::Comm, "=> " << toHex(arAPDU.ToReadOnly(), true));
	LOG_BLOCK(LogLevel::Interpret, "=> " << arAPDU.ToString());
	mFragments.push_back(arAPDU);
	this->DoSendUnsol();
}

void MockAppLayer::SendRequest(APDU& arAPDU)
{
	LOG_BLOCK(LogLevel::Comm, "=> " << toHex(arAPDU.ToReadOnly(), true));
	LOG_BLOCK(LogLevel::Interpret, "=> " << arAPDU.ToString());
	mFragments.push_back(arAPDU);
}

bool MockAppLayer::NothingToRead()
{
	return mFragments.size() == 0;
}

APDU MockAppLayer::Read()
{
	if(mFragments.size() == 0) throw InvalidStateException(LOCATION, "no more fragments");
	APDU frag = mFragments.front();
	frag.Interpret();
	mFragments.pop_front();
	return frag;
}

FunctionCode MockAppLayer::ReadFunction()
{
	if(mFragments.size() == 0) throw InvalidStateException(LOCATION, "No more fragments");
	else {
		FunctionCode func = mFragments.front().GetFunction();
		mFragments.pop_front();
		return func;
	}
}

void MockAppLayer::CancelResponse()
{
	++mNumCancel;
}

}

