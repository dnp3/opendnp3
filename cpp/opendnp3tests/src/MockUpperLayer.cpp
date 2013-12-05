#include "MockUpperLayer.h"

#include "BufferHelpers.h"

#include <openpal/ToHex.h>
#include <openpal/LoggableMacros.h>

#include <memory>

using namespace openpal;

namespace opendnp3
{

MockUpperLayer::MockUpperLayer(openpal::Logger aLogger) :
	Loggable(aLogger),
	IUpperLayer(aLogger)
{}

void MockUpperLayer::_OnReceive(const openpal::ReadOnlyBuffer& arBuffer)
{
	this->WriteToBuffer(arBuffer);
	if(mOnReceiveHandler) mOnReceiveHandler(arBuffer);
}

void MockUpperLayer::_OnSendSuccess()
{
	LOG_BLOCK(LogLevel::Debug, "OnSendSuccess");
	++mState.mSuccessCnt;
}

void MockUpperLayer::_OnSendFailure()
{
	LOG_BLOCK(LogLevel::Debug, "OnSendFailure");
	++mState.mFailureCnt;
}

void MockUpperLayer::_OnLowerLayerUp()
{
	LOG_BLOCK(LogLevel::Debug, "OnLowerLayerUp");
	++mState.mNumLayerUp;
}

void MockUpperLayer::_OnLowerLayerDown()
{
	LOG_BLOCK(LogLevel::Debug, "OnLowerLayerDown");
	++mState.mNumLayerDown;
}

void MockUpperLayer::SendDown(const openpal::ReadOnlyBuffer& arBuffer)
{
	if(this->mpLowerLayer) mpLowerLayer->Send(arBuffer);
}

void MockUpperLayer::SendDown(const std::string& arHexData)
{
	HexSequence hs(arHexData);	
	this->SendDown(hs.ToReadOnly());
}

}
