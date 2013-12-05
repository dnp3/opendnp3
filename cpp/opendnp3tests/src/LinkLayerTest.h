#ifndef __LINK_LAYER_TEST_H_
#define __LINK_LAYER_TEST_H_

#include "LogTester.h"
#include "MockUpperLayer.h"
#include "MockExecutor.h"

#include <opendnp3/LinkLayer.h>
#include <opendnp3/ILinkRouter.h>

namespace opendnp3
{

class LinkLayerTest : public ILinkRouter
{
public:

	LinkLayerTest(LinkConfig arCfg = DefaultConfig(), openpal::LogLevel aLevel = openpal::LogLevel::Warning, bool aImmediate = false);

	//ILinkRouter interface
	bool Transmit(const LinkFrame&);

	static LinkConfig DefaultConfig();

	LogTester log;

	MockExecutor mts;
	MockUpperLayer upper;
	LinkLayer link;

	LinkFrame mLastSend;
	size_t mNumSend;
};

}

#endif

