#ifndef __MOCK_FRAME_SINK_H_
#define __MOCK_FRAME_SINK_H_

#include <opendnp3/ILinkContext.h>
#include <opendnp3/LinkLayerConstants.h>

#include "BufferTestObject.h"

#include <opendnp3/gen/LinkFunction.h>

#include <functional>
#include <queue>

namespace opendnp3
{


class MockFrameSink : public ILinkContext, public BufferTestObject
{
public:

	MockFrameSink();

	// ILinkContext members
	void OnLowerLayerUp();
	void OnLowerLayerDown();

	//	Sec to Pri
	void Ack(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc);
	void Nack(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc);
	void LinkStatus(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc);
	void NotSupported (bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc);

	//	Pri to Sec

	void TestLinkStatus(bool aIsMaster, bool aFcb, uint16_t aDest, uint16_t aSrc);
	void ResetLinkStates(bool aIsMaster, uint16_t aDest, uint16_t aSrc);
	void RequestLinkStatus(bool aIsMaster, uint16_t aDest, uint16_t aSrc);
	void ConfirmedUserData(bool aIsMaster, bool aFcb, uint16_t aDest, uint16_t aSrc, const openpal::ReadOnlyBuffer&);
	void UnconfirmedUserData(bool aIsMaster, uint16_t aDest, uint16_t aSrc, const openpal::ReadOnlyBuffer&);

	void Reset();

	size_t mNumFrames;

	bool CheckLast(LinkFunction aCode, bool aIsMaster, uint16_t aDest, uint16_t aSrc);
	bool CheckLastWithFCB(LinkFunction aCode, bool aIsMaster, bool aFcb, uint16_t aDest, uint16_t aSrc);
	bool CheckLastWithDFC(LinkFunction aCode, bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc);

	// Last frame information
	LinkFunction mCode;
	bool mIsMaster;
	bool mIsRcvBuffFull;
	uint16_t mSrc;
	uint16_t mDest;
	bool mFcb;

	bool mLowerOnline;

	// Add a function to execute the next time a frame is received
	// This allows us to test re-entrant behaviors
	void AddAction(std::function<void ()> aFunc);

private:

	// Executes one action, if one is available
	void ExecuteAction();

	std::deque< std::function<void ()> > mActions;

	void Update(LinkFunction aCode, bool aIsMaster, uint16_t aSrc, uint16_t aDest);
};

}

#endif

