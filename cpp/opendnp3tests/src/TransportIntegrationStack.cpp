#include "TransportIntegrationStack.h"

#include <opendnp3/LinkRoute.h>

#include <openpal/IPhysicalLayerAsync.h>

using namespace openpal;

namespace opendnp3
{

TransportIntegrationStack::TransportIntegrationStack(Logger aLogger, IPhysicalLayerAsync* apPhys, LinkConfig aCfg) :
	mRouter(aLogger, apPhys, TimeDuration::Seconds(1)),
	mLink(aLogger, apPhys->GetExecutor(), aCfg),
	mTransport(aLogger),
	mUpper(aLogger)
{
	LinkRoute route(aCfg.RemoteAddr, aCfg.LocalAddr);
	mRouter.AddContext(&mLink, route);
	mRouter.EnableRoute(route);
	mLink.SetUpperLayer(&mTransport);
	mTransport.SetUpperLayer(&mUpper);
	mLink.SetRouter(&mRouter);
}


}
