#ifndef __I_LINK_CONTEXT_H_
#define __I_LINK_CONTEXT_H_

#include "IFrameSink.h"

#include <openpal/Visibility.h>

namespace opendnp3
{

// @section DESCRIPTION Interface from the link router to the link layer
class DLL_LOCAL ILinkContext : public IFrameSink
{
public:

	virtual ~ILinkContext() {}

	virtual void OnLowerLayerUp() = 0;
	virtual void OnLowerLayerDown() = 0;
};

}

#endif

