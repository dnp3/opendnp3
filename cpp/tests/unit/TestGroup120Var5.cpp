/*
 * Copyright 2013-2019 Automatak, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
 * LLC (www.automatak.com) under one or more contributor license agreements.
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Automatak LLC license
 * this file to you under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License. You may obtain
 * a copy of the License at:
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <ser4cpp/container/Buffer.h>
#include <ser4cpp/util/HexConversions.h>

#include <gen/objects/Group120.h>

#include "utils/BufferHelpers.h"

#include <catch.hpp>

using namespace opendnp3;
using namespace ser4cpp;

#define SUITE(name) "Group120Var5TestSuite - " name

TEST_CASE(SUITE("Parser rejects empty buffer"))
{
    HexSequence buffer("");

    Group120Var5 output;
    REQUIRE_FALSE(output.Read(buffer.ToRSeq()));
}

TEST_CASE(SUITE("Parser accepts empty challenge data and hmac"))
{
    // SEQ = 1, USER = 7, KeyWrap = 2 (AES256), KeyStatus = 1 (OK), MacAlgo = 4 (SHA-256 trunc 16), challenge len = 0
    HexSequence buffer("01 00 00 00 07 00 02 01 04 00 00");

    Group120Var5 output;
    REQUIRE(output.Read(buffer.ToRSeq()));
    REQUIRE(output.keyChangeSeqNum == 1);
    REQUIRE(output.userNum == 7);
    REQUIRE(output.keyWrapAlgo == KeyWrapAlgorithm::AES_256);
    REQUIRE(output.keyStatus == KeyStatus::OK);
    REQUIRE(output.hmacAlgo == HMACType::HMAC_SHA256_TRUNC_16);
    REQUIRE(output.challengeData.length() == 0);
    REQUIRE(output.hmacValue.length() == 0);
}

TEST_CASE(SUITE("Parser correctly interprets challenge data and hmac value"))
{
    // SEQ = 1, USER = 7, KeyWrap = 2 (AES256), KeyStatus = 1 (OK), MacAlgo = 4 (SHA-256 trunc 16), challenge len = 3
    HexSequence buffer("01 00 00 00 07 00 02 01 04 03 00 DE AD BE EF");

    Group120Var5 output;
    REQUIRE(output.Read(buffer.ToRSeq()));
    REQUIRE(HexConversions::to_hex(output.challengeData) == "DE AD BE");
    REQUIRE(HexConversions::to_hex(output.hmacValue) == "EF");
}

TEST_CASE(SUITE("Parser rejects one less than minimum required data"))
{
    // SEQ = 1, USER = 7, KeyWrap = 2 (AES256), KeyStatus = 1 (OK), MacAlgo = 4 (SHA-256 trunc 16), challenge len = ???
    // missing
    HexSequence buffer("01 00 00 00 07 00 02 01 04 00");

    Group120Var5 output;
    REQUIRE_FALSE(output.Read(buffer.ToRSeq()));
}

TEST_CASE(SUITE("Parser rejects if specified challenge data is missing"))
{
    // SEQ = 1, USER = 7, KeyWrap = 2 (AES256), KeyStatus = 1 (OK), MacAlgo = 4 (SHA-256 trunc 16), challenge len = 1,
    // missing data
    HexSequence buffer("01 00 00 00 07 00 02 01 04 01 00");

    Group120Var5 output;
    REQUIRE_FALSE(output.Read(buffer.ToRSeq()));
}

TEST_CASE(SUITE("Formatter correctly writes when sufficient space"))
{
    HexSequence challenge("DE AD");
    HexSequence hmac("BE EF");

    Group120Var5 status(8, 3, KeyWrapAlgorithm::AES_256, KeyStatus::OK, HMACType::HMAC_SHA1_TRUNC_8, challenge, hmac);
    const auto SIZE = status.Size();

    REQUIRE(SIZE == 15);

    Buffer output(SIZE);

    auto dest = output.as_wslice();
    REQUIRE(status.Write(dest));
    auto numWritten = output.length() - dest.length();

    REQUIRE(numWritten == SIZE);
    REQUIRE(HexConversions::to_hex(output.as_rslice().take(SIZE)) == "08 00 00 00 03 00 02 01 05 02 00 DE AD BE EF");
}

TEST_CASE(SUITE("Formatter rejects when one less than required space"))
{
    HexSequence challenge("DE AD BE EF");
    HexSequence hmac("AB BA");

    Group120Var5 status(8, 3, KeyWrapAlgorithm::AES_256, KeyStatus::OK, HMACType::HMAC_SHA1_TRUNC_8, challenge, hmac);
    const auto SIZE = status.Size();

    REQUIRE(SIZE == 17);

    Buffer output(SIZE - 1);

    auto dest = output.as_wslice();
    REQUIRE_FALSE(status.Write(dest));
    REQUIRE(dest.length() == output.length());
}