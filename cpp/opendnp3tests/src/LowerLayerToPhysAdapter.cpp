#include "LowerLayerToPhysAdapter.h"

#include <openpal/IPhysicalLayerAsync.h>

using namespace openpal;

namespace opendnp3
{

LowerLayerToPhysAdapter::LowerLayerToPhysAdapter(Logger aLogger, IPhysicalLayerAsync* apPhys, bool aAutoRead) :
	Loggable(aLogger),
	IHandlerAsync(aLogger),
	ILowerLayer(aLogger),
	mAutoRead(aAutoRead),
	mNumOpenFailure(0),
	mpPhys(apPhys)
{
	mpPhys->SetHandler(this);
}

LowerLayerToPhysAdapter::~LowerLayerToPhysAdapter()
{

}

void LowerLayerToPhysAdapter::StartRead()
{
	WriteBuffer buffer(mpBuff, BUFFER_SIZE);
	mpPhys->AsyncRead(buffer);
}

/* Implement IAsyncHandler */
void LowerLayerToPhysAdapter::_OnOpenFailure()
{
	++mNumOpenFailure;
}

/* Implement IUpperLayer */
void LowerLayerToPhysAdapter::_OnReceive(const openpal::ReadOnlyBuffer& arBuffer)
{
	// process the data into another buffer *before* kicking off another call,
	// otherwise you have a potential race condition
	if(mpUpperLayer != nullptr) mpUpperLayer->OnReceive(arBuffer);
	if(mAutoRead) this->StartRead();
}

void LowerLayerToPhysAdapter::_OnSendSuccess()
{
	if(mpUpperLayer != nullptr) mpUpperLayer->OnSendSuccess();
}

void LowerLayerToPhysAdapter::_OnSendFailure()
{
	if(mpUpperLayer != nullptr) mpUpperLayer->OnSendFailure();
}

void LowerLayerToPhysAdapter::_OnLowerLayerUp()
{
	if(mAutoRead) this->StartRead();
	if(mpUpperLayer != nullptr) mpUpperLayer->OnLowerLayerUp();
}

void LowerLayerToPhysAdapter::_OnLowerLayerDown()
{
	if(mpUpperLayer != nullptr) mpUpperLayer->OnLowerLayerDown();
}

void LowerLayerToPhysAdapter::_OnLowerLayerShutdown()
{

}

/* Implement ILowerLayer */
void LowerLayerToPhysAdapter::_Send(const openpal::ReadOnlyBuffer& arBuffer)
{	
	mpPhys->AsyncWrite(arBuffer);
}

}//end namespace
