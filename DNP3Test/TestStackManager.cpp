/*
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or more
 * contributor license agreements. See the NOTICE file distributed with this
 * work for additional information regarding copyright ownership.  Green Enery
 * Corp licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */

#include <boost/test/unit_test.hpp>
#include <APLTestTools/TestHelpers.h>

#include <DNP3/StackManager.h>

using namespace apl;
using namespace apl::dnp;
using namespace std;

BOOST_AUTO_TEST_SUITE(StackManagerTestSuite)

BOOST_AUTO_TEST_CASE(RemovePortWorksAfterAdd)
{
	StackManager sm;
	sm.AddTCPClient("client",PhysLayerSettings(), "127.0.0.1", 20000);
	sm.RemovePort("client");
}

BOOST_AUTO_TEST_CASE(AddRemoveMultipleTimes)
{
	StackManager sm;
	sm.AddTCPClient("client", PhysLayerSettings(), "127.0.0.1", 20000);
	sm.AddMaster("client", "master", apl::LEV_WARNING, NULL, MasterStackConfig());
	sm.RemovePort("client");
	sm.AddTCPClient("client", PhysLayerSettings(), "127.0.0.1", 20000);
	sm.AddMaster("client", "master", apl::LEV_WARNING, NULL, MasterStackConfig());
}

BOOST_AUTO_TEST_SUITE_END()

/* vim: set ts=4 sw=4: */
