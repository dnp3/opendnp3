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
#include "HeaderWriter.h"

#include <openpal/serialization/Serialization.h>

#include <cassert>

using namespace openpal;

namespace opendnp3
{

HeaderWriter::HeaderWriter(openpal::WSlice* position_) : position(position_) {}

uint32_t HeaderWriter::Remaining() const
{
    return position->Size();
}

void HeaderWriter::Mark()
{
    mark.Set(*position);
}

bool HeaderWriter::Rollback()
{
    if (mark.IsSet())
    {
        *position = mark.Get();
        mark.Clear();
        return true;
    }

    return false;
}

bool HeaderWriter::WriteHeader(GroupVariationID id, QualifierCode qc)
{
    if (position->Size() < 3)
    {
        return false;
    }

    UInt8::WriteBuffer(*position, id.group);
    UInt8::WriteBuffer(*position, id.variation);
    UInt8::WriteBuffer(*position, QualifierCodeToType(qc));
    return true;
}

bool HeaderWriter::WriteHeaderWithReserve(GroupVariationID id, QualifierCode qc, uint32_t reserve)
{
    return (position->Size() < (3 + reserve)) ? false : WriteHeader(id, qc);
}

bool HeaderWriter::WriteFreeFormat(const IVariableLength& value)
{
    uint32_t reserveSize = 1 + openpal::UInt16::SIZE + value.Size();
    if (this->WriteHeaderWithReserve(value.InstanceID(), QualifierCode::UINT16_FREE_FORMAT, reserveSize))
    {
        openpal::UInt8::WriteBuffer(*position, 1);
        openpal::UInt16::WriteBuffer(*position, value.Size());
        return value.Write(*position);
    }

    return false;
}

} // namespace opendnp3
