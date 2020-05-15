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
#ifndef OPENDNP3_UNITTESTS_MOCKSOEHANDLER_H
#define OPENDNP3_UNITTESTS_MOCKSOEHANDLER_H

#include <opendnp3/master/ISOEHandler.h>

#include <map>
#include <vector>

// simple measurement handler for testing purposes
class MockSOEHandler : public opendnp3::ISOEHandler
{
public:
    template<class T> class Record
    {
    public:
        Record() : sequence(0) {}

        Record(const T& meas_, const opendnp3::HeaderInfo& info_, uint32_t sequence_)
            : meas(meas_), info(info_), sequence(sequence_)
        {
        }

        T meas;
        opendnp3::HeaderInfo info;
        uint32_t sequence;
    };

    MockSOEHandler() : soeCount(0) {}

    void BeginFragment(const opendnp3::ResponseInfo& info) override {}

    void EndFragment(const opendnp3::ResponseInfo& info) override {}

    uint32_t TotalReceived() const
    {
        return soeCount;
    }

    void Process(const opendnp3::HeaderInfo& info,
                 const opendnp3::ICollection<opendnp3::Indexed<opendnp3::Binary>>& values) final
    {
        this->RecordAny(info, values, this->binarySOE);
    }

    void Process(const opendnp3::HeaderInfo& info,
                 const opendnp3::ICollection<opendnp3::Indexed<opendnp3::DoubleBitBinary>>& values) final
    {
        this->RecordAny(info, values, this->doubleBinarySOE);
    }

    void Process(const opendnp3::HeaderInfo& info,
                 const opendnp3::ICollection<opendnp3::Indexed<opendnp3::Analog>>& values) final
    {
        this->RecordAny(info, values, this->analogSOE);
    }

    void Process(const opendnp3::HeaderInfo& info,
                 const opendnp3::ICollection<opendnp3::Indexed<opendnp3::Counter>>& values) final
    {
        this->RecordAny(info, values, this->counterSOE);
    }

    void Process(const opendnp3::HeaderInfo& info,
                 const opendnp3::ICollection<opendnp3::Indexed<opendnp3::FrozenCounter>>& values) final
    {
        this->RecordAny(info, values, this->frozenCounterSOE);
    }

    void Process(const opendnp3::HeaderInfo& info,
                 const opendnp3::ICollection<opendnp3::Indexed<opendnp3::BinaryOutputStatus>>& values) final
    {
        this->RecordAny(info, values, this->binaryOutputStatusSOE);
    }

    void Process(const opendnp3::HeaderInfo& info,
                 const opendnp3::ICollection<opendnp3::Indexed<opendnp3::AnalogOutputStatus>>& values) final
    {
        this->RecordAny(info, values, this->analogOutputStatusSOE);
    }

    void Process(const opendnp3::HeaderInfo& info,
                 const opendnp3::ICollection<opendnp3::Indexed<opendnp3::OctetString>>& values) final
    {
        this->RecordAny(info, values, this->octetStringSOE);
    }

    void Process(const opendnp3::HeaderInfo& info,
                 const opendnp3::ICollection<opendnp3::Indexed<opendnp3::TimeAndInterval>>& values) final
    {
        this->RecordAny(info, values, this->timeAndIntervalSOE);
    }

    void Process(const opendnp3::HeaderInfo& info,
                 const opendnp3::ICollection<opendnp3::Indexed<opendnp3::BinaryCommandEvent>>& values) final
    {
        this->RecordAny(info, values, this->binaryCommandEventSOE);
    }

    void Process(const opendnp3::HeaderInfo& info,
                 const opendnp3::ICollection<opendnp3::Indexed<opendnp3::AnalogCommandEvent>>& values) final
    {
        this->RecordAny(info, values, this->analogCommandEventSOE);
    }

    void Process(const opendnp3::HeaderInfo& info, const opendnp3::ICollection<opendnp3::DNPTime>& values) final
    {
        values.ForeachItem([this](const opendnp3::DNPTime& value) {
            ++this->soeCount;
            this->timeSOE.push_back(value);
        });
    }

    void Clear()
    {
        soeCount = 0;

        binarySOE.clear();
        doubleBinarySOE.clear();
        analogSOE.clear();
        counterSOE.clear();
        frozenCounterSOE.clear();
        binaryOutputStatusSOE.clear();
        analogOutputStatusSOE.clear();
        octetStringSOE.clear();
        timeAndIntervalSOE.clear();
        binaryCommandEventSOE.clear();
        analogCommandEventSOE.clear();
        timeSOE.clear();
    }

    std::map<uint16_t, Record<opendnp3::Binary>> binarySOE;
    std::map<uint16_t, Record<opendnp3::DoubleBitBinary>> doubleBinarySOE;
    std::map<uint16_t, Record<opendnp3::Analog>> analogSOE;
    std::map<uint16_t, Record<opendnp3::Counter>> counterSOE;
    std::map<uint16_t, Record<opendnp3::FrozenCounter>> frozenCounterSOE;
    std::map<uint16_t, Record<opendnp3::BinaryOutputStatus>> binaryOutputStatusSOE;
    std::map<uint16_t, Record<opendnp3::AnalogOutputStatus>> analogOutputStatusSOE;
    std::map<uint16_t, Record<opendnp3::OctetString>> octetStringSOE;
    std::map<uint16_t, Record<opendnp3::TimeAndInterval>> timeAndIntervalSOE;
    std::map<uint16_t, Record<opendnp3::BinaryCommandEvent>> binaryCommandEventSOE;
    std::map<uint16_t, Record<opendnp3::AnalogCommandEvent>> analogCommandEventSOE;
    std::vector<opendnp3::DNPTime> timeSOE;

private:
    uint32_t soeCount;

    template<class T>
    void RecordAny(const opendnp3::HeaderInfo& info,
                   const opendnp3::ICollection<opendnp3::Indexed<T>>& values,
                   std::map<uint16_t, Record<T>>& records)
    {
        auto process = [this, info, &records](const opendnp3::Indexed<T>& pair) {
            Record<T> record(pair.value, info, soeCount);
            records[pair.index] = record;
            ++this->soeCount;
        };

        values.ForeachItem(process);
    }
};

#endif
