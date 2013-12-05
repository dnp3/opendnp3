#ifndef __PHYSICAL_LAYER_WRAPPER_H_
#define __PHYSICAL_LAYER_WRAPPER_H_

#include <openpal/IPhysicalLayerAsync.h>
#include <openpal/IHandlerAsync.h>

#include "RandomDouble.h"

namespace opendnp3
{

class PhysicalLayerWrapper : public openpal::IPhysicalLayerAsync, public openpal::IHandlerAsync
{
public:
	PhysicalLayerWrapper(openpal::Logger aLogger, openpal::IPhysicalLayerAsync* apProxy);

	openpal::IExecutor* GetExecutor() {
		return mpProxy->GetExecutor();
	}

	bool CanOpen() const {
		return mpProxy->CanOpen();
	}
	bool CanClose() const {
		return mpProxy->CanClose();
	}
	bool CanRead() const {
		return mpProxy->CanRead();
	}
	bool CanWrite() const {
		return mpProxy->CanWrite();
	}

	bool IsReading() const {
		return mpProxy->IsReading();
	}
	bool IsWriting() const {
		return mpProxy->IsWriting();
	}
	bool IsClosing() const {
		return mpProxy->IsClosing();
	}
	bool IsClosed() const {
		return mpProxy->IsClosed();
	}
	bool IsOpening() const {
		return mpProxy->IsOpening();
	}
	bool IsOpen() const {
		return mpProxy->IsOpen();
	}

	std::string ConvertStateToString() const {
		return mpProxy->ConvertStateToString();
	}

	void AsyncOpen();
	void AsyncClose();
	void AsyncWrite(const openpal::ReadOnlyBuffer&);
	void AsyncRead(openpal::WriteBuffer&);

	void SetHandler(openpal::IHandlerAsync* apHandler);

	// testing helpers
	void SetCorruptionProbability(double aProbability);

private:
	void _OnLowerLayerUp();
	void _OnLowerLayerDown();
	void _OnReceive(const openpal::ReadOnlyBuffer& arBuffer);
	void _OnSendSuccess();
	void _OnSendFailure();
	void _OnOpenFailure();

	double mCorruptionProbability;
	RandomDouble mRandom;

private:

	openpal::IPhysicalLayerAsync* mpProxy;
	openpal::IHandlerAsync* mpHandler;
};

}

#endif
