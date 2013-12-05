#include "LinkLayerRouterTest.h"

using namespace openpal;

namespace opendnp3
{

LinkLayerRouterTest::LinkLayerRouterTest(LogLevel aLevel, bool aImmediate) :
	log(),
	exe(),
	phys(Logger(&log, aLevel, "Physical"), &exe),
	router(Logger(&log, aLevel, "Router"), &phys, TimeDuration::Milliseconds(100))
{

}

}

