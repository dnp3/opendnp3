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
#ifndef OPENDNP3_UNITTESTS_LINK_PARSER_TEST_H
#define OPENDNP3_UNITTESTS_LINK_PARSER_TEST_H

#include "utils/BufferHelpers.h"

#include <opendnp3/logging/LogLevels.h>

#include "dnp3mocks/MockFrameSink.h"
#include "dnp3mocks/MockLogHandler.h"

#include <link/LinkLayerParser.h>

#include <cassert>
#include <cstring>

class LinkParserTest
{
public:
    LinkParserTest(bool aImmediate = false) : log(), sink(), parser(log.logger) {}

    void WriteData(const ser4cpp::rseq_t& input)
    {
        auto buff = parser.WriteBuff();
        assert(input.length() <= buff.length());
        buff.copy_from(input);
        parser.OnRead(input.length(), sink);
    }

    void WriteData(const std::string& hex)
    {
        HexSequence hs(hex);
        auto buff = parser.WriteBuff();
        assert(hs.Size() <= buff.length());
        memcpy(buff, hs, hs.Size());
        parser.OnRead(hs.Size(), sink);
    }

    MockLogHandler log;
    MockFrameSink sink;
    opendnp3::LinkLayerParser parser;
};

#endif
