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
#ifndef OPENDNP3_UNITTESTS_MOCK_APDU_HEADER_HANDLER_H
#define OPENDNP3_UNITTESTS_MOCK_APDU_HEADER_HANDLER_H

#include <app/parsing/IAPDUHandler.h>

#include <vector>

class MockApduHeaderHandler : public opendnp3::IAPDUHandler
{
public:
    bool IsAllowed(uint32_t headerCount, opendnp3::GroupVariation gv, opendnp3::QualifierCode qc) final
    {
        return true;
    }

    void OnHeaderResult(const opendnp3::HeaderRecord& record, const opendnp3::IINField& result) final
    {
        records.push_back(record);
    }

    opendnp3::IINField ProcessHeader(const opendnp3::RangeHeader& header,
                                     const opendnp3::ICollection<opendnp3::Indexed<opendnp3::IINValue>>& values) final
    {
        return ProcessAny(header, values, iinBits);
    }

    opendnp3::IINField ProcessHeader(const opendnp3::RangeHeader& header,
                                     const opendnp3::ICollection<opendnp3::Indexed<opendnp3::Binary>>& values) final
    {
        return ProcessAny(header, values, staticBinaries);
    }

    opendnp3::IINField ProcessHeader(
        const opendnp3::RangeHeader& header,
        const opendnp3::ICollection<opendnp3::Indexed<opendnp3::DoubleBitBinary>>& values) final
    {
        return ProcessAny(header, values, staticDoubleBinaries);
    }

    opendnp3::IINField ProcessHeader(
        const opendnp3::RangeHeader& header,
        const opendnp3::ICollection<opendnp3::Indexed<opendnp3::BinaryOutputStatus>>& values) final
    {
        return ProcessAny(header, values, staticControlStatii);
    }

    opendnp3::IINField ProcessHeader(const opendnp3::RangeHeader& header,
                                     const opendnp3::ICollection<opendnp3::Indexed<opendnp3::Counter>>& values) final
    {
        return ProcessAny(header, values, staticCounters);
    }

    opendnp3::IINField ProcessHeader(
        const opendnp3::RangeHeader& header,
        const opendnp3::ICollection<opendnp3::Indexed<opendnp3::FrozenCounter>>& values) final
    {
        return ProcessAny(header, values, staticFrozenCounters);
    }

    opendnp3::IINField ProcessHeader(const opendnp3::RangeHeader& header,
                                     const opendnp3::ICollection<opendnp3::Indexed<opendnp3::Analog>>& values) final
    {
        return ProcessAny(header, values, eventAnalogs);
    }

    opendnp3::IINField ProcessHeader(
        const opendnp3::RangeHeader& header,
        const opendnp3::ICollection<opendnp3::Indexed<opendnp3::AnalogOutputStatus>>& values) final
    {
        return ProcessAny(header, values, staticSetpointStatii);
    }

    opendnp3::IINField ProcessHeader(
        const opendnp3::RangeHeader& header,
        const opendnp3::ICollection<opendnp3::Indexed<opendnp3::OctetString>>& values) final
    {
        return ProcessAny(header, values, rangedOctets);
    }

    opendnp3::IINField ProcessHeader(const opendnp3::PrefixHeader& header,
                                     const opendnp3::ICollection<opendnp3::Indexed<opendnp3::Binary>>& values) final
    {
        return this->ProcessAny(header, values, eventBinaries);
    }

    opendnp3::IINField ProcessHeader(
        const opendnp3::PrefixHeader& header,
        const opendnp3::ICollection<opendnp3::Indexed<opendnp3::DoubleBitBinary>>& values) final
    {
        return this->ProcessAny(header, values, eventDoubleBinaries);
    }

    opendnp3::IINField ProcessHeader(const opendnp3::PrefixHeader& header,
                                     const opendnp3::ICollection<opendnp3::Indexed<opendnp3::Counter>>& values) final
    {
        return this->ProcessAny(header, values, eventCounters);
    }

    opendnp3::IINField ProcessHeader(
        const opendnp3::PrefixHeader& header,
        const opendnp3::ICollection<opendnp3::Indexed<opendnp3::FrozenCounter>>& values) final
    {
        return this->ProcessAny(header, values, eventFrozenCounters);
    }

    opendnp3::IINField ProcessHeader(const opendnp3::PrefixHeader& header,
                                     const opendnp3::ICollection<opendnp3::Indexed<opendnp3::Analog>>& values) final
    {
        return this->ProcessAny(header, values, eventAnalogs);
    }

    opendnp3::IINField ProcessHeader(
        const opendnp3::PrefixHeader& header,
        const opendnp3::ICollection<opendnp3::Indexed<opendnp3::OctetString>>& values) final
    {
        return this->ProcessAny(header, values, indexPrefixedOctets);
    }

    opendnp3::IINField ProcessHeader(
        const opendnp3::PrefixHeader& header,
        const opendnp3::ICollection<opendnp3::Indexed<opendnp3::BinaryCommandEvent>>& values) final
    {
        return this->ProcessAny(header, values, binaryCommandEvents);
    }

