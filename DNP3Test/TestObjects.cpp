
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
#include <APLTestTools/TestHelpers.h>

#include <DNP3/Objects.h>

using namespace apl;
using namespace std;
using namespace apl::dnp;



BOOST_AUTO_TEST_SUITE(ObjectTests)
template<int T>
bool TestRange(size_t start)
{
	size_t stop = start + T * 8 - 1;

	Group1Var1* pObj = Group1Var1::Inst();

	assert( stop >= start );
	assert( (stop - start + 1) <= (T * 8) );

	uint8_t data[T];
	for(size_t i = 0; i < T; i++) data[i] = 0;

	//check that all bits can be set
	for(size_t i = start; i <= stop; i++) {
		pObj->Write(data, start, i, true);
	}
	for(size_t i = 0; i < T; i++) BOOST_REQUIRE_EQUAL(data[i], 255);

	//check that all bits can be unset
	for(size_t i = start; i <= stop; i++) {
		pObj->Write(data, start, i, false);
	}
	for(size_t i = 0; i < T; i++) BOOST_REQUIRE_EQUAL(data[i], 0);



	for(size_t i = start; i <= stop; i++) {
		uint8_t* pPos = data + (i - start) / 8; //the byte you are on
		size_t bit = (i - start) % 8;
		pObj->Write(data, start, i, true);
		BOOST_REQUIRE_EQUAL(*pPos, 1 << bit);
		pObj->Write(data, start, i, false);
		BOOST_REQUIRE_EQUAL(*pPos, 0);

	}

	return true;
}

BOOST_AUTO_TEST_CASE(BitfieldObjects)
{
	TestRange<1>(0);
	TestRange<1>(1);
	TestRange<2>(0);
	TestRange<2>(3);
	TestRange<4>(1234);
}
BOOST_AUTO_TEST_SUITE_END()
