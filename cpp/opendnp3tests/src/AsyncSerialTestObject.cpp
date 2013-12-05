#include "AsyncSerialTestObject.h"
#include <boost/asio.hpp>

using namespace openpal;
using namespace asiopal;

namespace opendnp3
{

AsyncSerialTestObject::AsyncSerialTestObject(SerialSettings cfg, LogLevel aLevel, bool aImmediate) :
	AsyncTestObjectASIO(),
	log(),
	mPort(Logger(&log, aLevel, "Serial"), this->GetService(), cfg),
	mAdapter(Logger(&log, aLevel, "Adapter"), &mPort, true),
	mUpper(Logger(&log, aLevel, "MockUpper"))
{
	mAdapter.SetUpperLayer(&mUpper);
}

}
