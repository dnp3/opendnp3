#ifndef __PHYS_LOOPBACK_H_
#define __PHYS_LOOPBACK_H_

#include <opendnp3/PhysicalLayerMonitor.h>
#include <opendnp3/CopyableBuffer.h>

namespace opendnp3
{

/**
*	Buffers and sends all bytes received to back on the same layer.
*/
class PhysLoopback : public PhysicalLayerMonitor
{
public:
	PhysLoopback(openpal::Logger, openpal::IPhysicalLayerAsync*);

private:

	size_t mBytesReadWritten;	

	CopyableBuffer mBuffer;

	void _OnReceive(const openpal::ReadOnlyBuffer&);
	void _OnSendSuccess(void);
	void _OnSendFailure(void);

	void OnPhysicalLayerOpenSuccessCallback(void);
	void OnPhysicalLayerOpenFailureCallback(void) {}
	void OnPhysicalLayerCloseCallback(void) {}

	void StartRead();
};

}

#endif
