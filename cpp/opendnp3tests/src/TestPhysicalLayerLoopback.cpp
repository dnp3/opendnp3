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
#include <boost/test/unit_test.hpp>

#include <asiopal/Log.h>

#include "PhysLoopback.h"
#include "MockExecutor.h"
#include "MockPhysicalLayerAsync.h"
#include "TestHelpers.h"

using namespace opendnp3;
using namespace openpal;
using namespace boost;

BOOST_AUTO_TEST_SUITE(PhysicalLayerLoopbackSuite)

class LoopbackTest
{
public:

	LoopbackTest() :
		log(),
		exe(),
		phys(Logger(&log, LogLevel::Info, "phys"), &exe),
		loopback(Logger(&log, LogLevel::Info, "loopback"), &phys) {
		loopback.Start();
	}

	asiopal::EventLog log;
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
