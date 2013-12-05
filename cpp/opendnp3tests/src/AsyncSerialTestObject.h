#ifndef __ASYNC_SERIAL_TEST_OBJECT_H_
#define __ASYNC_SERIAL_TEST_OBJECT_H_

#include <asiopal/PhysicalLayerAsyncSerial.h>

#include "LowerLayerToPhysAdapter.h"
#include "AsyncTestObjectASIO.h"
#include "LogTester.h"
#include "MockUpperLayer.h"

namespace opendnp3
{

class AsyncSerialTestObject : public AsyncTestObjectASIO
{
public:
	AsyncSerialTestObject(asiopal::SerialSettings cfg, openpal::LogLevel aLevel = openpal::LogLevel::Info, bool aImmediate = false);
	virtual ~AsyncSerialTestObject() {}

	LogTester log;
	asiopal::PhysicalLayerAsyncSerial mPort;
	LowerLayerToPhysAdapter mAdapter;
	MockUpperLayer mUpper;
};

}

#endif
