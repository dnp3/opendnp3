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

#include "EventWriters.h"

#include "outstation/OctetStringSerializer.h"

namespace opendnp3
{

class OctetStringEventWriter : public IEventWriter<OctetString>
{
    const OctetStringSerializer serializer;
    PrefixedWriteIterator<ser4cpp::UInt16, OctetString> iterator;

public:
    OctetStringEventWriter(HeaderWriter& writer, uint8_t size)
        : serializer(true, size),
          iterator(
              writer.IterateOverCountWithPrefix<ser4cpp::UInt16>(QualifierCode::UINT16_CNT_UINT16_INDEX, serializer))
    {
    }

    bool Write(const OctetString& meas, uint16_t index) override
    {
        if (meas.Size() != this->serializer.get_size())
            return false;

        return iterator.Write(meas, index);
    }
};

uint16_t EventWriters::Write(uint8_t firstSize, HeaderWriter& writer, IEventCollection<OctetString>& items)
{
    OctetStringEventWriter handler(writer, firstSize);
    return items.WriteSome(handler);
}

} // namespace opendnp3
