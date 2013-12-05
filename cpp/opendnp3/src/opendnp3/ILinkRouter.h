#ifndef __I_LINK_ROUTER_H_
#define __I_LINK_ROUTER_H_

#include <openpal/Visibility.h>

namespace opendnp3
{

class LinkFrame;

// @section DESCRIPTION Interface from the link layer to the link router
class DLL_LOCAL ILinkRouter
{
public:

	virtual ~ILinkRouter() {}

	virtual bool Transmit(const LinkFrame&) = 0;
};

}

#endif

