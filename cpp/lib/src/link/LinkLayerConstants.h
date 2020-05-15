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
#ifndef OPENDNP3_LINKLAYERCONSTANTS_H
#define OPENDNP3_LINKLAYERCONSTANTS_H

#include <cstdint>

namespace opendnp3
{

const uint8_t LPDU_MIN_LENGTH = 5;
const uint8_t LPDU_MAX_LENGTH = 255;
const uint8_t LPDU_HEADER_SIZE = 10;
const uint8_t LPDU_DATA_BLOCK_SIZE = 16;
const uint8_t LPDU_CRC_SIZE = 2;
const uint8_t LPDU_DATA_PLUS_CRC_SIZE = 18;
const uint8_t LPDU_MAX_USER_DATA_SIZE = 250;
const uint16_t LPDU_MAX_FRAME_SIZE = 292; // 10(header) + 250 (user data) + 32 (block CRC's) = 292 frame bytes

// Broadcast addresses
enum LinkBroadcastAddress : uint16_t
{
    DontConfirm = 0xFFFD,
    ShallConfirm = 0xFFFE,
    OptionalConfirm = 0xFFFF,
};

/// Indices for use with buffers containing link headers
enum LinkHeaderIndex : uint8_t
{
    LI_START_05 = 0,
    LI_START_64 = 1,
    LI_LENGTH = 2,
    LI_CONTROL = 3,
    LI_DESTINATION = 4,
    LI_SOURCE = 6,
    LI_CRC = 8
};

/// Masks for use with the CONTROL byte
enum ControlMask : uint8_t
{
    MASK_DIR = 0x80,
    MASK_PRM = 0x40,
    MASK_FCB = 0x20,
    MASK_FCV = 0x10,
    MASK_FUNC = 0x0F,
    MASK_FUNC_OR_PRM = MASK_PRM | MASK_FUNC
};

} // namespace opendnp3

#endif
