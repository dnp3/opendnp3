#include <boost/test/unit_test.hpp>

#include <opendnp3/PhysicalLayerMonitor.h>
#include <opendnp3/CopyableBuffer.h>

#include <queue>

namespace opendnp3
{

class MockPhysicalLayerMonitor : public PhysicalLayerMonitor
{
public:
	MockPhysicalLayerMonitor(
	        openpal::Logger& arLogger,
	        openpal::IPhysicalLayerAsync* apPhys,
	        openpal::TimeDuration aMinOpenRetry,
	        openpal::TimeDuration aMaxOpenRetry
	);

	size_t mOpens;
	size_t mCloses;

	size_t mNumReads;
	size_t mBytesRead;
	size_t mBytesWritten;
	size_t mLastWriteSize;

	CopyableBuffer mReadBuffer;
	CopyableBuffer mWriteBuffer;
	CopyableBuffer mExpectReadBuffer;

	void OnPhysicalLayerOpenSuccessCallback();
	void OnPhysicalLayerOpenFailureCallback() {}
	void OnPhysicalLayerCloseCallback();

	void _OnReceive(const uint8_t* apData, size_t aNumBytes);
	void _OnSendSuccess(void);
	void _OnSendFailure(void);

	void WriteData(const CopyableBuffer& arData);
	void ExpectData(const CopyableBuffer& arData);

	bool NextStateIs(ChannelState aState);
	bool AllExpectedDataHasBeenReceived();

protected:
	void OnStateChange(ChannelState);
	std::queue< ChannelState > mState;

private:

	void TransmitNext();

};

}
