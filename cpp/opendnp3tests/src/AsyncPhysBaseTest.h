#ifndef __ASYNC_PHYS_BASE_TEST_H_
#define __ASYNC_PHYS_BASE_TEST_H_

#include "LowerLayerToPhysAdapter.h"
#include "LogTester.h"
#include "MockUpperLayer.h"
#include "MockPhysicalLayerAsync.h"
#include "MockExecutor.h"

namespace opendnp3
{

class AsyncPhysBaseTest
{
public:
	AsyncPhysBaseTest(openpal::LogLevel aLevel = openpal::LogLevel::Info, bool aImmediate = false);

	LogTester log;
	MockExecutor exe;
	MockPhysicalLayerAsync phys;
	LowerLayerToPhysAdapter adapter;
	MockUpperLayer upper;
};

}

#endif
