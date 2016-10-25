/*
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
#include <catch.hpp>

#include <openpal/logging/LogRoot.h>

#include "mocks/PhysLoopback.h"
#include "mocks/MockPhysicalLayer.h"

#include <testlib/MockExecutor.h>
#include <testlib/MockLogHandler.h>

#include <opendnp3/LogLevels.h>

using namespace opendnp3;
using namespace openpal;

#define SUITE(name) "PhysicalLayerLoopbackSuite - " name

class LoopbackTest
{
public:

	LoopbackTest(uint32_t filters = levels::NORMAL) :
		log(),
		root(&log, "test", filters),
		exe(),
		phys(root.logger, exe),
		loopback(root.logger, exe, &phys)
	{
		loopback.Start();
	}

	testlib::MockLogHandler log;
	openpal::LogRoot root;
	testlib::MockExecutor exe;
	MockPhysicalLayer phys;
	PhysLoopback loopback;
};

TEST_CASE(SUITE("StartsReadingWhenOpened"))
{
	LoopbackTest test;

	REQUIRE(test.phys.IsOpening());
	test.phys.SignalOpenSuccess();

	REQUIRE(test.phys.IsReading());
	REQUIRE_FALSE(test.phys.IsWriting());
}

TEST_CASE(SUITE("EchosDataOnRead"))
{
	LoopbackTest test;
	test.phys.SignalOpenSuccess();

	test.phys.TriggerRead("0A 0B 0C");
	REQUIRE(test.phys.IsWriting());
	REQUIRE_FALSE(test.phys.IsReading());
	REQUIRE(test.phys.BufferEqualsHex("0A 0B 0C"));
}

TEST_CASE(SUITE("ReadsAgainAfterDataIsWritten"))
{
	LoopbackTest test;
	test.phys.SignalOpenSuccess();

	test.phys.TriggerRead("0A 0B 0C");
	test.phys.SignalSendSuccess();
	REQUIRE(test.phys.IsReading());
	REQUIRE_FALSE(test.phys.IsWriting());
	test.phys.TriggerRead("0D 0E 0F");
	REQUIRE(test.phys.BufferEqualsHex("0A 0B 0C 0D 0E 0F"));
}


