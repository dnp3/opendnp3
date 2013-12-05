#ifndef __TRANSPORT_INTEGRATION_STACK_H_
#define __TRANSPORT_INTEGRATION_STACK_H_

#include <opendnp3/LinkLayerRouter.h>
#include <opendnp3/LinkLayer.h>
#include <opendnp3/TransportLayer.h>

#include "MockUpperLayer.h"

namespace openpal
{
class IPhysicalLayerAsync;
}

namespace opendnp3
{

class TransportIntegrationStack
{
public:
	TransportIntegrationStack(openpal::Logger, openpal::IPhysicalLayerAsync*, LinkConfig);

	LinkLayerRouter mRouter;
	LinkLayer mLink;
	TransportLayer mTransport;
	MockUpperLayer mUpper;
};

}

#endif

