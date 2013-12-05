#ifndef __APPLICATION_STACK_H_
#define __APPLICATION_STACK_H_

#include <openpal/Visibility.h>

#include "LinkLayer.h"
#include "TransportLayer.h"
#include "AppLayer.h"

namespace openpal
{
class IExecutoer;
}

namespace opendnp3
{

class DLL_LOCAL ApplicationStack
{
public:
	ApplicationStack(openpal::Logger aLogger, openpal::IExecutor* apExecutor, AppConfig aAppCfg, LinkConfig aCfg);

	LinkLayer mLink;
	TransportLayer mTransport;
	AppLayer mApplication;
};

}

#endif
