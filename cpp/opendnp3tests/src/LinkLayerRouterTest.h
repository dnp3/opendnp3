#ifndef __LINK_LAYER_ROUTER_TEST_H_
#define __LINK_LAYER_ROUTER_TEST_H_

#include "LogTester.h"
#include "MockExecutor.h"
#include "MockPhysicalLayerAsync.h"

#include <opendnp3/LinkLayerRouter.h>


namespace opendnp3
{

class LinkLayerRouterTest
{
public:
	LinkLayerRouterTest(openpal::LogLevel aLevel = openpal::LogLevel::Warning, bool aImmediate = false);

	LogTester log;
	MockExecutor exe;
	MockPhysicalLayerAsync phys;
	LinkLayerRouter router;
};

}

#endif

