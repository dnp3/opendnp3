#include "ApplicationStack.h"

using namespace openpal;

namespace opendnp3
{

ApplicationStack::ApplicationStack(openpal::Logger aLogger, openpal::IExecutor* apExecutor, AppConfig aAppCfg, LinkConfig aCfg) :
	mLink(aLogger.GetSubLogger("link"), apExecutor, aCfg),
	mTransport(aLogger.GetSubLogger("transport")),
	mApplication(aLogger.GetSubLogger("app"), apExecutor, aAppCfg)
{
	mLink.SetUpperLayer(&mTransport);
	mTransport.SetUpperLayer(&mApplication);
}

}
