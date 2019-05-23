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
#ifndef OPENDNP3_MEASUREMENTHANDLER_H
#define OPENDNP3_MEASUREMENTHANDLER_H

#include <openpal/logging/LogMacros.h>
#include <openpal/logging/Logger.h>

#include "opendnp3/LogLevels.h"
#include "opendnp3/app/APDUHeader.h"
#include "opendnp3/app/parsing/Collections.h"
#include "opendnp3/app/parsing/IAPDUHandler.h"
#include "opendnp3/app/parsing/ParseResult.h"
#include "opendnp3/gen/Attributes.h"
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
    static ParseResult ProcessMeasurements(const openpal::RSlice& objects,
                                           openpal::Logger& logger,
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
    MeasurementHandler(const openpal::Logger& logger, ISOEHandler* pSOEHandler);

    ~MeasurementHandler();

private:
    openpal::Logger logger;

    static TimestampMode ModeFromType(GroupVariation gv);

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
    IINField ProcessHeader(const RangeHeader& header, const ICollection<Indexed<Group121Var1>>& values) override;

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
    IINField ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<Group122Var1>>& values) override;
    IINField ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<Group122Var2>>& values) override;

    template<class Target, class Source>
    IINField LoadValuesWithTransformTo(const HeaderRecord& record, const ICollection<Indexed<Source>>& values)
    {
        auto transform = [](const Indexed<Source>& input) -> Indexed<Target> { return Convert<Source, Target>(input); };

        auto collection = Map<Indexed<Source>, Indexed<Target>>(values, transform);

        return this->LoadValues(record, ModeFromType(record.enumeration), collection);
    }

    template<class T>
    IINField LoadValues(const HeaderRecord& record, TimestampMode tsmode, const ICollection<Indexed<T>>& values)
    {
        this->CheckForTxStart();
        HeaderInfo info(record.enumeration, record.GetQualifierCode(), tsmode, record.headerIndex);
        this->pSOEHandler->Process(info, values);
        return IINField();
    }

    template<class T> IINField ProcessWithCTO(const HeaderRecord& record, const ICollection<Indexed<T>>& values);

    bool txInitiated;
    ISOEHandler* pSOEHandler;

    TimestampMode ctoMode;
    DNPTime commonTimeOccurence;

    void CheckForTxStart();

    static SecurityStat Convert(const Group121Var1& meas);
    static SecurityStat Convert(const Group122Var1& meas);
    static SecurityStat Convert(const Group122Var2& meas);

    template<class T, class U> static Indexed<U> Convert(const Indexed<T>& input)
    {
        return WithIndex(Convert(input.value), input.index);
    }
};

template<class T>
IINField MeasurementHandler::ProcessWithCTO(const HeaderRecord& record, const ICollection<Indexed<T>>& values)
{
    if (ctoMode == TimestampMode::INVALID)
    {
        FORMAT_LOG_BLOCK(logger, flags::WARN, "No prior CTO objects for %s",
                         GroupVariationToString(record.enumeration));
        return IINField(IINBit::PARAM_ERROR);
    }

    const auto MODE = this->ctoMode;
    const auto cto = this->commonTimeOccurence;

    auto transform = [cto](const Indexed<T>& input) -> Indexed<T> {
        Indexed<T> copy(input);
        copy.value.time = DNPTime(input.value.time + cto);
        return copy;
    };

    auto adjusted = Map<Indexed<T>, Indexed<T>>(values, transform);

    return this->LoadValues(record, MODE, adjusted);
}

} // namespace opendnp3

#endif
