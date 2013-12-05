#include "PhysLoopback.h"

#include <openpal/IPhysicalLayerAsync.h>

using namespace openpal;
using namespace std::chrono;

namespace opendnp3
{

PhysLoopback::PhysLoopback(openpal::Logger aLogger, openpal::IPhysicalLayerAsync* apPhys) :
	Loggable(aLogger),
	PhysicalLayerMonitor(aLogger, apPhys, openpal::TimeDuration::Seconds(5), openpal::TimeDuration::Seconds(5)),
	mBytesReadWritten(0),	
	mBuffer(1024)
{

}

void PhysLoopback::StartRead()
{
	WriteBuffer buffer(mBuffer, mBuffer.Size());
	mpPhys->AsyncRead(buffer);
}

void PhysLoopback::_OnReceive(const openpal::ReadOnlyBuffer& arBuffer)
{
	mBytesReadWritten += arBuffer.Size();
	mpPhys->AsyncWrite(arBuffer);
}

void PhysLoopback::_OnSendSuccess(void)
{
	this->StartRead();
}

void PhysLoopback::_OnSendFailure(void)
{

}

void PhysLoopback::OnPhysicalLayerOpenSuccessCallback(void)
{
	this->StartRead();
}

}

