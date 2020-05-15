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
#ifndef OPENDNP3_UNITTESTS_MOCK_EVENT_WRITE_HANDLER_H
#define OPENDNP3_UNITTESTS_MOCK_EVENT_WRITE_HANDLER_H

#include <opendnp3/app/EventType.h>
#include <opendnp3/app/MeasurementInfo.h>

#include <outstation/event/IEventWriteHandler.h>

#include <deque>
#include <sstream>
#include <vector>

class MockEventWriteHandler final : public opendnp3::IEventWriteHandler
{
    struct Record
    {
        opendnp3::EventType type;
        uint8_t variation;
        uint16_t count;
    };

    std::deque<Record> expected;

private:
    template<class T> class EventWriterImpl : public opendnp3::IEventWriter<T>
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
    void Expect(opendnp3::EventBinaryVariation variation, uint16_t count)
    {
        this->ExpectType<opendnp3::BinaryInfo>(variation, count);
    }

    void Expect(opendnp3::EventDoubleBinaryVariation variation, uint16_t count)
    {
        this->ExpectType<opendnp3::DoubleBitBinaryInfo>(variation, count);
    }

    void Expect(opendnp3::EventCounterVariation variation, uint16_t count)
    {
        this->ExpectType<opendnp3::CounterInfo>(variation, count);
    }

    void Expect(opendnp3::EventAnalogVariation variation, uint16_t count)
    {
        this->ExpectType<opendnp3::AnalogInfo>(variation, count);
    }

    void AssertEmpty()
    {
        if (!this->expected.empty())
        {
            throw std::logic_error("more events expected");
        }
    }

    virtual uint16_t Write(opendnp3::EventBinaryVariation variation,
                           const opendnp3::Binary& first,
                           opendnp3::IEventCollection<opendnp3::Binary>& items) override
    {
        return this->WriteAny<opendnp3::BinaryInfo>(variation, items);
    }

    virtual uint16_t Write(opendnp3::EventDoubleBinaryVariation variation,
                           const opendnp3::DoubleBitBinary& first,
                           opendnp3::IEventCollection<opendnp3::DoubleBitBinary>& items) override
    {
        return this->WriteAny<opendnp3::DoubleBitBinaryInfo>(variation, items);
    }

    virtual uint16_t Write(opendnp3::EventCounterVariation variation,
                           const opendnp3::Counter& first,
                           opendnp3::IEventCollection<opendnp3::Counter>& items) override
    {
        return this->WriteAny<opendnp3::CounterInfo>(variation, items);
    }

    virtual uint16_t Write(opendnp3::EventFrozenCounterVariation variation,
                           const opendnp3::FrozenCounter& first,
                           opendnp3::IEventCollection<opendnp3::FrozenCounter>& items) override
    {
        return this->WriteAny<opendnp3::FrozenCounterInfo>(variation, items);
    }

    virtual uint16_t Write(opendnp3::EventAnalogVariation variation,
                           const opendnp3::Analog& first,
                           opendnp3::IEventCollection<opendnp3::Analog>& items) override
    {
        return this->WriteAny<opendnp3::AnalogInfo>(variation, items);
    }

    virtual uint16_t Write(opendnp3::EventBinaryOutputStatusVariation variation,
                           const opendnp3::BinaryOutputStatus& first,
                           opendnp3::IEventCollection<opendnp3::BinaryOutputStatus>& items) override
    {
        return this->WriteAny<opendnp3::BinaryOutputStatusInfo>(variation, items);
    }

    virtual uint16_t Write(opendnp3::EventAnalogOutputStatusVariation variation,
                           const opendnp3::AnalogOutputStatus& first,
                           opendnp3::IEventCollection<opendnp3::AnalogOutputStatus>& items) override
    {
        return this->WriteAny<opendnp3::AnalogOutputStatusInfo>(variation, items);
    }

    virtual uint16_t Write(opendnp3::EventOctetStringVariation variation,
                           const opendnp3::OctetString& first,
                           opendnp3::IEventCollection<opendnp3::OctetString>& items) override
    {
        return this->WriteAny<opendnp3::OctetStringInfo>(variation, items);
    }

private:
    template<class T> void ExpectType(typename T::event_variation_t variation, uint16_t count)
    {
        expected.push_back(Record{T::EventTypeEnum, static_cast<uint8_t>(variation), count});
    }

    template<class T>
    uint16_t WriteAny(typename T::event_variation_t variation, opendnp3::IEventCollection<typename T::meas_t>& items);
};

template<class T>
uint16_t MockEventWriteHandler::WriteAny(typename T::event_variation_t variation,
                                         opendnp3::IEventCollection<typename T::meas_t>& items)
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

#endif
