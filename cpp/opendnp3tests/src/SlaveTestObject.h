#ifndef __SLAVE_TEST_OBJECT_H_
#define __SLAVE_TEST_OBJECT_H_

#include <opendnp3/Slave.h>
#include <opendnp3/Database.h>
#include <opendnp3/SlaveConfig.h>

#include "MockExecutor.h"
#include "LogTester.h"
#include "MockCommandHandler.h"
#include "MockAppLayer.h"
#include "MockTimeWriteHandler.h"

namespace opendnp3
{

class SlaveTestObject
{
public:
	SlaveTestObject(const SlaveConfig& arCfg, openpal::LogLevel aLevel = openpal::LogLevel::Info, bool aImmediate = false);

	void SendToSlave(const std::string& arData, SequenceInfo aSeq = SI_OTHER);

	std::string Read();

	bool NothingToRead();

	size_t Count() {
		return app.Count();
	}


	LogTester log;
	MockTimeWriteHandler mMockTimeWriteHandler;
	MockExecutor mts;
	MockAppLayer app;
	Database db;
	MockCommandHandler cmdHandler;
	Slave slave;
	APDU mAPDU;
	openpal::Logger mLogger;

	std::queue<openpal::UTCTimestamp> mTimeWrites;

};

}

#endif

