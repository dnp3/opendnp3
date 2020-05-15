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
#ifndef OPENDNP3_HEXLOGGING_H
#define OPENDNP3_HEXLOGGING_H

#include "logging/Location.h"

#include "opendnp3/logging/Logger.h"

#include <ser4cpp/container/SequenceTypes.h>
#include <ser4cpp/util/HexConversions.h>
#include <ser4cpp/util/Uncopyable.h>

namespace opendnp3
{

class HexLogging : private ser4cpp::StaticOnly
{
    static const uint32_t max_hex_per_line = max_log_entry_size / 3;

public:
    static void log(Logger& logger,
                    LogLevel level,
                    const ser4cpp::rseq_t& source,
                    char separator = ' ',
                    uint32_t first_row_size = max_hex_per_line,
                    uint32_t other_row_size = max_hex_per_line)
    {
        ser4cpp::rseq_t copy(source);
        uint32_t row = 0;

        const auto max_first_size = ser4cpp::bounded<uint32_t>(first_row_size, 1, max_hex_per_line);
        const auto max_other_size = ser4cpp::bounded<uint32_t>(other_row_size, 1, max_hex_per_line);

        while (copy.is_not_empty())
        {
            const auto row_size = (row == 0) ? max_first_size : max_other_size;
            copy = log_line(logger, level, copy, separator, row_size);
            ++row;
        }
    }

private:
    static ser4cpp::rseq_t log_line(
        Logger& logger, LogLevel level, const ser4cpp::rseq_t& data, char separator, uint32_t max_row_size)
    {
        char buffer[max_log_entry_size];

        uint32_t count = 0;

        while ((count < max_row_size) && (count < data.length()))
        {
            auto pos = count * 3;
            buffer[pos] = ser4cpp::HexConversions::to_hex_char((data[count] & 0xF0) >> 4);
            buffer[pos + 1] = ser4cpp::HexConversions::to_hex_char(data[count] & 0x0F);
            buffer[pos + 2] = separator;
            ++count;
        }

        buffer[(3 * count) - 1] = '\0';

        logger.log(level, LOCATION, buffer);

        return data.skip(count);
    }
};

} // namespace opendnp3

#endif // OPENDNP3_HEXLOGGING_H
