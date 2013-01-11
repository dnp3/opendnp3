//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one
// or more contributor license agreements. See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  Green Enery Corp licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
//
#include <boost/test/unit_test.hpp>
#include <APLTestTools/TestHelpers.h>

#include <APL/DataTypes.h>
#include <APL/QualityMasks.h>

BOOST_AUTO_TEST_SUITE(QualityMasks)

BOOST_AUTO_TEST_CASE(ToString)
{
	BOOST_REQUIRE_EQUAL(apl::Binary::QualConverter::GetNameString(0), "");

	BOOST_REQUIRE_NOT_EQUAL(apl::Binary::QualConverter::GetNameString(~0).find("LocalForced"), std::string::npos);
	BOOST_REQUIRE_NOT_EQUAL(apl::Counter::QualConverter::GetNameString(~0).find("CommsLost"), std::string::npos);
	BOOST_REQUIRE_NOT_EQUAL(apl::Analog::QualConverter::GetNameString(~0).find("Overrange"), std::string::npos);
	BOOST_REQUIRE_NOT_EQUAL(apl::ControlStatus::QualConverter::GetNameString(~0).find("Online"), std::string::npos);
	BOOST_REQUIRE_NOT_EQUAL(apl::SetpointStatus::QualConverter::GetNameString(~0).find("Restart"), std::string::npos);
}

BOOST_AUTO_TEST_SUITE_END()
