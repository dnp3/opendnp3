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
#ifndef OPENDNP3_LOGMACROS_H
#define OPENDNP3_LOGMACROS_H

#include "logging/HexLogging.h"
#include "logging/Location.h"

#include <cstdio>

#ifdef WIN32
#define SAFE_STRING_FORMAT(dest, length_, format, ...) _snprintf_s(dest, length_, _TRUNCATE, format, ##__VA_ARGS__)
#else
#define SAFE_STRING_FORMAT(dest, size, format, ...) snprintf(dest, size, format, ##__VA_ARGS__)
#endif // WIN32

#define LOG_FORMAT(logger, levels, format, ...)                                                                        \
    {                                                                                                                  \
        char message_buffer_some_name_no_conflict[opendnp3::max_log_entry_size];                                       \
        SAFE_STRING_FORMAT(message_buffer_some_name_no_conflict, opendnp3::max_log_entry_size, format, ##__VA_ARGS__); \
        logger.log(levels, LOCATION, message_buffer_some_name_no_conflict);                                            \
    }

#define SIMPLE_LOG_BLOCK(logger, levels, message)                                                                      \
    if (logger.is_enabled(levels))                                                                                     \
    {                                                                                                                  \
        logger.log(levels, LOCATION, message);                                                                         \
    }

#define SIMPLE_LOGGER_BLOCK(pLogger, levels, message)                                                                  \
    if (pLogger && pLogger->is_enabled(levels))                                                                        \
    {                                                                                                                  \
        pLogger->log(levels, LOCATION, message);                                                                       \
    }

#define FORMAT_LOG_BLOCK(logger, levels, format, ...)                                                                  \
    if (logger.is_enabled(levels))                                                                                     \
    {                                                                                                                  \
        char message_buffer_some_name_no_conflict[opendnp3::max_log_entry_size];                                       \
        SAFE_STRING_FORMAT(message_buffer_some_name_no_conflict, opendnp3::max_log_entry_size, format, ##__VA_ARGS__); \
        logger.log(levels, LOCATION, message_buffer_some_name_no_conflict);                                            \
    }

#define FORMAT_LOGGER_BLOCK(pLogger, levels, format, ...)                                                              \
    if (pLogger && pLogger->is_enabled(levels))                                                                        \
    {                                                                                                                  \
        char message_buffer_some_name_no_conflict[opendnp3::max_log_entry_size];                                       \
        SAFE_STRING_FORMAT(message_buffer_some_name_no_conflict, opendnp3::max_log_entry_size, format, ##__VA_ARGS__); \
        pLogger->log(levels, LOCATION, message_buffer_some_name_no_conflict);                                          \
    }

#define FORMAT_HEX_BLOCK(logger, levels, buffer, firstSize, otherSize)                                                 \
    if (logger.is_enabled(levels))                                                                                     \
    {                                                                                                                  \
        opendnp3::HexLogging::log(logger, levels, buffer, ' ', firstSize, otherSize);                                  \
    }

#endif // OPENDNP3_LOGMACROS_H
