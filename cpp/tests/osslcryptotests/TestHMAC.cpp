/*
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
#include <osslcrypto/ErrorCodes.h>

#include <openpal/container/Buffer.h>

#include <testlib/HexConversions.h>
#include <testlib/BufferHelpers.h>

#include <thread>
#include <atomic>

#define SUITE(name) "HMACTestSuite - " name

using namespace std;
using namespace openpal;
using namespace osslcrypto;
using namespace testlib;

void TestHMACSuccess(IHMACAlgo& algo, const openpal::RSlice& key, const openpal::RSlice& data, const std::string& expected)
{
	Buffer output(algo.OutputSize());
	auto dest = output.GetWSlice();
	error_code ec;
	auto result = algo.Calculate(key, { data }, dest, ec);
	REQUIRE_FALSE(ec);
	auto resultStr = ToHex(result, false);
	REQUIRE(resultStr == expected);
}

void TestInsufficientOutputSizeFails(IHMACAlgo& algo)
{
	Buffer buffer(algo.OutputSize() - 1);
	auto dest = buffer.GetWSlice();
	error_code ec;
	auto output = algo.Calculate(RSlice(), { RSlice() }, dest, ec);
	REQUIRE(ec == make_error_code(errors::HMAC_INSUFFICIENT_OUTPUT_BUFFER_SIZE));
	REQUIRE(output.IsEmpty());
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

	auto keyView = RSlice(reinterpret_cast<const uint8_t*>(key.c_str()), key.size());
	auto dataView = RSlice(reinterpret_cast<const uint8_t*>(data.c_str()), data.size());

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

	auto keyView = RSlice(reinterpret_cast<const uint8_t*>(key.c_str()), key.size());
	auto dataView = RSlice(reinterpret_cast<const uint8_t*>(data.c_str()), data.size());

	CryptoProvider crypto;

	TestHMACSuccess(crypto.GetSHA1HMAC(), keyView, dataView, HMAC_SHA1);
	TestHMACSuccess(crypto.GetSHA256HMAC(), keyView, dataView, HMAC_SHA256);
}

TEST_CASE(SUITE("InsufficientWriteBuffer"))
{
	CryptoProvider crypto;
	TestInsufficientOutputSizeFails(crypto.GetSHA1HMAC());
	TestInsufficientOutputSizeFails(crypto.GetSHA256HMAC());
}
