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

#ifndef OPENDNP3_EVENTWRITERS_H
#define OPENDNP3_EVENTWRITERS_H

#include "IEventWriteHandler.h"
#include "app/DNP3Serializer.h"
#include "app/HeaderWriter.h"
#include "gen/objects/Group51.h"

#include <ser4cpp/serialization/LittleEndian.h>

namespace opendnp3
{

class EventWriters
{

public:
    template<class T>
    static uint16_t Write(HeaderWriter& writer, IEventCollection<T>& items, const DNP3Serializer<T>& serializer)
    {
        BasicEventWriter<T> handler(writer, serializer);
        return items.WriteSome(handler);
    }

    template<class T>
    static uint16_t WriteWithCTO(const DNPTime& cto,
                                 HeaderWriter& writer,
                                 IEventCollection<T>& items,
                                 const DNP3Serializer<T>& serializer)
    {
        if (cto.quality == TimestampQuality::SYNCHRONIZED)
        {
            Group51Var1 value;
            value.time = cto;
            CTOEventWriter<T, Group51Var1> handler(value, writer, serializer);
            return items.WriteSome(handler);
        }
        else
        {
            Group51Var2 value;
            value.time = cto;
            CTOEventWriter<T, Group51Var2> handler(value, writer, serializer);
            return items.WriteSome(handler);
        }
    }

    static uint16_t Write(uint8_t firstSize, HeaderWriter& writer, IEventCollection<OctetString>& items);

private:
    template<class T> class BasicEventWriter final : public IEventWriter<T>
    {
        PrefixedWriteIterator<ser4cpp::UInt16, T> iterator;

    public:
        BasicEventWriter(HeaderWriter& writer, const DNP3Serializer<T>& serializer)
            : iterator(writer.IterateOverCountWithPrefix<ser4cpp::UInt16, T>(QualifierCode::UINT16_CNT_UINT16_INDEX,
                                                                             serializer))
        {
        }

        virtual bool Write(const T& meas, uint16_t index) override
        {
            return iterator.IsValid() ? iterator.Write(meas, index) : false;
        }
    };

    template<class T, class U> class CTOEventWriter final : public IEventWriter<T>
    {
        const DNPTime cto;
        PrefixedWriteIterator<ser4cpp::UInt16, T> iterator;

    public:
        CTOEventWriter(const U& cto, HeaderWriter& writer, const DNP3Serializer<T>& serializer)
            : cto(cto.time),
              iterator(writer.IterateOverCountWithPrefixAndCTO<ser4cpp::UInt16, T, U>(
                  QualifierCode::UINT16_CNT_UINT16_INDEX, serializer, cto))
        {
        }

        virtual bool Write(const T& meas, uint16_t index) override
        {
            if (!this->iterator.IsValid())
                return false;

            // Check that the quality of the measurement fits with the CTO variation
            if (this->cto.quality == TimestampQuality::SYNCHRONIZED)
            {
                if (meas.time.quality != TimestampQuality::SYNCHRONIZED)
                    return false;
            }
            else
            {
                if (meas.time.quality == TimestampQuality::SYNCHRONIZED)
                    return false;
            }

            // can't encode timestamps that go backwards
            if (meas.time.value < this->cto.value)
                return false;

            const auto diff = meas.time.value - this->cto.value;

            // can't encode timestamps where the diff is greater than uint16_t
            if (diff > ser4cpp::UInt16::max_value)
                return false;

            auto copy = meas;
            copy.time = DNPTime(diff);

            return this->iterator.Write(copy, index);
        }
    };
};

} // namespace opendnp3

#endif
