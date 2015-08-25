
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

	/// Called when a the reset/unreset status of the link layer changes
	virtual void OnStateChange(LinkStatus value) {}

	/// Called when the keep alive timer elapses. This doesn't denote a keep-alive failure, it's just a notification
	virtual void OnKeepAliveTimeout() {}
};

}

#endif
