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
#include <catch.hpp>

#include <opendnp3/ErrorCodes.h>
#include <opendnp3/link/LinkFrame.h>

#include <openpal/util/ToHex.h>
#include <openpal/container/Buffer.h>

#include "BufferSegment.h"
#include "LinkLayerTest.h"
#include "LinkHex.h"

#include <testlib/HexConversions.h>

#include <iostream>

using namespace openpal;
using namespace opendnp3;
using namespace testlib;

#define SUITE(name) "LinkLayerKeepAliveTestSuite - " name

TEST_CASE(SUITE("ForwardsKeepAliveTimeouts"))
{
	LinkConfig config(true, false);
	config.KeepAliveTimeout = TimeDuration::Seconds(5);
	LinkLayerTest t(config);
	
	t.link.OnLowerLayerUp();

	REQUIRE(t.exe.NumPendingTimers() == 1);
	REQUIRE(t.listener.numKeepAliveTimeout == 0);

	REQUIRE(t.exe.AdvanceToNextTimer());
	REQUIRE(t.exe.RunMany() > 0);
	REQUIRE(t.listener.numKeepAliveTimeout == 1);
}

TEST_CASE(SUITE("SendsKeepAliveAfterExpiration"))
{
	LinkConfig config(true, false);
	config.KeepAliveTimeout = TimeDuration::Seconds(5);
	LinkLayerTest t(config);

	t.link.OnLowerLayerUp();

	REQUIRE(t.exe.NumPendingTimers() == 1);
	REQUIRE(t.listener.numKeepAliveTimeout == 0);

	REQUIRE(t.exe.AdvanceToNextTimer());
	REQUIRE(t.exe.RunMany() > 0);	

	REQUIRE(ToHex(t.lastWrite) == LinkHex::Ack(true, false, 1024, 1));
}



