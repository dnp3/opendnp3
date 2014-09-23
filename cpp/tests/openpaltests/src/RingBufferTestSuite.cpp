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

#include <thread>

using namespace openpal;

#define SUITE(name) "RingBufferTestSuite - " name

void RunTest();

TEST_CASE(SUITE("RingBufferIsThreadSafe"))
{	
	for (int i = 0; i < 100; ++i)
	{
		RunTest();
	}	
}

void RunTest()
{
	RingBuffer<128> rb;

	const int SIZE = 1024 * 1024;

	auto source = std::unique_ptr<uint8_t[]>(new uint8_t[SIZE]);
	auto destination = std::unique_ptr<uint8_t[]>(new uint8_t[SIZE]);

	//initialize source to a known pattern modulo 256
	for (int i = 0; i < SIZE; ++i)
	{
		source[i] = static_cast<uint8_t>(i & 0xFF);
	}

	ReadOnlyBuffer src(source.get(), SIZE);
	WriteBuffer dest(destination.get(), SIZE);

	auto writeAll = [&]() {
		while (!src.IsEmpty())
		{
			rb.PutMany(src);
		}
	};

	auto readAll = [&]() {
		while (!dest.IsEmpty())
		{
			rb.GetMany(dest);
		}
	};

	std::thread writer(writeAll);
	std::thread reader(readAll);

	writer.join();
	reader.join();

	for (int i = 0; i < SIZE; ++i)
	{
		if (source[i] != destination[i])
		{
			FAIL("source != dest at index: " << i);
		}
	}
}


