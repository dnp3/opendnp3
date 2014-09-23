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

#include <openpal/container/Queue.h>

using namespace openpal;

#define SUITE(name) "StaticQueue - " name

TEST_CASE(SUITE("StaticQueueCompiles"))
{
	Queue<char, int> queue(4);
}

TEST_CASE(SUITE("CorrectInitialState"))
{
	Queue<int, uint8_t> queue(3);

	REQUIRE(queue.IsEmpty());
	REQUIRE(!queue.IsFull());
	REQUIRE(0 == queue.Size());
	REQUIRE(3 == queue.Capacity());
}

TEST_CASE(SUITE("PushesUntilFullAndPopsCorrectly"))
{
	Queue<int, uint16_t> queue(3);

	REQUIRE(queue.Enqueue(1));
	REQUIRE(queue.Enqueue(2));
	REQUIRE(queue.Enqueue(3));
	REQUIRE(queue.IsFull());
	REQUIRE(1 == *queue.Pop());
	REQUIRE(2 == *queue.Pop());
	REQUIRE(3 == *queue.Pop());
	REQUIRE(0 == queue.Size());
}

TEST_CASE(SUITE("PushesAndPopsInRing"))
{
	Queue<int, uint16_t> queue(3);

	REQUIRE(queue.Enqueue(1));
	REQUIRE(queue.Enqueue(2));
	REQUIRE(queue.Enqueue(3));
	REQUIRE(queue.IsFull());

	for (int i = 1; i < 100; ++i)
	{
		auto pValue = queue.Pop();
		REQUIRE(pValue);
		REQUIRE(*pValue == i);
		REQUIRE(queue.Enqueue(i + 3));
	}
}

TEST_CASE(SUITE("QueueCanBeClearedWhileNeitherFullNorEmpty"))
{
	Queue<int, uint16_t> queue(3);

	REQUIRE(queue.Enqueue(1));
	REQUIRE(queue.Enqueue(2));
	queue.Clear();
	REQUIRE(0 == queue.Size());
	REQUIRE(queue.Enqueue(1));
	REQUIRE(queue.Enqueue(2));
	REQUIRE(queue.Enqueue(3));
	REQUIRE(3 == queue.Size());
	REQUIRE_FALSE(queue.Enqueue(4));
	REQUIRE(queue.IsFull());
}

TEST_CASE(SUITE("QueueCanBeClearedWhileEmpty"))
{
	Queue<int, uint16_t> queue(3);
	queue.Clear();
	REQUIRE(queue.Enqueue(1));
	REQUIRE(queue.Enqueue(2));
	REQUIRE(queue.Enqueue(3));
	REQUIRE(3 == queue.Size());
	REQUIRE_FALSE(queue.Enqueue(4));
	REQUIRE(queue.IsFull());
}

TEST_CASE(SUITE("QueueCanBeClearedWhileFull"))
{
	Queue<int, uint16_t> queue(3);

	REQUIRE(queue.Enqueue(1));
	REQUIRE(queue.Enqueue(2));
	REQUIRE(queue.Enqueue(3));
	queue.Clear();
	REQUIRE(queue.Enqueue(1));
	REQUIRE(queue.Enqueue(2));
	REQUIRE(queue.Enqueue(3));
	REQUIRE(3 == queue.Size());
	REQUIRE_FALSE(queue.Enqueue(4));
	REQUIRE(queue.IsFull());
}




