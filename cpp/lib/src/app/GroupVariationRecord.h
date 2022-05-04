/*
 * Copyright 2013-2022 Step Function I/O, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Step Function I/O
 * LLC (www.automatak.com) under one or more contributor license agreements.
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
#ifndef OPENDNP3_GROUPVARIATIONRECORD_H
#define OPENDNP3_GROUPVARIATIONRECORD_H

#include "app/Range.h"

#include "opendnp3/gen/GroupVariation.h"
#include "opendnp3/gen/QualifierCode.h"
#include "opendnp3/gen/TimestampQuality.h"
#include "opendnp3/util/Uncopyable.h"

#include <cstdint>

namespace opendnp3
{

enum class GroupVariationType : int
{
    STATIC,
    EVENT,
    OTHER
};

struct EnumAndType
{
    EnumAndType(GroupVariation enumeration_, GroupVariationType type_) : enumeration(enumeration_), type(type_) {}

    GroupVariation enumeration;
    GroupVariationType type;
};

class GroupVariationRecord
{

public:
    static EnumAndType GetEnumAndType(uint8_t group, uint8_t variation);

    static uint16_t GetGroupVar(uint8_t group, uint8_t variation);

    static GroupVariationRecord GetRecord(uint8_t group, uint8_t variation);

    static GroupVariationType GetType(uint8_t group, uint8_t variation);

    GroupVariationRecord(uint8_t group_, uint8_t variation_, GroupVariation enumeration_, GroupVariationType type_);

    GroupVariationRecord()
        : enumeration(GroupVariation::UNKNOWN), type(GroupVariationType::OTHER), group(0), variation(0)
    {
    }

    GroupVariation enumeration;
    GroupVariationType type;
    uint8_t group;
    uint8_t variation;
};

class HeaderRecord : public GroupVariationRecord
{
public:
    HeaderRecord() : qualifier(0), headerIndex(0) {}

    HeaderRecord(const GroupVariationRecord& gv, uint8_t qualifier, uint32_t headerIndex);

    QualifierCode GetQualifierCode() const;

    uint8_t qualifier;
    uint32_t headerIndex;
};

// ---- Specific header types  ---

class AllObjectsHeader : public HeaderRecord
{
public:
    explicit AllObjectsHeader(const HeaderRecord& record) : HeaderRecord(record) {}
};

class CountHeader : public HeaderRecord
{
public:
    CountHeader(const HeaderRecord& record, uint16_t count_) : HeaderRecord(record), count(count_) {}

    uint16_t count;
};

class FreeFormatHeader : public HeaderRecord
{
public:
    FreeFormatHeader(const HeaderRecord& record, uint16_t count_) : HeaderRecord(record), count(count_) {}

    uint16_t count;
};

class RangeHeader : public HeaderRecord
{
public:
    RangeHeader(const HeaderRecord& record, const Range& range_) : HeaderRecord(record), range(range_) {}

    Range range;
};

class PrefixHeader : public HeaderRecord
{
public:
    PrefixHeader(const HeaderRecord& record, uint16_t count_) : HeaderRecord(record), count(count_) {}

    uint16_t count;
};

} // namespace opendnp3

#endif
