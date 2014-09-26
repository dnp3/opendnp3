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

#include "OutstationTestObject.h"
#include "APDUHexBuilders.h"
#include "HexConversions.h"

#include <opendnp3/ErrorCodes.h>

using namespace std;
using namespace opendnp3;
using namespace openpal;

#define SUITE(name) "OutstationAssignClassTestSuite - " name

TEST_CASE(SUITE("RejectsWithFuncNotSupportedIfAppDoesNotSupport"))
{
	OutstationConfig config;
	OutstationTestObject t(config);
	t.LowerLayerUp();

	// assign binaries to class 2
	t.SendToOutstation("C0 16 3C 03 06 01 00 06");
	REQUIRE(t.lower.PopWriteAsHex() == "C0 81 80 01");

	REQUIRE(t.application.classAssignments.empty());
}

TEST_CASE(SUITE("RejectsWithParamErrorIfNoType"))
{
	OutstationConfig config;
	OutstationTestObject t(config);
	t.application.supportsAssignClass = true;
	t.LowerLayerUp();

	// assign binaries to class 2
	t.SendToOutstation("C0 16 3C 03 06 01 00 06");
	REQUIRE(t.lower.PopWriteAsHex() == "C0 81 80 04");
	REQUIRE(t.application.classAssignments.empty());
}

TEST_CASE(SUITE("AcceptsAssignClassViaAllObjects"))
{
	OutstationConfig config;
	OutstationTestObject t(config, DatabaseTemplate::BinaryOnly(5));
	t.application.supportsAssignClass = true;
	t.LowerLayerUp();

	// assign binaries to class 2
	t.SendToOutstation("C0 16 3C 03 06 01 00 06");
	REQUIRE(t.lower.PopWriteAsHex() == "C0 81 80 00");

	for (uint16_t i = 0; i <= 4; ++i)
	{
		REQUIRE(t.db.buffers.binaries.metadata[i].clazz == PointClass::Class2);
	}
		
	REQUIRE(t.application.classAssignments.size() == 1);
	auto assignment = t.application.classAssignments.front();
	REQUIRE(assignment == std::make_tuple(AssignClassType::BinaryInput, PointClass::Class2, 0, 4));
}

