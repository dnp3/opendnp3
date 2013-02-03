
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the GNU Affero General Public License Version 3.0
// (the "Additional License"). You may not use these modifications except in
// compliance with the additional license. You may obtain a copy of the Additional
// License at
//
// http://www.gnu.org/licenses/agpl.html
//
// Contact Automatak, LLC for a commercial license to these modifications
//
#include <boost/test/unit_test.hpp>

#include <opendnp3/Types.h>
#include <limits>

using namespace std;
using namespace opendnp3;

#undef max
#undef min

template<class T>
void TestType(T min, T max, size_t aBytes)
{
	BOOST_CHECK_EQUAL(sizeof(T), aBytes);

	T val = min;
	T limit = numeric_limits<T>::min();
	BOOST_CHECK_EQUAL(val, limit);

	val = max;
	limit = numeric_limits<T>::max();
	BOOST_CHECK_EQUAL(val, limit);
}

BOOST_AUTO_TEST_SUITE(TestTypes)

BOOST_AUTO_TEST_CASE( UByte )
{
	TestType<uint8_t>(0, 255, 1);
}

BOOST_AUTO_TEST_CASE( Int16 )
{
	TestType<int16_t>(-32768, 32767, 2);
}

BOOST_AUTO_TEST_CASE( UInt16 )
{
	TestType<uint16_t>(0, 65535, 2);
}

BOOST_AUTO_TEST_CASE( Int32 )
{
	TestType<int32_t>(0x80000000, 2147483647L, 4);
}

BOOST_AUTO_TEST_CASE( UInt32 )
{
	TestType<uint32_t>(0, 4294967295UL, 4);
}

BOOST_AUTO_TEST_CASE( Int64)
{
	TestType<int64_t>(0x8000000000000000LL, 9223372036854775807LL, 8);
}

BOOST_AUTO_TEST_SUITE_END()
