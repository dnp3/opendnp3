#include "AsyncPhysBaseTest.h"

using namespace openpal;

namespace opendnp3
{

AsyncPhysBaseTest::AsyncPhysBaseTest(LogLevel aLevel, bool aImmediate) :
	log(),
	exe(),
	phys(Logger(&log, aLevel, "phys"), &exe),
	adapter(Logger(&log, aLevel, "adapter"), &phys, false),
	upper(Logger(&log, aLevel, "upper"))
{
	adapter.SetUpperLayer(&upper);
}


}


