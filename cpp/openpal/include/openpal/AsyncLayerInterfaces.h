#ifndef __ASYNC_LAYER_INTERFACES_H_
#define __ASYNC_LAYER_INTERFACES_H_

#include <openpal/Loggable.h>

#include "BufferWrapper.h"

namespace openpal
{

class IUpDown
{
public:
	IUpDown() : mIsLowerLayerUp(false) {}
	virtual ~IUpDown() {}

	// Called by a 'LowerLayer' when it can start performing send services
	void OnLowerLayerUp();

	// Called by a 'LowerLayer' when it can no longer perform Tx services for this layer.
	// Implies previous sends have failed.
	void OnLowerLayerDown();

	bool IsLowerLayerUp() {
		return mIsLowerLayerUp;
	}

private:

private:
	bool mIsLowerLayerUp;

	virtual void _OnLowerLayerUp() = 0;
	virtual void _OnLowerLayerDown() = 0;
};


class ILowerLayer;

class IUpperLayer : public IUpDown, protected virtual openpal::Loggable
{
public:
	IUpperLayer(openpal::Logger&);
	virtual ~IUpperLayer() {}

	// Called by 'layer down' when data arrives
	void OnReceive(const ReadOnlyBuffer&);

	// Called by 'layer down' when a previously requested send operation succeeds
	// Layers can only have 1 outstanding send operation. The callback is guaranteed
	// unless the the OnLowerLayerDown() function is called before
	void OnSendSuccess();

	// Called by 'layer down' when a previously requested send operation fails
	void OnSendFailure();

	void SetLowerLayer(ILowerLayer*);


protected:

	ILowerLayer* mpLowerLayer;

	//these are the NVII delegates
	virtual void _OnReceive(const ReadOnlyBuffer&) = 0;
	virtual void _OnSendSuccess() = 0;
	virtual void _OnSendFailure() = 0;
	virtual bool LogReceive() {
		return true;
	}

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
	// override this descriptor, it's use in the Hex log messages
	virtual std::string RecvString() const {
		return "<-";
	}
#endif

};

class ILowerLayer : protected virtual openpal::Loggable
{
public:
	ILowerLayer(openpal::Logger&);
	virtual ~ILowerLayer() {}

	void Send(const ReadOnlyBuffer&);

	void SetUpperLayer(IUpperLayer*);

protected:

	IUpperLayer* mpUpperLayer;

private:

	virtual void _Send(const ReadOnlyBuffer&) = 0;

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
	// override this descriptor, it's use in the Hex log messages
	virtual std::string SendString() const {
		return "->";
	}
#endif

};

}

#endif
