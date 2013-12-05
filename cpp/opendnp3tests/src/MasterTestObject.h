#ifndef __MASTER_TEST_OBJECT_H_
#define __MASTER_TEST_OBJECT_H_

#include "MockExecutor.h"
#include "LogTester.h"

#include <opendnp3/Master.h>
#include <opendnp3/AsyncTaskGroup.h>

#include <deque>

#include "MockMeasurementHandler.h"
#include "MockAppLayer.h"

namespace opendnp3
{


struct MasterConfig;

class MasterTestObject
{
public:
	MasterTestObject(MasterConfig, openpal::LogLevel aLevel = openpal::LogLevel::Info, bool aImmediate = false);

	void RespondToMaster(const std::string& arData, bool aFinal = true);
	void SendUnsolToMaster(const std::string& arData);
	std::string Read();

	void BindStateListener();

	LogTester log;
	openpal::FixedUTCTimeSource fixedUTC;
	MockExecutor mts;
	AsyncTaskGroup group;
	MockMeasurementHandler meas;
	MockAppLayer app;
	Master master;
	APDU mAPDU;
	std::deque<StackState> states;
};

}

#endif

