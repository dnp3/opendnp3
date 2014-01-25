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

#include <openpal/StaticQueue.h>

using namespace openpal;

BOOST_AUTO_TEST_SUITE(QueueAdapterTestSuite)

BOOST_AUTO_TEST_CASE(CorrectInitialState)
{
	StaticArray<int, uint8_t, 3> array;
	QueueAdapter<int, uint8_t> stack(array.ToIndexable());

	BOOST_REQUIRE(stack.IsEmpty());
	BOOST_REQUIRE(!stack.IsFull());
	BOOST_REQUIRE_EQUAL(0, stack.Size());
	BOOST_REQUIRE_EQUAL(3, stack.Capacity());
}

BOOST_AUTO_TEST_CASE(PushesAndPopsCorrectly)
{
	StaticArray<int, int, 3> array;
	QueueAdapter<int, int> stack(array.ToIndexable());

	stack.Push(1);
	stack.Push(2);
	stack.Push(3);
	BOOST_REQUIRE(stack.IsFull());
	BOOST_REQUIRE_EQUAL(1, stack.Pop());
	BOOST_REQUIRE_EQUAL(2, stack.Pop());
	BOOST_REQUIRE_EQUAL(3, stack.Pop());
	BOOST_REQUIRE(stack.IsFull()); //both full and empty!
	BOOST_REQUIRE(stack.IsEmpty());
	stack.Clear();	
	BOOST_REQUIRE(stack.IsEmpty());
	BOOST_REQUIRE(!stack.IsFull());
}

BOOST_AUTO_TEST_CASE(StaticQueueCompiles)
{
	StaticQueue<char*, int, 4> queue;	
}

BOOST_AUTO_TEST_SUITE_END()
