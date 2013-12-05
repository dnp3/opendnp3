#ifndef __ASYNC_PHYS_TEST_OBJECT_H_
#define __ASYNC_PHYS_TEST_OBJECT_H_

#include "AsyncTestObjectASIO.h"
#include "MockUpperLayer.h"
#include "LogTester.h"

#include <asiopal/PhysicalLayerAsyncTCPClient.h>
#include <asiopal/PhysicalLayerAsyncTCPServer.h>

#include "LowerLayerToPhysAdapter.h"

namespace opendnp3
{

class AsyncPhysTestObject : public AsyncTestObjectASIO
{
public:
	AsyncPhysTestObject(openpal::LogLevel aLevel = openpal::LogLevel::Info, bool aImmediate = false, bool aAutoRead = true);

private:
	LogTester log;
	openpal::Logger logger;

public:
	asiopal::PhysicalLayerAsyncTCPClient mTCPClient;
	asiopal::PhysicalLayerAsyncTCPServer mTCPServer;

	LowerLayerToPhysAdapter mClientAdapter;
	LowerLayerToPhysAdapter mServerAdapter;

	MockUpperLayer mClientUpper;
	MockUpperLayer mServerUpper;
};

}

#endif
