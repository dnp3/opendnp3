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

#include "TestHelpers.h"

#include <opendnp3/DynamicallyAllocatedDatabase.h>
#include <opendnp3/ResponseContext.h>

#include <openpal/ToHex.h>


using namespace openpal;
using namespace opendnp3;

BOOST_AUTO_TEST_SUITE(ResponseContextTestSuite)

DatabaseTemplate tmp(5,5,5,5,5);
uint8_t bytes[2048];
const WriteBuffer buffer(bytes, 2048);

BOOST_AUTO_TEST_CASE(RejectsUnknownVariation)
{	
	DynamicallyAllocatedDatabase dadb(tmp);
	Database db(dadb.GetFacade());

	ResponseContext context(&db, nullptr); //TODO
	BOOST_REQUIRE(QueueResult::OBJECT_UNDEFINED == context.QueueReadRange(GroupVariation::Group2Var2, StaticRange(0,1)));
	BOOST_REQUIRE(QueueResult::SUCCESS == context.QueueReadRange(GroupVariation::Group1Var2, StaticRange(0,1)));
}

BOOST_AUTO_TEST_CASE(RespondsWithValues)
{	
	DynamicallyAllocatedDatabase dadb(tmp);
	Database db(dadb.GetFacade());
	ResponseContext context(&db, nullptr); //TODO

	BOOST_REQUIRE(QueueResult::SUCCESS == context.QueueReadRange(GroupVariation::Group1Var2, StaticRange(0,3)));
		
	ObjectWriter writer(buffer.Truncate(40));
	BOOST_REQUIRE(LoadResult::COMPLETED == context.Load(writer));

	BOOST_REQUIRE_EQUAL("01 02 00 00 03 02 02 02 02", toHex(writer.ToReadOnly()));
}

BOOST_AUTO_TEST_CASE(DetectsOutOfRange)
{	
	DynamicallyAllocatedDatabase dadb(tmp);
	Database db(dadb.GetFacade());
	ResponseContext context(&db, nullptr); //TODO

	BOOST_REQUIRE(QueueResult::OUT_OF_RANGE == context.QueueReadRange(GroupVariation::Group1Var2, StaticRange(0,6)));
	BOOST_REQUIRE(QueueResult::OUT_OF_RANGE == context.QueueReadRange(GroupVariation::Group1Var2, StaticRange(4,6)));
	BOOST_REQUIRE(QueueResult::OUT_OF_RANGE == context.QueueReadRange(GroupVariation::Group1Var2, StaticRange(4,5)));
	BOOST_REQUIRE(QueueResult::OUT_OF_RANGE == context.QueueReadRange(GroupVariation::Group1Var2, StaticRange(4,3)));
	BOOST_REQUIRE(QueueResult::OUT_OF_RANGE == context.QueueReadRange(GroupVariation::Group1Var2, StaticRange(10,11)));	
}

BOOST_AUTO_TEST_CASE(WritesMixedValues)
{	
	DynamicallyAllocatedDatabase dadb(tmp);
	Database db(dadb.GetFacade());
	ResponseContext context(&db, nullptr); //TODO

	BOOST_REQUIRE(QueueResult::SUCCESS == context.QueueReadRange(GroupVariation::Group30Var2, StaticRange(1,2)));
	BOOST_REQUIRE(QueueResult::SUCCESS == context.QueueReadRange(GroupVariation::Group1Var2, StaticRange(3,4)));

	ObjectWriter writer(buffer.Truncate(40));
	BOOST_REQUIRE(LoadResult::COMPLETED == context.Load(writer));
	BOOST_REQUIRE_EQUAL("1E 02 00 01 02 02 00 00 02 00 00 01 02 00 03 04 02 02", toHex(writer.ToReadOnly()));	
}

BOOST_AUTO_TEST_CASE(ReturnsFullWhen2ndHeaderCantBeWritten)
{	
	DynamicallyAllocatedDatabase dadb(tmp);
	Database db(dadb.GetFacade());
	ResponseContext context(&db, nullptr); //TODO

	BOOST_REQUIRE(QueueResult::SUCCESS == context.QueueReadRange(GroupVariation::Group30Var2, StaticRange(1,2)));
	BOOST_REQUIRE(QueueResult::SUCCESS == context.QueueReadRange(GroupVariation::Group1Var2, StaticRange(3,4)));

	{
		ObjectWriter writer(buffer.Truncate(12)); //enough for first header, but not the 2nd
		BOOST_REQUIRE(LoadResult::FULL == context.Load(writer));
		BOOST_REQUIRE_EQUAL("1E 02 00 01 02 02 00 00 02 00 00", toHex(writer.ToReadOnly()));
	}

	{
		ObjectWriter writer(buffer.Truncate(12));
		BOOST_REQUIRE(LoadResult::COMPLETED == context.Load(writer));
		BOOST_REQUIRE_EQUAL("01 02 00 03 04 02 02", toHex(writer.ToReadOnly()));
	}
}

BOOST_AUTO_TEST_CASE(ReturnsFullWhenOnlyPartof2ndHeaderCanBeWritten)
{	
	DynamicallyAllocatedDatabase dadb(tmp);
	Database db(dadb.GetFacade());
	ResponseContext context(&db, nullptr); //TODO

	BOOST_REQUIRE(QueueResult::SUCCESS == context.QueueReadRange(GroupVariation::Group30Var2, StaticRange(1,2)));
	BOOST_REQUIRE(QueueResult::SUCCESS == context.QueueReadRange(GroupVariation::Group1Var2, StaticRange(3,4)));

	{
		ObjectWriter writer(buffer.Truncate(17)); //enough for first header, but not the full 2nd header
		BOOST_REQUIRE(LoadResult::FULL == context.Load(writer));
		BOOST_REQUIRE_EQUAL("1E 02 00 01 02 02 00 00 02 00 00 01 02 00 03 03 02", toHex(writer.ToReadOnly()));
	}

	{
		ObjectWriter writer(buffer.Truncate(12));
		BOOST_REQUIRE(LoadResult::COMPLETED == context.Load(writer));
		BOOST_REQUIRE_EQUAL("01 02 00 04 04 02", toHex(writer.ToReadOnly()));
	}
}

BOOST_AUTO_TEST_CASE(HandlesIntegrityPoll)
{	
	DatabaseTemplate tmp2(1, 0, 1, 1, 0); // 1 Binary, 1 Counter, 1 Control Status
	DynamicallyAllocatedDatabase dadb(tmp2);
	Database db(dadb.GetFacade());
	ResponseContext context(&db, nullptr); //TODO

	BOOST_REQUIRE(QueueResult::SUCCESS == context.QueueReadAllObjects(GroupVariation::Group60Var1));

	{
		ObjectWriter writer(buffer); 
		BOOST_REQUIRE(LoadResult::COMPLETED == context.Load(writer));
		BOOST_REQUIRE_EQUAL("01 02 00 00 00 02 14 02 00 00 00 02 00 00 0A 02 00 00 00 02", toHex(writer.ToReadOnly()));
	}
}

BOOST_AUTO_TEST_SUITE_END()
