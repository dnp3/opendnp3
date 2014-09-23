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

#include <openpal/container/RingBuffer.h>

using namespace openpal;

#define SUITE(name) "RingBuffer - " name

TEST_CASE(SUITE("ReadAndWriteModulo2"))
{	
	RingBuffer rb(3);

	uint8_t buffer[2];
	

	for (int i = 0; i < 10; ++i)
	{
		rb.Put(1);
		rb.Put(2);
		auto wb = WriteBuffer(buffer, 2);
		auto num = rb.Read(wb);
		REQUIRE(num == 2);
		REQUIRE(buffer[0] == 1);
		REQUIRE(buffer[1] == 2);
	}
}

TEST_CASE(SUITE("WriteOverflowDropsOldBytes"))
{
	RingBuffer rb(3);

	uint8_t buffer[3];

	for (int i = 0; i < 10; ++i)
	{

		rb.Put(1);
		rb.Put(2);
		rb.Put(3);
		rb.Put(4);

		auto wb = WriteBuffer(buffer, 3);
		auto num = rb.Read(wb);

		REQUIRE(num == 3);
		REQUIRE(buffer[0] == 2);
		REQUIRE(buffer[1] == 3);
		REQUIRE(buffer[2] == 4);
	}
}
