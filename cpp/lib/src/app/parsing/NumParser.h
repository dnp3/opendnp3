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
#ifndef OPENDNP3_NUMPARSER_H
#define OPENDNP3_NUMPARSER_H

#include "app/Range.h"
#include "app/parsing/ParseResult.h"

#include "opendnp3/logging/Logger.h"

#include <ser4cpp/container/SequenceTypes.h>

namespace opendnp3
{

// A one or two byte unsigned integer parser
class NumParser
{
    // a function that consumes bytes from a buffer and returns a uint16_t count
    typedef uint16_t (*ReadFun)(ser4cpp::rseq_t& buffer);

public:
    uint8_t NumBytes() const;

    ParseResult ParseCount(ser4cpp::rseq_t& buffer, uint16_t& count, Logger* pLogger) const;
    ParseResult ParseRange(ser4cpp::rseq_t& buffer, Range& range, Logger* pLogger) const;

    uint16_t ReadNum(ser4cpp::rseq_t& buffer) const;

    static NumParser OneByte();
    static NumParser TwoByte();

private:
    // read the number, consuming from the buffer
    // return true if there is enough bytes, false otherwise
    bool Read(uint16_t& num, ser4cpp::rseq_t& buffer) const;

    static uint16_t ReadOneByte(ser4cpp::rseq_t& buffer);
    static uint16_t ReadTwoBytes(ser4cpp::rseq_t& buffer);

    NumParser(ReadFun pReadFun, uint8_t size);

    ReadFun pReadFun;
    uint8_t size;

    NumParser() = delete;
};

} // namespace opendnp3

#endif
