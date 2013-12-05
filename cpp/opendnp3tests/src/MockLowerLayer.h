#ifndef __MOCK_LOWER_LAYER_H_
#define __MOCK_LOWER_LAYER_H_

#include <openpal/AsyncLayerInterfaces.h>

#include "BufferTestObject.h"

namespace opendnp3
{

class MockLowerLayer : public openpal::ILowerLayer, public BufferTestObject
{
public:
	MockLowerLayer(openpal::Logger);
	virtual ~MockLowerLayer() {}

	void SendUp(const openpal::ReadOnlyBuffer& arBuffer);
	void SendUp(const std::string&);
	void SendSuccess();
	void SendFailure();
	void ThisLayerUp();
	void ThisLayerDown();

	void EnableAutoSendCallback(bool aIsSuccess);
	void DisableAutoSendCallback();

private:

	bool mAutoSendCallback;
	bool mIsSuccess;

	virtual std::string SendString() const {
		return " MockLowerLayer ->";
	}

	//these are the NVII delegates
	void _Send(const openpal::ReadOnlyBuffer& arBuffer);
};

}

#endif
