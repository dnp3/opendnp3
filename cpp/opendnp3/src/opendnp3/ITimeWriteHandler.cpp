#include <opendnp3/ITimeWriteHandler.h>

namespace opendnp3
{

NullTimeWriteHandler NullTimeWriteHandler::mInstance;

ITimeWriteHandler* NullTimeWriteHandler::Inst()
{
	return &mInstance;
}

}


