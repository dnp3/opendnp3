#include "SlaveTestObject.h"

#include "BufferHelpers.h"

#include <openpal/ToHex.h>
#include <openpal/LoggableMacros.h>

#include <sstream>

using namespace std;
using namespace openpal;

namespace opendnp3
{

SlaveTestObject::SlaveTestObject(const SlaveConfig& arCfg, LogLevel aLevel, bool aImmediate) :
	log(),
	mMockTimeWriteHandler([this](UTCTimestamp time)
{
	mTimeWrites.push(time);
}),
mts(),
app(Logger(&log, aLevel, "app")),
db(Logger(&log, aLevel, "db")),
slave(Logger(&log, aLevel, "slave"), &app, &mts, &mMockTimeWriteHandler, &db, &cmdHandler, arCfg),
mLogger(Logger(&log, aLevel, "test"))
{
	app.SetUser(&slave);
}

void SlaveTestObject::SendToSlave(const std::string& arData, SequenceInfo aSeq)
{
	HexSequence hs(arData);
	mAPDU.Reset();
	mAPDU.Write(hs.ToReadOnly());
	mAPDU.Interpret();
	LOG_BLOCK(LogLevel::Interpret, "<= " << mAPDU.ToString());
	slave.OnRequest(mAPDU, aSeq);
}

bool SlaveTestObject::NothingToRead()
{
	if(app.NothingToRead()) return true;
	else {
		ostringstream oss;
		oss << "Expected nothing but outstation wrote: " << Read();
		throw InvalidStateException(LOCATION, oss.str());
	}
}

std::string SlaveTestObject::Read()
{
	mAPDU = app.Read();	
	return toHex(mAPDU.ToReadOnly(), true);
}

}

