
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
#ifndef __MASTER_TEST_OBJECT_H_
#define __MASTER_TEST_OBJECT_H_

#include "MockExecutor.h"
#include "LogTester.h"

#include <opendnp3/AsyncTaskScheduler.h>
#include <opendnp3/Master.h>

#include <deque>

#include "FlexibleDataObserver.h"
#include "MockAppLayer.h"

namespace opendnp3
{


struct MasterConfig;

class MasterTestObject : public LogTester
{
public:
	MasterTestObject(MasterConfig, FilterLevel aLevel = LEV_INFO, bool aImmediate = false);

	void RespondToMaster(const std::string& arData, bool aFinal = true);
	void SendUnsolToMaster(const std::string& arData);
	std::string Read();

	void BindStateListener();

	MockTimeSource fake_time;
	MockExecutor mts;
	AsyncTaskScheduler ats;
	FlexibleDataObserver fdo;
	MockAppLayer app;
	Master master;
	APDU mAPDU;
	std::deque<StackState> states;
};

}

#endif

