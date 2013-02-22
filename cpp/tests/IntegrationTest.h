
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the GNU Affero General Public License Version 3.0
// (the "Additional License"). You may not use these modifications except in
// compliance with the additional license. You may obtain a copy of the Additional
// License at
//
// http://www.gnu.org/licenses/agpl.html
//
// Contact Automatak, LLC for a commercial license to these modifications
//
#ifndef __INTEGRATION_TEST_H_
#define __INTEGRATION_TEST_H_

#include <opendnp3/DNP3Manager.h>

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

	IntegrationTest(FilterLevel aLevel, boost::uint16_t aStartPort, size_t aNumPairs, size_t aNumPoints);

	size_t IncrementData();

	bool WaitForSameData(millis_t aTimeout, bool aDescribeAnyMissingData);

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
	void AddStackPair(FilterLevel aLevel, size_t aNumPoints);

	std::vector< std::shared_ptr<ComparingDataObserver> > mMasterObservers;
	FanoutDataObserver mFanout;

	const uint16_t M_START_PORT;

	FlexibleDataObserver mLocalFDO;
	MockCommandHandler mCmdHandler;

	DNP3Manager mMgr;
	size_t NUM_POINTS;
};

}

/* vim: set ts=4 sw=4: */

#endif

