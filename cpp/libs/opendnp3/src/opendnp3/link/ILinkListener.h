
#ifndef OPENDNP3_ILINK_LISTENER_H
#define OPENDNP3_ILINK_LISTENER_H

#include <opendnp3/gen/LinkStatus.h>

namespace opendnp3
{

/**
* Various optional callbacks that can be received for the link layer
*/
class ILinkListener
{
	public:

	/// overridable
	virtual void OnStateChange(LinkStatus value) {}
};

}

#endif
