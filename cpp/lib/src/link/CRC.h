/*
 * Copyright 2013-2020 Automatak, LLC
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
#ifndef OPENDNP3_CRC_H
#define OPENDNP3_CRC_H

#include <ser4cpp/container/SequenceTypes.h>

#include <cstdint>

namespace opendnp3
{

class CRC
{
public:
    static uint16_t CalcCrc(const uint8_t* input, size_t length);

    static uint16_t CalcCrc(const ser4cpp::rseq_t& view);

    static void AddCrc(uint8_t* input, size_t length);

    static bool IsCorrectCRC(const uint8_t* input, size_t length);

private:
    static uint16_t crcTable[256]; // Precomputed CRC lookup table
};

} // namespace opendnp3

#endif
