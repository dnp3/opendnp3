#include "TransportLoopbackTestObject.h"

#include <opendnp3/LinkRoute.h>

#include <openpal/IPhysicalLayerAsync.h>

#include <boost/asio.hpp>
#include <sstream>

using namespace std;
using namespace openpal;

namespace opendnp3
{

TransportLoopbackTestObject::TransportLoopbackTestObject(
        boost::asio::io_service* apService,
        IPhysicalLayerAsync* apPhys,
        LinkConfig aCfgA,
        LinkConfig aCfgB,
        LogLevel aLevel,
        bool aImmediate) :

	AsyncTestObjectASIO(apService),
	log(),
	mLogger(Logger(&log, aLevel, "test")),
	mCfgA(aCfgA),
	mCfgB(aCfgB),
	mLinkA(mLogger, apPhys->GetExecutor(), aCfgA),
	mLinkB(mLogger, apPhys->GetExecutor(), aCfgB),
	mTransA(mLogger),
	mTransB(mLogger),
	mRouter(mLogger, apPhys, TimeDuration::Seconds(1)),
	mUpperA(mLogger),
	mUpperB(mLogger)
{
	LinkRoute routeA(mCfgA.RemoteAddr, mCfgA.LocalAddr);
	LinkRoute routeB(mCfgB.RemoteAddr, mCfgB.LocalAddr);

	mRouter.AddContext(&mLinkA, routeA);
	mRouter.EnableRoute(routeA);
	mRouter.AddContext(&mLinkB, routeB);
	mRouter.EnableRoute(routeB);

	mLinkA.SetUpperLayer(&mTransA);
	mLinkB.SetUpperLayer(&mTransB);

	mLinkA.SetRouter(&mRouter);
	mLinkB.SetRouter(&mRouter);

	mTransA.SetUpperLayer(&mUpperA);
	mTransB.SetUpperLayer(&mUpperB);
}

TransportLoopbackTestObject::~TransportLoopbackTestObject()
{
	mRouter.Shutdown();
	this->GetService()->run();
}

bool TransportLoopbackTestObject::LayersUp()
{
	return mUpperA.IsLowerLayerUp() && mUpperB.IsLowerLayerUp();
}

void TransportLoopbackTestObject::Start()
{
	mRouter.Start();
}

}


