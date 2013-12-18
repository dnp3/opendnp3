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

#include <openpal/DynamicArray.h>
#include <openpal/RandomInsertAdapter.h>

using namespace openpal;

BOOST_AUTO_TEST_SUITE(RandomInsertAdapterTestSuite)

BOOST_AUTO_TEST_CASE(CorrectInitialState)
{
	DynamicArray<uint32_t> indices(3);
	DynamicArray<std::string> values(3);
	RandomInsertAdapter<std::string> ria(values.ToIndexable(), indices.ToIndexable());
	BOOST_REQUIRE(!ria.IsFull());
}

BOOST_AUTO_TEST_CASE(ReleasesValuesCorrectly)
{
	DynamicArray<uint32_t> indices(3);
	DynamicArray<std::string> values(3);	
	RandomInsertAdapter<std::string> ria(values.ToIndexable(), indices.ToIndexable());	

	auto index1 = ria.Add("hello");
	auto index2 = ria.Add("world");
	auto index3 = ria.Add("!!!!");
	
	BOOST_REQUIRE_EQUAL(2, index1);
	BOOST_REQUIRE_EQUAL(1, index2);
	BOOST_REQUIRE_EQUAL(0, index3);

	BOOST_REQUIRE(ria.IsFull());
	ria.Release(index2);
	BOOST_REQUIRE(!ria.IsFull());

	auto index4 = ria.Add("42");
	BOOST_REQUIRE(ria.IsFull());
	BOOST_REQUIRE_EQUAL(index2, index4);
	
}



BOOST_AUTO_TEST_SUITE_END()
