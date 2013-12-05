#ifndef __LINK_RECEIVER_TEST_H_
#define __LINK_RECEIVER_TEST_H_

#include <opendnp3/LinkLayerReceiver.h>

#include "LogTester.h"
#include "BufferHelpers.h"
#include "MockFrameSink.h"

namespace opendnp3
{

class LinkReceiverTest
{
public:
	LinkReceiverTest(openpal::LogLevel aLevel = openpal::LogLevel::Warning, bool aImmediate = false) :
		log(),
		mSink(),
		mRx(openpal::Logger(&log, aLevel, "ReceiverTest"), &mSink)
	{}

	void WriteData(const LinkFrame& arFrame) {
		auto buff = mRx.WriteBuff();
		assert(arFrame.GetSize() <= buff.Size());
		memcpy(buff, arFrame.GetBuffer(), arFrame.GetSize());
		mRx.OnRead(arFrame.GetSize());
	}

	void WriteData(const std::string& arHex) {
		HexSequence hs(arHex);
		auto buff = mRx.WriteBuff();
		assert(hs.Size() <= buff.Size());
		memcpy(buff, hs, hs.Size());
		mRx.OnRead(hs.Size());
	}

	LogTester log;
	MockFrameSink mSink;
	LinkLayerReceiver mRx;
};

}

#endif

