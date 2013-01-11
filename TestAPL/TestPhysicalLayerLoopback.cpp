
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
#include <boost/test/unit_test.hpp>

#include <APL/Log.h>
#include <APL/PhysLoopback.h>
#include <APLTestTools/MockExecutor.h>
#include <APLTestTools/MockPhysicalLayerAsync.h>
#include <APLTestTools/TestHelpers.h>

using namespace apl;
using namespace boost;

BOOST_AUTO_TEST_SUITE(PhysicalLayerLoopbackSuite)

class LoopbackTest
{
public:

	LoopbackTest() :
		log(),
		exe(),
		phys(log.GetLogger(LEV_INFO, "phys"), &exe),
		loopback(log.GetLogger(LEV_INFO, "loopback"), &phys, &exe) {
		loopback.Start();
	}

	EventLog log;
	MockExecutor exe;
	MockPhysicalLayerAsync phys;
	PhysLoopback loopback;
};

BOOST_AUTO_TEST_CASE(StartsReadingWhenOpened)
{
	LoopbackTest test;

	BOOST_REQUIRE(test.phys.IsOpening());
	test.phys.SignalOpenSuccess();

	BOOST_REQUIRE(test.phys.IsReading());
	BOOST_REQUIRE_FALSE(test.phys.IsWriting());
}

BOOST_AUTO_TEST_CASE(EchosDataOnRead)
{
	LoopbackTest test;
	test.phys.SignalOpenSuccess();

	test.phys.TriggerRead("0A 0B 0C");
	BOOST_REQUIRE(test.phys.IsWriting());
	BOOST_REQUIRE_FALSE(test.phys.IsReading());
	BOOST_REQUIRE(test.phys.BufferEqualsHex("0A 0B 0C"));
}

BOOST_AUTO_TEST_CASE(ReadsAgainAfterDataIsWritten)
{
	LoopbackTest test;
	test.phys.SignalOpenSuccess();

	test.phys.TriggerRead("0A 0B 0C");
	test.phys.SignalSendSuccess();
	BOOST_REQUIRE(test.phys.IsReading());
	BOOST_REQUIRE_FALSE(test.phys.IsWriting());
	test.phys.TriggerRead("0D 0E 0F");
	BOOST_REQUIRE(test.phys.BufferEqualsHex("0A 0B 0C 0D 0E 0F"));
}

BOOST_AUTO_TEST_SUITE_END()
