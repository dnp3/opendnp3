#ifndef __TRANSPORT_LOOPBACK_TEST_OBJECT_H_
#define __TRANSPORT_LOOPBACK_TEST_OBJECT_H_

#include "AsyncTestObjectASIO.h"
#include "LogTester.h"
#include "MockUpperLayer.h"

#include <opendnp3/LinkLayerRouter.h>
#include <opendnp3/LinkLayer.h>
#include <opendnp3/TransportLayer.h>

namespace opendnp3
{


class TransportLoopbackTestObject :  public AsyncTestObjectASIO
{
public:
	TransportLoopbackTestObject(
	        boost::asio::io_service*,
	        openpal::IPhysicalLayerAsync*,
	        LinkConfig,
	        LinkConfig,
	        openpal::LogLevel aLevel = openpal::LogLevel::Info,
	        bool aImmediate = false);

	~TransportLoopbackTestObject();

	openpal::Logger& GetLogger() {
		return mLogger;
	}

	bool LayersUp();

	void Start();

	LogTester log;

private:
	openpal::Logger mLogger;


	LinkConfig mCfgA;
	LinkConfig mCfgB;

	LinkLayer mLinkA;
	LinkLayer mLinkB;
	TransportLayer mTransA;
	TransportLayer mTransB;
	LinkLayerRouter mRouter;

public:
	MockUpperLayer mUpperA;
	MockUpperLayer mUpperB;

};

}

#endif
