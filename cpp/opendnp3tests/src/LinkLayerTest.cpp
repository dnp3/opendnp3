#include "LinkLayerTest.h"

using namespace openpal;

namespace opendnp3
{

LinkLayerTest::LinkLayerTest(LinkConfig arCfg, LogLevel aLevel, bool aImmediate) :
	log(),
	mts(),
	upper(Logger(&log, aLevel, "MockUpperLayer")),
	link(Logger(&log, aLevel, "LinkLayer"), &mts, arCfg),
	mNumSend(0)
{
	link.SetUpperLayer(&upper);
	link.SetRouter(this);
}

bool LinkLayerTest::Transmit(const LinkFrame& arFrame)
{
	mLastSend = arFrame;
	++mNumSend;
	return true;
}

LinkConfig LinkLayerTest::DefaultConfig()
{
	LinkConfig cfg(true, false);
	return cfg;
}

}

