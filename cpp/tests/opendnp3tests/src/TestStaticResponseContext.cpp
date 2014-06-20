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

#include <opendnp3/outstation/DynamicallyAllocatedDatabase.h>
#include <opendnp3/outstation/StaticResponseContext.h>

#include <opendnp3/app/APDURequest.h>
#include <opendnp3/app/APDUResponse.h>

#include <openpal/util/ToHex.h>

#include "APDUHelpers.h"

using namespace openpal;
using namespace opendnp3;

#define SUITE(name) "StaticResponseContextTestSuite - " name

DatabaseTemplate tmp(5, 5, 5, 5, 5, 5);

const uint32_t SIZE = 2048;
uint8_t fixedBuffer[SIZE];

/* TODO
TEST_CASE(SUITE("RejectsUnknownVariation"))
{
	DynamicallyAllocatedDatabase dadb(tmp);
	Database db(dadb.GetFacade());

	StaticResponseContext context(&db);
	REQUIRE((QueueResult::OBJECT_UNDEFINED == context.QueueReadRange(GroupVariation::Group2Var2, StaticRange(0, 1))));
	REQUIRE((QueueResult::SUCCESS == context.QueueReadRange(GroupVariation::Group1Var2, StaticRange(0, 1))));
}


TEST_CASE(SUITE("RespondsWithValues"))
{
	DynamicallyAllocatedDatabase dadb(tmp);
	Database db(dadb.GetFacade());
	StaticResponseContext context(&db);

	REQUIRE((QueueResult::SUCCESS == context.QueueReadRange(GroupVariation::Group1Var2, StaticRange(0, 3))));

	APDUResponse rsp(APDUHelpers::Response());
	REQUIRE((StaticLoadResult::COMPLETED == context.Load(rsp)));

	REQUIRE("C0 81 00 00 01 02 00 00 03 02 02 02 02" ==  toHex(rsp.ToReadOnly()));
}

TEST_CASE(SUITE("RespondsWithFrozenCounters"))
{
	DynamicallyAllocatedDatabase dadb(tmp);
	Database db(dadb.GetFacade());
	StaticResponseContext context(&db);

	REQUIRE((QueueResult::SUCCESS == context.QueueReadRange(GroupVariation::Group21Var1, StaticRange(0, 3))));

	APDUResponse rsp(APDUHelpers::Response());
	REQUIRE((StaticLoadResult::COMPLETED == context.Load(rsp)));

	REQUIRE("C0 81 00 00 15 01 00 00 03 02 00 00 00 00 02 00 00 00 00 02 00 00 00 00 02 00 00 00 00" ==  toHex(rsp.ToReadOnly()));
}

TEST_CASE(SUITE("DetectsOutOfRange"))
{
	DynamicallyAllocatedDatabase dadb(tmp);
	Database db(dadb.GetFacade());
	StaticResponseContext context(&db);

	REQUIRE((QueueResult::OUT_OF_RANGE == context.QueueReadRange(GroupVariation::Group1Var2, StaticRange(0, 6))));
	REQUIRE((QueueResult::OUT_OF_RANGE == context.QueueReadRange(GroupVariation::Group1Var2, StaticRange(4, 6))));
	REQUIRE((QueueResult::OUT_OF_RANGE == context.QueueReadRange(GroupVariation::Group1Var2, StaticRange(4, 5))));
	REQUIRE((QueueResult::OUT_OF_RANGE == context.QueueReadRange(GroupVariation::Group1Var2, StaticRange(4, 3))));
	REQUIRE((QueueResult::OUT_OF_RANGE == context.QueueReadRange(GroupVariation::Group1Var2, StaticRange(10, 11))));
}

TEST_CASE(SUITE("WritesMixedValues"))
{
	DynamicallyAllocatedDatabase dadb(tmp);
	Database db(dadb.GetFacade());
	StaticResponseContext context(&db);

	REQUIRE((QueueResult::SUCCESS == context.QueueReadRange(GroupVariation::Group30Var2, StaticRange(1, 2))));
	REQUIRE((QueueResult::SUCCESS == context.QueueReadRange(GroupVariation::Group1Var2, StaticRange(3, 4))));

	APDUResponse rsp(APDUHelpers::Response());
	REQUIRE((StaticLoadResult::COMPLETED == context.Load(rsp)));
	REQUIRE("C0 81 00 00 1E 02 00 01 02 02 00 00 02 00 00 01 02 00 03 04 02 02" ==  toHex(rsp.ToReadOnly()));
}

TEST_CASE(SUITE("ReturnsFullWhen2ndHeaderCantBeWritten"))
{
	DynamicallyAllocatedDatabase dadb(tmp);
	Database db(dadb.GetFacade());
	StaticResponseContext context(&db);

	REQUIRE((QueueResult::SUCCESS == context.QueueReadRange(GroupVariation::Group30Var2, StaticRange(1, 2))));
	REQUIRE((QueueResult::SUCCESS == context.QueueReadRange(GroupVariation::Group1Var2, StaticRange(3, 4))));

	{
		APDUResponse rsp(APDUHelpers::Response(16)); //enough for first header, but not the 2nd

		REQUIRE((StaticLoadResult::FULL == context.Load(rsp)));
		REQUIRE("A0 81 00 00 1E 02 00 01 02 02 00 00 02 00 00" ==  toHex(rsp.ToReadOnly()));
	}

	{
		APDUResponse rsp(APDUHelpers::Response());
		REQUIRE((StaticLoadResult::COMPLETED == context.Load(rsp)));
		REQUIRE("40 81 00 00 01 02 00 03 04 02 02" ==  toHex(rsp.ToReadOnly()));
	}
}

TEST_CASE(SUITE("ReturnsFullWhenOnlyPartof2ndHeaderCanBeWritten"))
{
	DynamicallyAllocatedDatabase dadb(tmp);
	Database db(dadb.GetFacade());
	StaticResponseContext context(&db);

	REQUIRE((QueueResult::SUCCESS == context.QueueReadRange(GroupVariation::Group30Var2, StaticRange(1, 2))));
	REQUIRE((QueueResult::SUCCESS == context.QueueReadRange(GroupVariation::Group1Var2, StaticRange(3, 4))));

	{
		APDUResponse rsp(APDUHelpers::Response(21)); //enough for first header, but not the full 2nd header
		REQUIRE((StaticLoadResult::FULL == context.Load(rsp)));
		REQUIRE("A0 81 00 00 1E 02 00 01 02 02 00 00 02 00 00 01 02 00 03 03 02" ==  toHex(rsp.ToReadOnly()));
	}

	{
		APDUResponse rsp(APDUHelpers::Response());
		REQUIRE((StaticLoadResult::COMPLETED == context.Load(rsp)));
		REQUIRE("40 81 00 00 01 02 00 04 04 02" ==  toHex(rsp.ToReadOnly()));
	}
}

TEST_CASE(SUITE("HandlesIntegrityPoll"))
{
	DatabaseTemplate tmp2(1, 0, 1, 0, 1, 0); // 1 Binary, 1 Counter, 1 Control Status
	DynamicallyAllocatedDatabase dadb(tmp2);
	Database db(dadb.GetFacade());
	StaticResponseContext context(&db);

	REQUIRE((QueueResult::SUCCESS == context.QueueReadAllObjects(GroupVariation::Group60Var1)));

	{
		APDUResponse rsp(APDUHelpers::Response());
		REQUIRE((StaticLoadResult::COMPLETED == context.Load(rsp)));
		REQUIRE("C0 81 00 00 01 02 00 00 00 02 14 01 00 00 00 02 00 00 00 00 0A 02 00 00 00 02" ==  toHex(rsp.ToReadOnly()));
	}
}
*/


