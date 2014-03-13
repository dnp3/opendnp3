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


#include <opendnp3/app/APDUParser.h>
#include <opendnp3/app/APDUHeaderParser.h>
#include <opendnp3/outstation/CachedRequest.h>

#include "MockAPDUHeaderHandler.h"
#include "BufferHelpers.h"

using namespace opendnp3;

#define SUITE(name) "CachedRequestTestSuite - " name

TEST_CASE(SUITE("DoesntApplyIfNotSet"))
{
	CachedRequest cache;
	REQUIRE_FALSE(cache.IsSet());
}

TEST_CASE(SUITE("Correctly"))
{
	CachedRequest cache;
	{
		HexSequence temp("C0 02 01 02 06");
		APDURecord record;
		REQUIRE((APDUHeaderParser::Result::OK == APDUHeaderParser::ParseRequest(temp.ToReadOnly(), record)));
		cache.Set(record, SequenceInfo::PREVIOUS);
	} // source destructs

	REQUIRE(cache.IsSet());

	MockApduHeaderHandler handler;
	cache.Apply([&](const APDURecord & record, SequenceInfo seq)
	{

		REQUIRE((SequenceInfo::PREVIOUS == seq));
		REQUIRE((APDUParser::Result::OK == APDUParser::ParseTwoPass(record.objects, &handler, nullptr)));

	});

	REQUIRE_FALSE(cache.IsSet());

	REQUIRE(1 ==  handler.groupVariations.size());
	REQUIRE((GroupVariation::Group1Var2 == handler.groupVariations[0]));
}


