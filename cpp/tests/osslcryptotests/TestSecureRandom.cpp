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

#include <osslcrypto/CryptoProvider.h>

#include <openpal/container/DynamicBuffer.h>
#include <openpal/util/ToHex.h>

#include <thread>

#define SUITE(name) "SecureRandomTestSuite - " name

using namespace std;
using namespace openpal;
using namespace osslcrypto;

TEST_CASE(SUITE("BasicInstantiationAndRequestRandomWorks"))
{
	CryptoProvider provider;

	DynamicBuffer buffer(10);

	for (int i = 0; i < 10; ++i)
	{
		REQUIRE(provider.GetSecureRandom(buffer.GetWriteBufferView()));		
	}	
}

void RunRandIterations(CryptoProvider& provider, int iterations)
{
	DynamicBuffer buffer(100);

	for (int i = 0; i < iterations; ++i)
	{
		REQUIRE(provider.GetSecureRandom(buffer.GetWriteBufferView()));
	}
}

TEST_CASE(SUITE("TestThatMultiThreadingDoesNotCrash"))
{
	CryptoProvider provider;
	vector<unique_ptr<thread>> threads;

	for (int i = 0; i < 100; ++i)
	{
		auto runner = [&provider]() { RunRandIterations(provider, 100); };
		threads.push_back(std::make_unique<std::thread>(runner));
	}

	for (auto& t : threads)
	{
		t->join();
	}
	
}