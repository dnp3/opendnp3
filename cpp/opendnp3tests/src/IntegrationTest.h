/**
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */
#ifndef __INTEGRATION_TEST_H_
#define __INTEGRATION_TEST_H_

#include <asiodnp3/DNP3Manager.h>

#include <opendnp3/LogLevels.h>

#include <asiopal/Log.h>
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

	IntegrationTest(uint16_t aStartPort, size_t aNumPairs, uint16_t aNumPoints, uint32_t filters = levels::NORMAL);

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
	void AddStackPair(uint32_t filters, uint16_t aNumPoints);

	std::vector< std::shared_ptr<ComparingDataObserver> > mMasterObservers;
	FanoutDataObserver mFanout;

	const uint16_t M_START_PORT;

	FlexibleDataObserver mLocalFDO;
	MockCommandHandler mCmdHandler;

	asiopal::EventLog mLog;
	asiopal::IOServiceThreadPool mPool;
	asiodnp3::DNP3Manager mMgr;
	uint16_t NUM_POINTS;
};

}



#endif

