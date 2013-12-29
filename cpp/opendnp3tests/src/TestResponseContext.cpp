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
	ResponseContext context(&db);
	BOOST_REQUIRE(QueueResult::OBJECT_UNDEFINED == context.QueueRead(GroupVariation::Group2Var2, StaticRange(0,1)));
	BOOST_REQUIRE(QueueResult::SUCCESS == context.QueueRead(GroupVariation::Group1Var2, StaticRange(0,1)));
}

BOOST_AUTO_TEST_CASE(RespondsWithValues)
{	
	DynamicallyAllocatedDatabase dadb(tmp);
	Database db(dadb.GetFacade());
	ResponseContext context(&db);

	BOOST_REQUIRE(QueueResult::SUCCESS == context.QueueRead(GroupVariation::Group1Var2, StaticRange(0,3)));
		
	ObjectWriter writer(buffer.Truncate(40));
	auto result = context.Load(writer);

	BOOST_REQUIRE_EQUAL("01 02 00 00 03 02 02 02 02", toHex(writer.ToReadOnly()));
}

BOOST_AUTO_TEST_CASE(DetectsOutOfRange)
{	
	DynamicallyAllocatedDatabase dadb(tmp);
	Database db(dadb.GetFacade());
	ResponseContext context(&db);

	BOOST_REQUIRE(QueueResult::OUT_OF_RANGE == context.QueueRead(GroupVariation::Group1Var2, StaticRange(0,6)));
	BOOST_REQUIRE(QueueResult::OUT_OF_RANGE == context.QueueRead(GroupVariation::Group1Var2, StaticRange(4,6)));
	BOOST_REQUIRE(QueueResult::OUT_OF_RANGE == context.QueueRead(GroupVariation::Group1Var2, StaticRange(4,5)));
	BOOST_REQUIRE(QueueResult::OUT_OF_RANGE == context.QueueRead(GroupVariation::Group1Var2, StaticRange(4,3)));
	BOOST_REQUIRE(QueueResult::OUT_OF_RANGE == context.QueueRead(GroupVariation::Group1Var2, StaticRange(10,11)));	
}

BOOST_AUTO_TEST_SUITE_END()
