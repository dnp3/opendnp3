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

#include <opendnp3/APDUParser.h>
#include <opendnp3/APDUHeaderParser.h>
#include <opendnp3/CachedRequest.h>

#include "MockAPDUHeaderHandler.h"
#include "BufferHelpers.h"

using namespace opendnp3;

BOOST_AUTO_TEST_SUITE(CachedRequestTestSuite)

BOOST_AUTO_TEST_CASE(DoesntApplyIfNotSet)
{
	CachedRequest cache(100);	
	BOOST_REQUIRE_FALSE(cache.IsSet());
}

BOOST_AUTO_TEST_CASE(Correctly)
{
	CachedRequest cache(100);
	{
		HexSequence temp("C0 02 01 02 06");		
		APDURecord record;
		BOOST_REQUIRE(APDUHeaderParser::Result::OK == APDUHeaderParser::ParseRequest(temp.ToReadOnly(), record));		
		cache.Set(record, SequenceInfo::PREVIOUS);
	} // source destructs

	BOOST_REQUIRE(cache.IsSet());

	MockApduHeaderHandler handler;
	cache.Apply([&](const APDURecord& record, SequenceInfo seq){ 
		
		BOOST_REQUIRE(SequenceInfo::PREVIOUS == seq);
		BOOST_REQUIRE(APDUParser::Result::OK == APDUParser::ParseHeaders(record.objects, handler));

	});

	BOOST_REQUIRE_FALSE(cache.IsSet());

	BOOST_REQUIRE_EQUAL(1, handler.groupVariations.size());
	BOOST_REQUIRE(GroupVariation::Group1Var2 == handler.groupVariations[0]);
}

BOOST_AUTO_TEST_SUITE_END()
