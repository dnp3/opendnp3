

#include "LinkStatus.h"

namespace opendnp3
{
	char const* LinkStatusToString(LinkStatus arg)
	{
	  switch(arg)
	  {
		case(LinkStatus::UNRESET):
		  return "UNRESET";
		case(LinkStatus::RESET):
		  return "RESET";
	  }
	  return "UNRESET";
	}
}
