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
#include "ObjectHeaderParser.h"

#include "app/GroupVariationRecord.h"
#include "logging/LogMacros.h"

#include "opendnp3/logging/LogLevels.h"

#include <ser4cpp/serialization/LittleEndian.h>

namespace opendnp3
{

ObjectHeader::ObjectHeader() : group(0), variation(0), qualifier(0) {}

ParseResult ObjectHeaderParser::ParseObjectHeader(ObjectHeader& header, ser4cpp::rseq_t& buffer, Logger* pLogger)
{
    if (buffer.length() < 3)
    {
        SIMPLE_LOGGER_BLOCK(pLogger, flags::WARN, "Not enough data for header");
        return ParseResult::NOT_ENOUGH_DATA_FOR_HEADER;
    }

    ser4cpp::UInt8::read_from(buffer, header.group);
    ser4cpp::UInt8::read_from(buffer, header.variation);
    ser4cpp::UInt8::read_from(buffer, header.qualifier);
    return ParseResult::OK;
}

bool ObjectHeaderParser::ReadFirstGroupVariation(const ser4cpp::rseq_t& objects, GroupVariation& gv)
{
    ser4cpp::rseq_t copy(objects);
    ObjectHeader oheader;
    if (ObjectHeaderParser::ParseObjectHeader(oheader, copy, nullptr) != ParseResult::OK)
    {
        return false;
    }

    gv = GroupVariationRecord::GetRecord(oheader.group, oheader.variation).enumeration;

    return true;
}

} // namespace opendnp3
