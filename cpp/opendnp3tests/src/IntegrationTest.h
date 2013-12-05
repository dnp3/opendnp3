#ifndef __INTEGRATION_TEST_H_
#define __INTEGRATION_TEST_H_

#include <asiopal/Log.h>
#include <opendnp3/DNP3Manager.h>

#include <asiopal/IOServiceThreadPool.h>

#include "Random.h"
#include "FlexibleDataObserver.h"
#include "AsyncTestObjectASIO.h"
#include "LogTester.h"
#include "FanoutDataObserver.h"
#include "MockCommandHandler.h"
#include "ComparingDataObserver.h"

#include <vector>
#include <memory>

namespace opendnp3
{

class IntegrationTest
{
public:

	IntegrationTest(openpal::LogLevel aLevel, boost::uint16_t aStartPort, size_t aNumPairs, size_t aNumPoints);

	size_t IncrementData();

	bool WaitForSameData(openpal::TimeDuration aTimeout, bool aDescribeAnyMissingData);

private:

	void InitLocalObserver();

	void ResetObservers();

	Binary RandomBinary();
	Analog RandomAnalog();
	Counter RandomCounter();

	Binary Next(const Binary& arPoint);
	Analog Next(const Analog& arPoint);
	Counter Next(const Counter& arPoint);


	void RegisterChange();
	void AddStackPair(openpal::LogLevel aLevel, size_t aNumPoints);

	std::vector< std::shared_ptr<ComparingDataObserver> > mMasterObservers;
	FanoutDataObserver mFanout;

	const uint16_t M_START_PORT;

	FlexibleDataObserver mLocalFDO;
	MockCommandHandler mCmdHandler;

	asiopal::EventLog mLog;
	asiopal::IOServiceThreadPool mPool;
	DNP3Manager mMgr;
	size_t NUM_POINTS;
};

}

/* vim: set ts=4 sw=4: */

#endif

