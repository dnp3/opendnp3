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

#include <secauth/StringConversions.h>

using namespace openpal;
using namespace secauth;

#define SUITE(name) "StringConversionTestSuite - " name

TEST_CASE(SUITE("Conversion from slice to string makes a copy of the data"))
{
	auto getJim = []() -> std::string
	{
		uint8_t jim[3] = { 0x6A, 0x69, 0x6D };
		RSlice slice(jim, 3);
		return ToString(slice);
	};

	auto name = getJim();
	REQUIRE(name == "jim");
}

TEST_CASE(SUITE("Conversion from string to slice provides a UTF-8 view of the underlying byte array"))
{
	std::string jim("jim");

	auto slice = AsSlice(jim);

	REQUIRE(slice.Size() == 3);
	REQUIRE(slice[0] == 0x6A);
	REQUIRE(slice[1] == 0x69);
	REQUIRE(slice[2] == 0x6D);
}

