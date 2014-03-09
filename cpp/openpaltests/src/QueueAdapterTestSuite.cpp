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

#include <openpal/StaticQueue.h>

using namespace openpal;

#define SUITE(name) "QueueAdapter - " name

TEST_CASE(SUITE("StaticQueueCompiles"))
{
	StaticQueue<char*, int, 4> queue;
}

TEST_CASE(SUITE("CorrectInitialState"))
{
	StaticArray<int, uint8_t, 3> array;
	QueueAdapter<int, uint8_t> stack(array.ToIndexable());

	REQUIRE(stack.IsEmpty());
	REQUIRE(!stack.IsFull());
	REQUIRE(0 == stack.Size());
	REQUIRE(3 == stack.Capacity());
}

TEST_CASE(SUITE("PushesAndPopsCorrectly"))
{
	StaticArray<int, int, 3> array;
	QueueAdapter<int, int> stack(array.ToIndexable());

	stack.Push(1);
	stack.Push(2);
	stack.Push(3);
	REQUIRE(stack.IsFull());
	REQUIRE(1 == stack.Pop());
	REQUIRE(2 == stack.Pop());
	REQUIRE(3 == stack.Pop());
	REQUIRE(stack.IsFull()); //both full and empty!
	REQUIRE(stack.IsEmpty());
	stack.Clear();	
	REQUIRE(stack.IsEmpty());
	REQUIRE(!stack.IsFull());
}




