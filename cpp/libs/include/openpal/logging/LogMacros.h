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
#ifndef OPENPAL_LOGMACROS_H
#define OPENPAL_LOGMACROS_H

#include "Location.h"

#ifndef OPENPAL_STRIP_LOGGING

#include "StringFormatting.h"

#include <cstdio>

#ifdef WIN32
#define SAFE_STRING_FORMAT(dest, size, format, ...) _snprintf_s(dest, size, _TRUNCATE, format, ##__VA_ARGS__)
#else
#define SAFE_STRING_FORMAT(dest, size, format, ...) snprintf(dest, size, format, ##__VA_ARGS__)
#endif

#ifdef OPENPAL_CUSTOMIZE_LOGGING

#include "CustomLogMacros.h"

#else

#define SIMPLE_LOG_BLOCK(logger, filters, message)                                                                     \
    if (logger.IsEnabled(filters))                                                                                     \
    {                                                                                                                  \
        logger.Log(filters, LOCATION, message);                                                                        \
    }

#define SIMPLE_LOGGER_BLOCK(pLogger, filters, message)                                                                 \
    if (pLogger && pLogger->IsEnabled(filters))                                                                        \
    {                                                                                                                  \
        pLogger->Log(filters, LOCATION, message);                                                                      \
    }

#define FORMAT_LOG_BLOCK(logger, filters, format, ...)                                                                 \
    if (logger.IsEnabled(filters))                                                                                     \
    {                                                                                                                  \
        char format_message_buffer[openpal::MAX_LOG_ENTRY_SIZE];                                                       \
        SAFE_STRING_FORMAT(format_message_buffer, openpal::MAX_LOG_ENTRY_SIZE, format, ##__VA_ARGS__);                 \
        logger.Log(filters, LOCATION, format_message_buffer);                                                          \
    }

#define FORMAT_LOGGER_BLOCK(pLogger, filters, format, ...)                                                             \
    if (pLogger && pLogger->IsEnabled(filters))                                                                        \
    {                                                                                                                  \
        char format_message_buffer[openpal::MAX_LOG_ENTRY_SIZE];                                                       \
        SAFE_STRING_FORMAT(format_message_buffer, openpal::MAX_LOG_ENTRY_SIZE, format, ##__VA_ARGS__);                 \
        pLogger->Log(filters, LOCATION, format_message_buffer);                                                        \
    }

#define FORMAT_HEX_BLOCK(logger, filters, buffer, firstSize, otherSize)                                                \
    if (logger.IsEnabled(filters))                                                                                     \
    {                                                                                                                  \
        LogHex(logger, filters, buffer, firstSize, otherSize);                                                         \
    }

#endif

#else

#define SAFE_STRING_FORMAT(dest, size, format, ...)

#define SIMPLE_LOG_BLOCK(logger, filters, message)

#define SIMPLE_LOGGER_BLOCK(pLogger, filters, message)

#define FORMAT_LOG_BLOCK(logger, filters, format, ...)

#define FORMAT_LOGGER_BLOCK(pLogger, filters, format, ...)

#define FORMAT_HEX_BLOCK(logger, filters, buffer, firstSize, otherSize)

#endif // end OPENPAL_STRIP_LOGGING

#endif // end include guards
