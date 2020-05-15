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

#include "OctetStringSerializer.h"

namespace opendnp3
{
OctetStringSerializer::OctetStringSerializer(bool isEvent, uint8_t size)
    : DNP3Serializer(GroupVariationID(isEvent ? 111 : 110, size),
                     size,
                     nullptr, // won't be used for reading
                     &OctetStringSerializer::Write)
{
}

bool OctetStringSerializer::Write(const OctetString& value, ser4cpp::wseq_t& buffer)
{
    if (value.Size() > buffer.length())
    {
        return false;
    }
    const auto value_buffer = value.ToBuffer();
    const ser4cpp::rseq_t slice(value_buffer.data, value_buffer.length);
    buffer.copy_from(slice);
    return true;
}
}; // namespace opendnp3
