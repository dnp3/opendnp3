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
#ifndef OPENDNP3_PRINTINGSOEHANDLER_H
#define OPENDNP3_PRINTINGSOEHANDLER_H

#include "opendnp3/master/ISOEHandler.h"

#include <iostream>
#include <memory>
#include <sstream>

namespace opendnp3
{

/**
 *	ISOEHandler singleton that prints to the console.
 */
class PrintingSOEHandler final : public ISOEHandler
{

public:
    PrintingSOEHandler() {}

    static std::shared_ptr<ISOEHandler> Create()
    {
        return std::make_shared<PrintingSOEHandler>();
    }

    void BeginFragment(const ResponseInfo& info) override;
    void EndFragment(const ResponseInfo& info) override;

    virtual void Process(const HeaderInfo& info, const ICollection<Indexed<Binary>>& values) override;
    virtual void Process(const HeaderInfo& info, const ICollection<Indexed<DoubleBitBinary>>& values) override;
    virtual void Process(const HeaderInfo& info, const ICollection<Indexed<Analog>>& values) override;
    virtual void Process(const HeaderInfo& info, const ICollection<Indexed<Counter>>& values) override;
    virtual void Process(const HeaderInfo& info, const ICollection<Indexed<FrozenCounter>>& values) override;
    virtual void Process(const HeaderInfo& info, const ICollection<Indexed<BinaryOutputStatus>>& values) override;
    virtual void Process(const HeaderInfo& info, const ICollection<Indexed<AnalogOutputStatus>>& values) override;
    virtual void Process(const HeaderInfo& info, const ICollection<Indexed<OctetString>>& values) override;
    virtual void Process(const HeaderInfo& info, const ICollection<Indexed<TimeAndInterval>>& values) override;
    virtual void Process(const HeaderInfo& info, const ICollection<Indexed<BinaryCommandEvent>>& values) override;
    virtual void Process(const HeaderInfo& info, const ICollection<Indexed<AnalogCommandEvent>>& values) override;
    virtual void Process(const HeaderInfo& info, const ICollection<DNPTime>& values) override;

private:
    template<class T> static void PrintAll(const HeaderInfo& info, const ICollection<Indexed<T>>& values)
    {
        auto print = [&](const Indexed<T>& pair) { Print<T>(info, pair.value, pair.index); };
        values.ForeachItem(print);
    }

    template<class T> static void Print(const HeaderInfo& info, const T& value, uint16_t index)
    {
        std::cout << "[" << index << "] : " << ValueToString(value) << " : " << static_cast<int>(value.flags.value)
                  << " : " << value.time.value << std::endl;
    }

    template<class T> static std::string ValueToString(const T& meas)
    {
        std::ostringstream oss;
        oss << meas.value;
        return oss.str();
    }

    static std::string GetTimeString(TimestampQuality tsquality)
    {
        std::ostringstream oss;
        switch (tsquality)
        {
        case (TimestampQuality::SYNCHRONIZED):
            return "synchronized";
            break;
        case (TimestampQuality::UNSYNCHRONIZED):
            oss << "unsynchronized";
            break;
        default:
            oss << "no timestamp";
            break;
        }

        return oss.str();
    }

    static std::string ValueToString(const DoubleBitBinary& meas)
    {
        return DoubleBitSpec::to_human_string(meas.value);
    }
};

} // namespace opendnp3

#endif
