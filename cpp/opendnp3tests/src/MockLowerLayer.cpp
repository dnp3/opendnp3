#include "MockLowerLayer.h"

#include "BufferHelpers.h"
#include <assert.h>

using namespace openpal;

namespace opendnp3
{

MockLowerLayer::MockLowerLayer(Logger aLogger) :
	Loggable(aLogger),
	ILowerLayer(aLogger),
	mAutoSendCallback(true),
	mIsSuccess(true)
{

}


void MockLowerLayer::EnableAutoSendCallback(bool aIsSuccess)
{
	mAutoSendCallback = true;
	mIsSuccess = aIsSuccess;
}

void MockLowerLayer::DisableAutoSendCallback()
{
	mAutoSendCallback = false;
}

void MockLowerLayer::_Send(const openpal::ReadOnlyBuffer& arBuffer)
{
	this->WriteToBuffer(arBuffer);
	if(mAutoSendCallback && mpUpperLayer != nullptr) {
		if(mIsSuccess) mpUpperLayer->OnSendSuccess();
		else mpUpperLayer->OnSendFailure();
	}
}

void MockLowerLayer::SendUp(const openpal::ReadOnlyBuffer& arBuffer)
{
	if(this->mpUpperLayer != nullptr)
		mpUpperLayer->OnReceive(arBuffer);
}

void MockLowerLayer::SendUp(const std::string& arHexData)
{
	HexSequence hs(arHexData);
	this->SendUp(hs.ToReadOnly());
}

void MockLowerLayer::SendSuccess()
{
	if(mpUpperLayer != nullptr) mpUpperLayer->OnSendSuccess();
}

void MockLowerLayer::SendFailure()
{
	if(mpUpperLayer != nullptr) mpUpperLayer->OnSendFailure();
}

void MockLowerLayer::ThisLayerUp()
{
	if(mpUpperLayer != nullptr) mpUpperLayer->OnLowerLayerUp();
}
void MockLowerLayer::ThisLayerDown()
{
	if(mpUpperLayer != nullptr) mpUpperLayer->OnLowerLayerDown();
}

}
