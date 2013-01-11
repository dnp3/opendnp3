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


/*
The tests in this file deal with the casting Int64 to double problems on the ARM device compiling with GCC.
A manual method is included that gets around the problem as well.
*/

#include <boost/test/unit_test.hpp>
#include <APLTestTools/TestHelpers.h>
#include <APL/Types.h>
#include "math.h"
#include <APL/Util.h>
#include <boost/numeric/conversion/converter.hpp>

#include <iostream>

using namespace apl;



BOOST_AUTO_TEST_SUITE(Casting)
template <typename T1, typename T2>
T2 TestCast(T1 input)
{
	return static_cast<T2>(input);
}

template <typename T1>
bool IsEqual(T1 a, T1 b)
{
	return a == b;
}

template <>
bool IsEqual(double a, double b)
{
	double diff = a - b;
	if(diff < 0) diff = -diff;
	return diff <= 1e-5;
}

template <>
bool IsEqual(float a, float b)
{
	float diff = a - b;
	if(diff < 0) diff = -diff;
	return diff <= 1e-5;
}

#define CHECK_CAST(t1, t2, a, b)\
	{t2 amod = TestCast<t1, t2>(a);\
	if(!IsEqual<t2>(amod,(b))){\
			std::ostringstream oss;					\
			oss << (#t1)<< "->" << (#t2) << " doesn't cast correctly: " << (amod) << " != " << (b); \
			std::cout << "Failure: " << oss.str() << std::endl;\
	}}


BOOST_AUTO_TEST_CASE(Casting)
{
	CHECK_CAST(double, int, 1000.0, 1000);
	CHECK_CAST(double, float, 1001.0, 1001.0f);
	CHECK_CAST(double, long, 1002.0, 1002);
	//these 2 tests fail on the arm device and fail strangley, there
	//is definatley something weird going on here, probably because
	//the 64 bit long long is the same size as a double so just gets
	//shifted into the double strangley. This means that the results
	//of an operation can be different if the casting is done and the
	//result is stored to memory or if the value stays in a register
	//(as an intermediate calculation value), very nasty.
	CHECK_CAST(double, int64_t, 1003.0, 1003);

	CHECK_CAST(int64_t, double, 1004, 1004.0);

	CHECK_CAST(double, float, 1012.0, 1012.0f);
	CHECK_CAST(int64_t, double, 1006, 1006.0);

	CHECK_CAST(int, double, 1007, 1007.0);
	CHECK_CAST(long, double, 1008, 1008.0);
	CHECK_CAST(int, float, 1009, 1009.0f);

	CHECK_CAST(float, int64_t, 1010.0f, 1010);
	CHECK_CAST(float, int, 1011.0f, 1011);

	CHECK_CAST(int64_t, double, 4294967296LL, 4294967296.0);
	CHECK_CAST(double, int64_t, 4294967296.0, 4294967296LL);

}

BOOST_AUTO_TEST_CASE(ManualConversion)
{

	BOOST_REQUIRE_FLOAT_EQUAL(4294967296.0, SafeCastInt64ToDouble(4294967296LL));
	BOOST_REQUIRE_FLOAT_EQUAL(4398046511104.00, SafeCastInt64ToDouble(4398046511104LL));

}
BOOST_AUTO_TEST_SUITE_END()
