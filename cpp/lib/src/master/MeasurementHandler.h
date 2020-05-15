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
#ifndef OPENDNP3_MEASUREMENTHANDLER_H
#define OPENDNP3_MEASUREMENTHANDLER_H

#include "app/APDUHeader.h"
#include "app/parsing/Collections.h"
#include "app/parsing/IAPDUHandler.h"
#include "app/parsing/ParseResult.h"
#include "logging/LogMacros.h"

#include "opendnp3/gen/Attributes.h"
#include "opendnp3/logging/LogLevels.h"
#include "opendnp3/logging/Logger.h"
#include "opendnp3/master/ISOEHandler.h"

namespace opendnp3
{

/**
 * Dedicated class for processing response data in the master.
 */
class MeasurementHandler final : public IAPDUHandler
{

public:
    /**
     * Static helper function for interpreting a response as a measurement response
     */
    static ParseResult ProcessMeasurements(ResponseInfo info,
                                           const ser4cpp::rseq_t& objects,
                                           Logger& logger,
                                           ISOEHandler* pHandler);

    // TODO
    virtual bool IsAllowed(uint32_t headerCount, GroupVariation gv, QualifierCode qc) override
    {
        return true;
    };

    /**
     * Creates a new ResponseLoader instance.
     *
     * @param logger	the Logger that the loader should use for message reporting
     */
    MeasurementHandler(ResponseInfo info, const Logger& logger, ISOEHandler* pSOEHandler);

    ~MeasurementHandler();

private:
    ResponseInfo info;
    Logger logger;

    static TimestampQuality ModeFromType(GroupVariation gv);

    IINField ProcessHeader(const CountHeader& header, const ICollection<Group50Var1>& values) override;

    // Handle the CTO objects
    IINField ProcessHeader(const CountHeader& header, const ICollection<Group51Var1>& values) override;
    IINField ProcessHeader(const CountHeader& header, const ICollection<Group51Var2>& values) override;

    IINField ProcessHeader(const RangeHeader& header, const ICollection<Indexed<Binary>>& values) override;
    IINField ProcessHeader(const RangeHeader& header, const ICollection<Indexed<DoubleBitBinary>>& values) override;
    IINField ProcessHeader(const RangeHeader& header, const ICollection<Indexed<BinaryOutputStatus>>& values) override;
    IINField ProcessHeader(const RangeHeader& header, const ICollection<Indexed<Counter>>& values) override;
    IINField ProcessHeader(const RangeHeader& header, const ICollection<Indexed<FrozenCounter>>& values) override;
    IINField ProcessHeader(const RangeHeader& header, const ICollection<Indexed<Analog>>& values) override;
    IINField ProcessHeader(const RangeHeader& header, const ICollection<Indexed<AnalogOutputStatus>>& values) override;
    IINField ProcessHeader(const RangeHeader& header, const ICollection<Indexed<OctetString>>& values) override;
    IINField ProcessHeader(const RangeHeader& header, const ICollection<Indexed<TimeAndInterval>>& values) override;

    IINField ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<Binary>>& values) override;
    IINField ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<BinaryOutputStatus>>& values) override;
    IINField ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<DoubleBitBinary>>& values) override;
    IINField ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<Counter>>& values) override;
    IINField ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<FrozenCounter>>& values) override;
    IINField ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<Analog>>& values) override;
    IINField ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<AnalogOutputStatus>>& values) override;
    IINField ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<OctetString>>& values) override;
    IINField ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<BinaryCommandEvent>>& values) override;
    IINField ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<AnalogCommandEvent>>& values) override;

    template<class Target, class Source>
    IINField LoadValuesWithTransformTo(const HeaderRecord& record, const ICollection<Indexed<Source>>& values)
    {
        auto transform = [](const Indexed<Source>& input) -> Indexed<Target> { return Convert<Source, Target>(input); };

        auto collection = Map<Indexed<Source>, Indexed<Target>>(values, transform);

        return this->LoadValues(record, ModeFromType(record.enumeration), collection);
    }

    template<class T>
    IINField LoadValues(const HeaderRecord& record, TimestampQuality tsquality, const ICollection<Indexed<T>>& values)
    {
        this->CheckForTxStart();
        HeaderInfo info(record.enumeration, record.GetQualifierCode(), tsquality, record.headerIndex);
        this->pSOEHandler->Process(info, values);
        return IINField();
    }

    template<class T> IINField ProcessWithCTO(const HeaderRecord& record, const ICollection<Indexed<T>>& values);

    bool txInitiated;
    ISOEHandler* pSOEHandler;

    DNPTime commonTimeOccurence;

    void CheckForTxStart();

    template<class T, class U> static Indexed<U> Convert(const Indexed<T>& input)
    {
        return WithIndex(Convert(input.value), input.index);
    }
};

template<class T>
IINField MeasurementHandler::ProcessWithCTO(const HeaderRecord& record, const ICollection<Indexed<T>>& values)
{
    if (this->commonTimeOccurence.quality == TimestampQuality::INVALID)
    {
        FORMAT_LOG_BLOCK(logger, flags::WARN, "No prior CTO objects for %s",
                         GroupVariationSpec::to_string(record.enumeration));
        return IINField(IINBit::PARAM_ERROR);
    }

    const auto cto = this->commonTimeOccurence;

    auto transform = [cto](const Indexed<T>& input) -> Indexed<T> {
        Indexed<T> copy(input);
        copy.value.time = DNPTime(input.value.time.value + cto.value, cto.quality);
        return copy;
    };

    auto adjusted = Map<Indexed<T>, Indexed<T>>(values, transform);

    return this->LoadValues(record, cto.quality, adjusted);
}

} // namespace opendnp3

#endif
