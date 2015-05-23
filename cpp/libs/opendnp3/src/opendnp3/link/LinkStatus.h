

#ifndef OPENDNP3_LINKSTATUS_H
#define OPENDNP3_LINKSTATUS_H

namespace opendnp3
{
	enum class LinkStatus : int
	{
	  /// DOWN
	  UNRESET = 0,
	  /// UP
	  RESET = 1,
	};

	char const* LinkStatusToString(LinkStatus arg);
}

#endif
