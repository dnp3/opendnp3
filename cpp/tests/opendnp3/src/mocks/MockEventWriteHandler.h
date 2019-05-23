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

#include "opendnp3/app/EventType.h"
#include "opendnp3/app/MeasurementTypeSpecs.h"
#include "opendnp3/outstation/event/IEventWriteHandler.h"

#include <deque>
#include <sstream>
#include <vector>

namespace opendnp3
{
class MockEventWriteHandler final : public IEventWriteHandler
{
    struct Record
    {
        EventType type;
        uint8_t variation;
        uint16_t count;
    };

    std::deque<Record> expected;

private:
    template<class T> class EventWriterImpl : public IEventWriter<T>
    {
    public:
        bool success = true;
        std::vector<uint16_t> written_indices;

        virtual bool Write(const T& meas, uint16_t index) override
        {
            if (success)
            {
                written_indices.push_back(index);
            }
            return success;
        }
    };

public:
    void Expect(EventBinaryVariation variation, uint16_t count)
    {
        this->ExpectType<BinarySpec>(variation, count);
    }

    void Expect(EventDoubleBinaryVariation variation, uint16_t count)
    {
        this->ExpectType<DoubleBitBinarySpec>(variation, count);
    }

    void Expect(EventCounterVariation variation, uint16_t count)
    {
        this->ExpectType<CounterSpec>(variation, count);
    }

    void Expect(EventAnalogVariation variation, uint16_t count)
    {
        this->ExpectType<AnalogSpec>(variation, count);
    }

    void AssertEmpty()
    {
        if (!this->expected.empty())
        {
            throw std::logic_error("more events expected");
        }
    }

    virtual uint16_t Write(EventBinaryVariation variation,
                           const Binary& first,
                           IEventCollection<Binary>& items) override
    {
        return this->WriteAny<BinarySpec>(variation, items);
    }

    virtual uint16_t Write(EventDoubleBinaryVariation variation,
                           const DoubleBitBinary& first,
                           IEventCollection<DoubleBitBinary>& items) override
    {
        return this->WriteAny<DoubleBitBinarySpec>(variation, items);
    }

    virtual uint16_t Write(EventCounterVariation variation,
                           const Counter& first,
                           IEventCollection<Counter>& items) override
    {
        return this->WriteAny<CounterSpec>(variation, items);
    }

    virtual uint16_t Write(EventFrozenCounterVariation variation,
                           const FrozenCounter& first,
                           IEventCollection<FrozenCounter>& items) override
    {
        return this->WriteAny<FrozenCounterSpec>(variation, items);
    }

    virtual uint16_t Write(EventAnalogVariation variation,
                           const Analog& first,
                           IEventCollection<Analog>& items) override
    {
        return this->WriteAny<AnalogSpec>(variation, items);
    }

    virtual uint16_t Write(EventBinaryOutputStatusVariation variation,
                           const BinaryOutputStatus& first,
                           IEventCollection<BinaryOutputStatus>& items) override
    {
        return this->WriteAny<BinaryOutputStatusSpec>(variation, items);
    }

    virtual uint16_t Write(EventAnalogOutputStatusVariation variation,
                           const AnalogOutputStatus& first,
                           IEventCollection<AnalogOutputStatus>& items) override
    {
        return this->WriteAny<AnalogOutputStatusSpec>(variation, items);
    }

    virtual uint16_t Write(EventOctetStringVariation variation,
                           const OctetString& first,
                           IEventCollection<OctetString>& items) override
    {
        return this->WriteAny<OctetStringSpec>(variation, items);
    }

private:
    template<class T> void ExpectType(typename T::event_variation_t variation, uint16_t count)
    {
        expected.push_back(Record{T::EventTypeEnum, static_cast<uint8_t>(variation), count});
    }

    template<class T>
    uint16_t WriteAny(typename T::event_variation_t variation, IEventCollection<typename T::meas_t>& items);
};

template<class T>
uint16_t MockEventWriteHandler::WriteAny(typename T::event_variation_t variation,
                                         IEventCollection<typename T::meas_t>& items)
{
    if (this->expected.empty())
    {
        throw std::logic_error("no more write events expected");
    }

    const auto record = this->expected.front();
    this->expected.pop_front();

    if (record.type != T::EventTypeEnum)
    {
        std::ostringstream oss;
        oss << "Unexpected event type: " << static_cast<int>(record.type);
        throw std::logic_error(oss.str());
    }

    if (record.variation != static_cast<uint8_t>(variation))
    {
        std::ostringstream oss;
        oss << "Unexpected variation: " << static_cast<int>(variation);
        throw std::logic_error(oss.str());
    }

    EventWriterImpl<typename T::meas_t> writer;
    const uint16_t count = items.WriteSome(writer);

    if (record.count != count)
    {
        std::ostringstream oss;
        oss << "Unexpected count: " << static_cast<int>(variation);
        throw std::logic_error(oss.str());
    }

    return count;
}
} // namespace opendnp3
