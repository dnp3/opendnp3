/*
 * Copyright 2013-2022 Step Function I/O, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Step Function I/O
 * LLC (www.automatak.com) under one or more contributor license agreements.
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Step Function I/O LLC license
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
#include "utils/DNPHelpers.h"

#include "utils/BufferHelpers.h"

#include <ser4cpp/util/HexConversions.h>

#include <catch.hpp>
#include <link/CRC.h>
#include <link/LinkFrame.h>

using namespace opendnp3;
using namespace ser4cpp;

std::string RepairCRC(const std::string& arData)
{
    HexSequence hs(arData);

    // validate the size of the data
    REQUIRE(hs.Size() >= 10);
    REQUIRE(hs.Size() <= 292);

    // first determine how much user data is present
    size_t full_blocks = (hs.Size() - 10) / 18;
    size_t partial_size = (hs.Size() - 10) % 18;

    // can't have a partial size < 3 since even 1 byte requires 2 CRC bytes
    if (partial_size > 0)
    {
        REQUIRE(partial_size >= 3);
    }

    // repair the header crc
    CRC::AddCrc(hs, 8);

    uint8_t* ptr = hs + 10;

    // repair the full blocks
    for (size_t i = 0; i < full_blocks; i++)
    {
        CRC::AddCrc(ptr, 16);
        ptr += 18;
    }

    // repair the partial block
    if (partial_size > 0)
        CRC::AddCrc(ptr, partial_size - 2);

    return HexConversions::to_hex(hs.ToRSeq(), true);
}
