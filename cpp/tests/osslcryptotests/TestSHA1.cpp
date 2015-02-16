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

#include <testlib/HexConversions.h>
#include <testlib/BufferHelpers.h>

#include <thread>
#include <atomic>

#define SUITE(name) "SHA1TestSuite - " name

using namespace std;
using namespace openpal;
using namespace osslcrypto;
using namespace testlib;

std::string data = "DE AD BE EF";
std::string digest = "D7 8F 8B B9 92 A5 6A 59 7F 6C 7A 1F B9 18 BB 78 27 13 67 EB";

TEST_CASE(SUITE("TestSHA1"))
{			
	HexSequence input(data);
	DynamicBuffer output(20);

	CryptoProvider crypto;
	auto write = output.GetWriteBufferView();
	REQUIRE(crypto.CalcSHA1(input.ToReadOnly(), write));
	REQUIRE(write.IsEmpty());

	REQUIRE(ToHex(output.ToReadOnly()) == digest);
}

TEST_CASE(SUITE("TestIncrementalSHA1"))
{
	HexSequence input(data);
	DynamicBuffer output(20);

	CryptoProvider crypto;	
	
	auto provider = crypto.CreateSHA1Provider();
	
	for (int i = 0; i < 2; ++i)
	{
		auto write = output.GetWriteBufferView();
		REQUIRE(provider->Init());
		REQUIRE(provider->Add(input.ToReadOnly().Take(2)));
		REQUIRE(provider->Add(input.ToReadOnly().Skip(2)));
		REQUIRE(provider->Complete(write));		
		REQUIRE(write.IsEmpty());
		REQUIRE(ToHex(output.ToReadOnly()) == digest);
	}
	

	
}