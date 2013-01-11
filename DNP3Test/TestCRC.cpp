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

#include <DNP3/DNPCrc.h>
#include <APLTestTools/BufferHelpers.h>

#include <iostream>
#include <vector>
#include <string>
#include <sstream>


using namespace std;
using namespace apl;
using namespace apl::dnp;


BOOST_AUTO_TEST_SUITE(CRC)

BOOST_AUTO_TEST_CASE(CrcTest)
{
	HexSequence hs("05 64 05 C0 01 00 00 04 E9 21");
	BOOST_REQUIRE_EQUAL(hs.Size(), 10);
	BOOST_REQUIRE_EQUAL(DNPCrc::CalcCrc(hs, 8), 0x21E9);
}

BOOST_AUTO_TEST_SUITE_END()
