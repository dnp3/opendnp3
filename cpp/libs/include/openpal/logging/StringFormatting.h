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
#ifndef OPENPAL_STRINGFORMATTING_H
#define OPENPAL_STRINGFORMATTING_H

#include "Logger.h"

namespace openpal
{
class RSlice;

const uint32_t MAX_LOG_ENTRY_SIZE = 120;
const uint32_t MAX_HEX_PER_LINE = 20;

static_assert(MAX_HEX_PER_LINE < (MAX_LOG_ENTRY_SIZE / 3), "Each hex byte takes 3 characters");

void LogHex(Logger& logger,
            const openpal::LogFilters& filters,
            const openpal::RSlice& source,
            uint32_t firstRowSize,
            uint32_t otherRowSize);

// Portable allocation of a copy of a cstring
char* AllocateCopy(char const* src);

} // namespace openpal

#endif
