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
#include "HeaderWriter.h"

#include <ser4cpp/serialization/LittleEndian.h>

#include <cassert>

namespace opendnp3
{

HeaderWriter::HeaderWriter(ser4cpp::wseq_t* position_) : position(position_) {}

size_t HeaderWriter::Remaining() const
{
    return position->length();
}

void HeaderWriter::Mark()
{
    mark.set(*position);
}

bool HeaderWriter::Rollback()
{
    if (mark.is_set())
    {
        *position = mark.get();
        mark.clear();
        return true;
    }

    return false;
}

bool HeaderWriter::WriteHeader(GroupVariationID id, QualifierCode qc)
{
    if (position->length() < 3)
    {
        return false;
    }

    ser4cpp::UInt8::write_to(*position, id.group);
    ser4cpp::UInt8::write_to(*position, id.variation);
    ser4cpp::UInt8::write_to(*position, QualifierCodeSpec::to_type(qc));
    return true;
}

bool HeaderWriter::WriteHeaderWithReserve(GroupVariationID id, QualifierCode qc, size_t reserve)
{
    return (position->length() < (3 + reserve)) ? false : WriteHeader(id, qc);
}

} // namespace opendnp3
