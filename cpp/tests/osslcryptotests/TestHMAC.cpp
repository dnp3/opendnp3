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

#define SUITE(name) "HMACTestSuite - " name

using namespace std;
using namespace openpal;
using namespace osslcrypto;
using namespace testlib;

void TestHMACSuccess(IHMACAlgo& algo, const openpal::ReadBufferView& key, const openpal::ReadBufferView& data, const std::string& expected)
{
	DynamicBuffer output(algo.OutputSize());	
	REQUIRE(algo.Calculate(key, { data }, output.GetWriteBufferView()));
	auto resultStr = ToHex(output.ToReadOnly(), false);
	REQUIRE(resultStr == expected);
}

TEST_CASE(SUITE("SHA1-SHA256-SHORTKEY"))
{		
	/*
		From:
		http://www.freeformatter.com/hmac-generator.html
	*/
	std::string key = "key";
	std::string data = "The quick brown fox jumps over the lazy dog";
	std::string HMAC_SHA1 = "DE7C9B85B8B78AA6BC8A7A36F70A90701C9DB4D9";
	std::string HMAC_SHA256 = "F7BC83F430538424B13298E6AA6FB143EF4D59A14946175997479DBC2D1A3CD8";

	auto keyView = ReadBufferView(reinterpret_cast<const uint8_t*>(key.c_str()), key.size());
	auto dataView = ReadBufferView(reinterpret_cast<const uint8_t*>(data.c_str()), data.size());

	CryptoProvider crypto;

	TestHMACSuccess(crypto.GetSHA1HMAC(), keyView, dataView, HMAC_SHA1);
	TestHMACSuccess(crypto.GetSHA256HMAC(), keyView, dataView, HMAC_SHA256);
}

TEST_CASE(SUITE("SHA1-SHA256-LONGKEY"))
{
	/*
	From:
	http://www.freeformatter.com/hmac-generator.html
	*/
	std::string key = "keykeykeykeykeykeykeykeykeykey";
	std::string data = "The quick brown fox jumps over the lazy dog";
	std::string HMAC_SHA1 = "DDD7B3A61B6E05A6E62CF9E0F8E28348E6CC51E2";
	std::string HMAC_SHA256 = "C3700B96B699F9092A40EBA4EC98C2523A945ACF6D488B7B80455BB5A00753F9";

	auto keyView = ReadBufferView(reinterpret_cast<const uint8_t*>(key.c_str()), key.size());
	auto dataView = ReadBufferView(reinterpret_cast<const uint8_t*>(data.c_str()), data.size());

	CryptoProvider crypto;

	TestHMACSuccess(crypto.GetSHA1HMAC(), keyView, dataView, HMAC_SHA1);
	TestHMACSuccess(crypto.GetSHA256HMAC(), keyView, dataView, HMAC_SHA256);
}
