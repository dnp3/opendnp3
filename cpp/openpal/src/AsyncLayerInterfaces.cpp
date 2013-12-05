#include <openpal/AsyncLayerInterfaces.h>

#include <openpal/Exception.h>
#include <openpal/LoggableMacros.h>
#include <openpal/ToHex.h>

#include <assert.h>

namespace openpal
{

//////////////////////////////////
// IUpDown
//////////////////////////////////

void IUpDown::OnLowerLayerUp()
{
	if(mIsLowerLayerUp) MACRO_THROW_EXCEPTION(InvalidStateException, "LowerLayerUp");
	mIsLowerLayerUp = true;
	this->_OnLowerLayerUp();
}

void IUpDown::OnLowerLayerDown()
{
	if(!mIsLowerLayerUp) MACRO_THROW_EXCEPTION(InvalidStateException, "LowerLayerDown");
	mIsLowerLayerUp = false;
	this->_OnLowerLayerDown();
}


//////////////////////////////////
// IUpperLayer
//////////////////////////////////
IUpperLayer::IUpperLayer(openpal::Logger& arLogger) :
	Loggable(arLogger),
	mpLowerLayer(nullptr)
{

}

void IUpperLayer::OnReceive(const ReadOnlyBuffer& arBuffer)
{
	if(this->LogReceive()) {
		LOG_BLOCK(LogLevel::Comm, RecvString() << " " << toHex(arBuffer, true));
	}
	this->_OnReceive(arBuffer); //call the implementation
}

void IUpperLayer::SetLowerLayer(ILowerLayer* apLowerLayer)
{
	assert(apLowerLayer != nullptr);
	mpLowerLayer = apLowerLayer;
}

void IUpperLayer::OnSendSuccess()
{
	this->_OnSendSuccess();
}

void IUpperLayer::OnSendFailure()
{
	this->_OnSendFailure();
}

//////////////////////////////////
// ILowerLayer
//////////////////////////////////

ILowerLayer::ILowerLayer(openpal::Logger& arLogger) :
	Loggable(arLogger),
	mpUpperLayer(nullptr)
{

}

void ILowerLayer::Send(const ReadOnlyBuffer& arBuffer)
{
	LOG_BLOCK(LogLevel::Comm, SendString() << " " << toHex(arBuffer, true));
	this->_Send(arBuffer);
}

void ILowerLayer::SetUpperLayer(IUpperLayer* apUpperLayer)
{
	assert(apUpperLayer != nullptr);
	mpUpperLayer = apUpperLayer;
	apUpperLayer->SetLowerLayer(this);
}

}//end namespace
