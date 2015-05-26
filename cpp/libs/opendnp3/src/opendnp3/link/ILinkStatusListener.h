
#ifndef OPENDNP3_ILINKSTATUSLISTENER_H
#define OPENDNP3_ILINKSTATUSLISTENER_H

#include <opendnp3/gen/LinkStatus.h>

namespace opendnp3
{

class ILinkStatusListener
{
	public:	
	virtual void OnStateChange(LinkStatus value) = 0;
};

}

#endif