    opendnp3::IINField ProcessHeader(
        const opendnp3::PrefixHeader& header,
        const opendnp3::ICollection<opendnp3::Indexed<opendnp3::AnalogCommandEvent>>& values) final
    {
        return this->ProcessAny(header, values, analogCommandEvents);
    }

    opendnp3::IINField ProcessHeader(const opendnp3::PrefixHeader& header,
                                     const opendnp3::ICollection<uint16_t>& values) final
    {
        return this->ProcessAny(header, values, indices);
    }

    /// --- controls ----

    opendnp3::IINField ProcessHeader(
        const opendnp3::PrefixHeader& header,
        const opendnp3::ICollection<opendnp3::Indexed<opendnp3::ControlRelayOutputBlock>>& meas) final
    {
        return this->ProcessAny(header, meas, crobRequests);
    }

    opendnp3::IINField ProcessHeader(
        const opendnp3::PrefixHeader& header,
        const opendnp3::ICollection<opendnp3::Indexed<opendnp3::AnalogOutputInt16>>& meas) final
    {
        return this->ProcessAny(header, meas, aoInt16Requests);
    }

    opendnp3::IINField ProcessHeader(
        const opendnp3::PrefixHeader& header,
        const opendnp3::ICollection<opendnp3::Indexed<opendnp3::AnalogOutputInt32>>& meas) final
    {
        return this->ProcessAny(header, meas, aoInt32Requests);
    }

    opendnp3::IINField ProcessHeader(
        const opendnp3::PrefixHeader& header,
        const opendnp3::ICollection<opendnp3::Indexed<opendnp3::AnalogOutputFloat32>>& meas) final
    {
        return this->ProcessAny(header, meas, aoFloat32Requests);
    }

    opendnp3::IINField ProcessHeader(
        const opendnp3::PrefixHeader& header,
        const opendnp3::ICollection<opendnp3::Indexed<opendnp3::AnalogOutputDouble64>>& meas) final
    {
        return this->ProcessAny(header, meas, aoDouble64Requests);
    }

    std::vector<opendnp3::HeaderRecord> records;

    std::vector<opendnp3::Indexed<opendnp3::IINValue>> iinBits;

    std::vector<opendnp3::Indexed<opendnp3::Binary>> eventBinaries;
    std::vector<opendnp3::Indexed<opendnp3::Binary>> staticBinaries;

    std::vector<opendnp3::Indexed<opendnp3::DoubleBitBinary>> eventDoubleBinaries;
    std::vector<opendnp3::Indexed<opendnp3::DoubleBitBinary>> staticDoubleBinaries;

    std::vector<opendnp3::Indexed<opendnp3::BinaryOutputStatus>> staticControlStatii;

    std::vector<opendnp3::Indexed<opendnp3::Counter>> eventCounters;
    std::vector<opendnp3::Indexed<opendnp3::Counter>> staticCounters;

    std::vector<opendnp3::Indexed<opendnp3::FrozenCounter>> eventFrozenCounters;
    std::vector<opendnp3::Indexed<opendnp3::FrozenCounter>> staticFrozenCounters;

    std::vector<opendnp3::Indexed<opendnp3::Analog>> eventAnalogs;
    std::vector<opendnp3::Indexed<opendnp3::Analog>> staticAnalogs;

    std::vector<opendnp3::Indexed<opendnp3::AnalogOutputStatus>> staticSetpointStatii;

    std::vector<opendnp3::Indexed<opendnp3::ControlRelayOutputBlock>> crobRequests;

    std::vector<opendnp3::Indexed<opendnp3::AnalogOutputInt16>> aoInt16Requests;
    std::vector<opendnp3::Indexed<opendnp3::AnalogOutputInt32>> aoInt32Requests;
    std::vector<opendnp3::Indexed<opendnp3::AnalogOutputFloat32>> aoFloat32Requests;
    std::vector<opendnp3::Indexed<opendnp3::AnalogOutputDouble64>> aoDouble64Requests;

    std::vector<opendnp3::Indexed<opendnp3::OctetString>> indexPrefixedOctets;
    std::vector<opendnp3::Indexed<opendnp3::OctetString>> rangedOctets;

    std::vector<opendnp3::Indexed<opendnp3::BinaryCommandEvent>> binaryCommandEvents;

    std::vector<opendnp3::Indexed<opendnp3::AnalogCommandEvent>> analogCommandEvents;

    std::vector<uint16_t> indices;

private:
    template<class T>
    opendnp3::IINField ProcessAny(const opendnp3::HeaderRecord& record,
                                  const opendnp3::ICollection<T>& meas,
                                  std::vector<T>& items)
    {
        auto add = [&items](const T& v) { items.push_back(v); };
        meas.ForeachItem(add);
        return opendnp3::IINField::Empty();
    }
};

#endif
