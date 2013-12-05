#include "LinkRoute.h"

#include <sstream>

namespace opendnp3
{

LinkRoute::LinkRoute(const uint16_t aRemoteAddr, const uint16_t aLocalAddr) :
	remote(aRemoteAddr),
	local(aLocalAddr)
{}

LinkRoute::LinkRoute() :
	remote(0),
	local(0)
{}

bool LinkRoute::LessThan::operator ()(const LinkRoute& a, const LinkRoute& b) const
{
	if(a.remote < b.remote) return true;
	else if(b.remote < a.remote) return false;
	else return a.local < b.local;
}

#ifndef OPENDNP3_STRIP_LOG_MESSAGES

std::string LinkRoute::ToString() const
{
	std::ostringstream oss;
	oss << " Local: " << this->local << " Remote: " << this->remote;	
	return oss.str();
}

#endif

}


