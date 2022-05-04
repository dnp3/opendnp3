/*
 * Copyright 2013-2022 Step Function I/O, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Step Function I/O
 * LLC (https://stepfunc.io) under one or more contributor license agreements.
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
#ifndef OPENDNP3_PARSERESULT_H
#define OPENDNP3_PARSERESULT_H

#include <cstdint>

namespace opendnp3
{

enum class ParseResult : uint8_t
{
    OK,
    NOT_ON_WHITELIST,
    NOT_ENOUGH_DATA_FOR_HEADER,
    NOT_ENOUGH_DATA_FOR_RANGE,
    NOT_ENOUGH_DATA_FOR_OBJECTS,
    UNREASONABLE_OBJECT_COUNT,
    UNKNOWN_OBJECT,
    UNKNOWN_QUALIFIER,
    INVALID_OBJECT_QUALIFIER,
    INVALID_OBJECT,
    BAD_START_STOP,
    COUNT_OF_ZERO
};

inline bool IsSuccess(ParseResult result)
{
    return result == ParseResult::OK;
}
inline bool IsFailure(ParseResult result)
{
    return result != ParseResult::OK;
}

} // namespace opendnp3

#endif
