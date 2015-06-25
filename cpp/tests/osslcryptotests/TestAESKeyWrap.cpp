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

#include <openpal/container/Buffer.h>

#include <testlib/HexConversions.h>
#include <testlib/BufferHelpers.h>

#include <thread>
#include <atomic>

#define SUITE(name) "AESKeyWrapTestSuite - " name

using namespace std;
using namespace openpal;
using namespace osslcrypto;
using namespace testlib;

// signatures for common testing functions
void TestKeyWrap(const std::string& kek, const std::string& input, const std::string& ciphertext, IKeyWrapAlgo& algo);
void TestKeyUnwrap(const std::string& kek, const std::string& input, const std::string& ciphertext, IKeyWrapAlgo& algo);

/*
From rfc3394		- 128 bits of Key Data with a 128 - bit KEK
KEK:				000102030405060708090A0B0C0D0E0F
Input:				00112233445566778899AABBCCDDEEFF
Ciphertext:			1FA68B0A8112B447AEF34BD8FB5A7B82 9D3E862371D2CFE5
*/
TEST_CASE(SUITE("TestKeyWrapUnWrap-128KEK-128Data"))
{
	auto kek = "000102030405060708090A0B0C0D0E0F";
	auto input = "00112233445566778899AABBCCDDEEFF";
	auto ciphertext = "1FA68B0A8112B447AEF34BD8FB5A7B829D3E862371D2CFE5";

	CryptoProvider crypto;
	TestKeyWrap(kek, input, ciphertext, crypto.GetAES128KeyWrap());
	TestKeyUnwrap(kek, input, ciphertext, crypto.GetAES128KeyWrap());
}

/*
From rfc3394		- 128 bits of Key Data with a 256 - bit KEK

KEK:				000102030405060708090A0B0C0D0E0F101112131415161718191A1B1C1D1E1F
Input:				00112233445566778899AABBCCDDEEFF
Ciphertext:			64E8C3F9CE0F5BA263E9777905818A2A 93C8191E7D6E8AE7
*/
TEST_CASE(SUITE("TestKeyWrapUnWrap-256KEK-128Data"))
{	
	auto kek = "000102030405060708090A0B0C0D0E0F101112131415161718191A1B1C1D1E1F";
	auto input = "00112233445566778899AABBCCDDEEFF";
	auto ciphertext = "64E8C3F9CE0F5BA263E9777905818A2A93C8191E7D6E8AE7";

	CryptoProvider crypto;
	TestKeyWrap(kek, input, ciphertext, crypto.GetAES256KeyWrap());
	TestKeyUnwrap(kek, input, ciphertext, crypto.GetAES256KeyWrap());
}

void TestKeyWrap(
	const std::string& kek,
	const std::string& input,
	const std::string& ciphertext,
	IKeyWrapAlgo& algo
	)
{
	HexSequence kekBuffer(kek);
	HexSequence inputBuffer(input);

	const uint32_t OUTPUT_SIZE = inputBuffer.Size() + 8;

	Buffer out(OUTPUT_SIZE);
	auto outputBuffer = out.GetWriteBufferView();
	std::error_code ec;
	auto result = algo.WrapKey(kekBuffer, inputBuffer, outputBuffer, ec);
	REQUIRE_FALSE(ec);	
	REQUIRE(outputBuffer.IsEmpty());
	REQUIRE(ToHex(result, false) == ciphertext);
}

void TestKeyUnwrap(
	const std::string& kek,
	const std::string& input,
	const std::string& ciphertext,
	IKeyWrapAlgo& algo
	)
{	
	HexSequence kekBuffer(kek);
	HexSequence inputBuffer(ciphertext);

	const uint32_t OUTPUT_SIZE = inputBuffer.Size() - 8;

	Buffer out(OUTPUT_SIZE);
	auto outputBuffer = out.GetWriteBufferView();
	std::error_code ec;
	auto result = algo.UnwrapKey(kekBuffer, inputBuffer, outputBuffer, ec);
	REQUIRE_FALSE(ec);
	REQUIRE(result.IsNotEmpty());	
	REQUIRE(outputBuffer.IsEmpty());
	REQUIRE(ToHex(result, false) == input);
}