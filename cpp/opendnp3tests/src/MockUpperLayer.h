#ifndef __MOCK_UPPER_LAYER_H_
#define __MOCK_UPPER_LAYER_H_

#include <openpal/AsyncLayerInterfaces.h>

#include <functional>

#include "BufferTestObject.h"

namespace opendnp3
{

class MockUpperLayer : public openpal::IUpperLayer, public BufferTestObject
{
public:

	typedef std::function<void (const openpal::ReadOnlyBuffer&)> OnReceiveHandler;

	struct State {

		State() {
			Reset();
		}

		size_t mSuccessCnt;
		size_t mFailureCnt;
		size_t mNumLayerUp;
		size_t mNumLayerDown;

		void Reset() {
			mSuccessCnt = mFailureCnt = mNumLayerUp = mNumLayerDown = 0;
		}
	};

	MockUpperLayer(openpal::Logger);
	virtual ~MockUpperLayer() {}

	void SendDown(const std::string&);
	void SendDown(const openpal::ReadOnlyBuffer& arBuffer);

	bool CountersEqual(size_t success, size_t failure) {
		return mState.mSuccessCnt == success && mState.mFailureCnt == failure;
	}

	bool StateEquals(const State& s) {
		return (mState.mSuccessCnt == s.mSuccessCnt)
		       && (mState.mFailureCnt == s.mFailureCnt)
		       && (mState.mNumLayerUp == s.mNumLayerUp)
		       && (mState.mNumLayerDown == s.mNumLayerDown);
	}

	void Reset() {
		mState.Reset();
	}
	State GetState() {
		return mState;
	}

	void SetReceiveHandler(const OnReceiveHandler& arHandler) {
		mOnReceiveHandler = arHandler;
	}

private:

	virtual std::string RecvString() const {
		return "MockUpperLayer <-";
	}

	OnReceiveHandler mOnReceiveHandler;
	State mState;

	//these are the NVII delegates
	void _OnReceive(const openpal::ReadOnlyBuffer& arBuffer);
	void _OnSendSuccess();
	void _OnSendFailure();
	void _OnLowerLayerUp();
	void _OnLowerLayerDown();
};



}

#endif
