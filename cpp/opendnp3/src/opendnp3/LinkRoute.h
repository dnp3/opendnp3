#ifndef __LINK_ROUTE_H_
#define __LINK_ROUTE_H_

#include <openpal/Visibility.h>

#include <cstdint>
#include <string>

namespace opendnp3
{

/**
 * Immutable class that defines a route from a DNP3 source address to a
 * destination address.  Remote/Local used here instead of source/destination.
 *
 * When transmitting, destination = remote, source = local
 * When receiving, destination = local, soource = remote
 *
 * Primary used as a key for stl map/set.
 */
class DLL_LOCAL LinkRoute
{
public:
	LinkRoute(const uint16_t aRemoteAddr, const uint16_t aLocalAddr);

	LinkRoute();

	uint16_t remote;
	uint16_t local;

	// comparison functor for use with stl map/set
	struct LessThan {
		// Const to fix VS compilation bug
		bool operator()(const LinkRoute& a, const LinkRoute& b) const;
	};

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
	std::string ToString() const;
#endif

};


}

#endif

