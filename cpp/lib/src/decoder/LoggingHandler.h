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
#ifndef OPENDNP3_LOGGINGHANDLER_H
#define OPENDNP3_LOGGINGHANDLER_H

#include "app/parsing/IAPDUHandler.h"
#include "decoder/Indent.h"
#include "logging/LogMacros.h"

#include "opendnp3/decoder/IDecoderCallbacks.h"
#include "opendnp3/gen/Attributes.h"
#include "opendnp3/logging/LogLevels.h"
#include "opendnp3/logging/Logger.h"

#include <cstdint>
#include <sstream>

namespace opendnp3
{
// stand-alone DNP3 decoder
class LoggingHandler final : public IAPDUHandler
{
public:
    LoggingHandler(const Logger& logger, IDecoderCallbacks& callbacks);

private:
    virtual void OnHeaderResult(const HeaderRecord& record, const IINField& result) override;

    static std::string ToUTCString(const DNPTime& time);

    Logger logger;
    IDecoderCallbacks* callbacks;

    static const char* GetStringValue(bool value)
    {
        return value ? "1" : "0";
    }

    static std::string ToHex(uint8_t b)
    {
        std::ostringstream oss;
        oss << ser4cpp::HexConversions::to_hex_char((b & 0xf0) >> 4) << ser4cpp::HexConversions::to_hex_char(b & 0xf);
        return oss.str();
    }

    /// --- templated helpers ---

    template<class T> IINField PrintV(const ICollection<Indexed<T>>& items);

    template<class T> IINField PrintTime(const ICollection<T>& items);

    template<class T> IINField PrintTime16(const ICollection<T>& items);

    template<class T> IINField PrintAO(const ICollection<Indexed<T>>& items);

    IINField PrintCrob(const ICollection<Indexed<ControlRelayOutputBlock>>& items);
    IINField PrintOctets(const ICollection<Indexed<OctetString>>& items);
    IINField PrintTimeAndInterval(const ICollection<Indexed<TimeAndInterval>>& values);

    template<class T> IINField PrintVQT(GroupVariation gv, const ICollection<Indexed<T>>& items);

    template<class T> IINField PrintSecStat(const ICollection<Indexed<T>>& values);

    template<class T, class Stringify>
    IINField PrintVQTStringify(GroupVariation gv, const ICollection<Indexed<T>>& items, const Stringify& stringify);

    /// --- Implement IAPDUHandler ---

    virtual bool IsAllowed(uint32_t, GroupVariation, QualifierCode) override
    {
        return true;
    }

    virtual IINField ProcessHeader(const AllObjectsHeader& record) override
    {
        return IINField::Empty();
    }
    virtual IINField ProcessHeader(const RangeHeader& header) override
    {
        return IINField::Empty();
    }
    virtual IINField ProcessHeader(const CountHeader& header) override
    {
        return IINField::Empty();
    }

    virtual IINField ProcessHeader(const CountHeader& header, const ICollection<Group50Var1>& values) override;
    virtual IINField ProcessHeader(const CountHeader& header, const ICollection<Group51Var1>& values) override;
    virtual IINField ProcessHeader(const CountHeader& header, const ICollection<Group51Var2>& values) override;
    virtual IINField ProcessHeader(const CountHeader& header, const ICollection<Group52Var1>& values) override;
    virtual IINField ProcessHeader(const CountHeader& header, const ICollection<Group52Var2>& values) override;

    virtual IINField ProcessHeader(const RangeHeader& header, const ICollection<Indexed<IINValue>>& values) override;
    virtual IINField ProcessHeader(const RangeHeader& header, const ICollection<Indexed<Binary>>& values) override;
    virtual IINField ProcessHeader(const RangeHeader& header,
                                   const ICollection<Indexed<DoubleBitBinary>>& values) override;
    virtual IINField ProcessHeader(const RangeHeader& header,
                                   const ICollection<Indexed<BinaryOutputStatus>>& values) override;
    virtual IINField ProcessHeader(const RangeHeader& header, const ICollection<Indexed<Counter>>& values) override;
    virtual IINField ProcessHeader(const RangeHeader& header,
                                   const ICollection<Indexed<FrozenCounter>>& values) override;
    virtual IINField ProcessHeader(const RangeHeader& header, const ICollection<Indexed<Analog>>& values) override;
    virtual IINField ProcessHeader(const RangeHeader& header,
                                   const ICollection<Indexed<AnalogOutputStatus>>& values) override;
    virtual IINField ProcessHeader(const RangeHeader& header, const ICollection<Indexed<OctetString>>& values) override;
    virtual IINField ProcessHeader(const RangeHeader& header,
                                   const ICollection<Indexed<TimeAndInterval>>& values) override;

    virtual IINField ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<Binary>>& values) override;
    virtual IINField ProcessHeader(const PrefixHeader& header,
                                   const ICollection<Indexed<BinaryOutputStatus>>& values) override;
    virtual IINField ProcessHeader(const PrefixHeader& header,
                                   const ICollection<Indexed<DoubleBitBinary>>& values) override;
    virtual IINField ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<Counter>>& values) override;
    virtual IINField ProcessHeader(const PrefixHeader& header,
                                   const ICollection<Indexed<FrozenCounter>>& values) override;
    virtual IINField ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<Analog>>& values) override;
    virtual IINField ProcessHeader(const PrefixHeader& header,
                                   const ICollection<Indexed<AnalogOutputStatus>>& values) override;
    virtual IINField ProcessHeader(const PrefixHeader& header,
                                   const ICollection<Indexed<OctetString>>& values) override;
    virtual IINField ProcessHeader(const PrefixHeader& header,
                                   const ICollection<Indexed<TimeAndInterval>>& values) override;
    virtual IINField ProcessHeader(const PrefixHeader& header,
                                   const ICollection<Indexed<BinaryCommandEvent>>& values) override;
    virtual IINField ProcessHeader(const PrefixHeader& header,
                                   const ICollection<Indexed<AnalogCommandEvent>>& values) override;

    virtual IINField ProcessHeader(const PrefixHeader& header,
                                   const ICollection<Indexed<ControlRelayOutputBlock>>& values) override;
    virtual IINField ProcessHeader(const PrefixHeader& header,
                                   const ICollection<Indexed<AnalogOutputInt16>>& values) override;
    virtual IINField ProcessHeader(const PrefixHeader& header,
                                   const ICollection<Indexed<AnalogOutputInt32>>& values) override;
    virtual IINField ProcessHeader(const PrefixHeader& header,
                                   const ICollection<Indexed<AnalogOutputFloat32>>& values) override;
    virtual IINField ProcessHeader(const PrefixHeader& header,
                                   const ICollection<Indexed<AnalogOutputDouble64>>& values) override;
};

template<class T> IINField LoggingHandler::PrintV(const ICollection<Indexed<T>>& items)
{
    Indent i(*callbacks);
    auto logItem = [this](const Indexed<T>& item) {
        FORMAT_LOG_BLOCK(logger, flags::APP_OBJECT_RX, "[%u] - value: %s", item.index,
                         GetStringValue(item.value.value));
    };

    items.ForeachItem(logItem);

    return IINField::Empty();
}

template<class T> IINField LoggingHandler::PrintTime(const ICollection<T>& items)
{
    Indent i(*callbacks);
    uint32_t count = 0;
    auto logItem = [this, &count](const T& item) {
        auto time = ToUTCString(item.time);
        FORMAT_LOG_BLOCK(logger, flags::APP_OBJECT_RX, "[%u] - time: %s", count, time.c_str());
        ++count;
    };

    items.ForeachItem(logItem);

    return IINField::Empty();
}

template<class T> IINField LoggingHandler::PrintTime16(const ICollection<T>& items)
{
    Indent i(*callbacks);
    uint32_t count = 0;
    auto logItem = [this, &count](const T& item) {
        std::ostringstream oss;
        oss << "[" << count << "] - time: " << item.time;
        SIMPLE_LOG_BLOCK(logger, flags::APP_OBJECT_RX, oss.str().c_str());
        ++count;
    };

    items.ForeachItem(logItem);

    return IINField::Empty();
}

template<class T> IINField LoggingHandler::PrintVQT(GroupVariation gv, const ICollection<Indexed<T>>& items)
{
    Indent i(*callbacks);
    auto logItem = [this, gv](const Indexed<T>& item) {
        std::ostringstream oss;
        oss << "[" << item.index << "] - value: " << item.value.value;
        if (HasFlags(gv))
        {
            oss << " flags: 0x" << std::hex << ToHex(item.value.flags.value) << std::dec;
        }
        if (HasAbsoluteTime(gv) || HasRelativeTime(gv))
        {
            oss << " time: " << ToUTCString(item.value.time);
        }
        SIMPLE_LOG_BLOCK(logger, flags::APP_OBJECT_RX, oss.str().c_str());
    };

    items.ForeachItem(logItem);

    return IINField::Empty();
}

template<class T> IINField LoggingHandler::PrintAO(const ICollection<Indexed<T>>& items)
{
    Indent i(*callbacks);
    auto logItem = [this](const Indexed<T>& item) {
        std::ostringstream oss;
        oss << "[" << item.index << "] - value: " << item.value.value;
        oss << " status: " << CommandStatusSpec::to_human_string(item.value.status);
        SIMPLE_LOG_BLOCK(logger, flags::APP_OBJECT_RX, oss.str().c_str());
    };

    items.ForeachItem(logItem);

    return IINField::Empty();
}

template<class T, class Stringify>
IINField LoggingHandler::PrintVQTStringify(GroupVariation gv,
                                           const ICollection<Indexed<T>>& items,
                                           const Stringify& stringify)
{
    Indent i(*callbacks);
    auto logItem = [this, gv, stringify](const Indexed<T>& item) {
        std::ostringstream oss;
        oss << "[" << item.index << "] - value: " << stringify(item.value.value);
        if (HasFlags(gv))
        {
            oss << " flags: 0x" << std::hex << ToHex(item.value.flags.value) << std::dec;
        }
        if (HasAbsoluteTime(gv) || HasRelativeTime(gv))
        {
            oss << " time: " << item.value.time.value;
        }
        SIMPLE_LOG_BLOCK(logger, flags::APP_OBJECT_RX, oss.str().c_str());
    };

    items.ForeachItem(logItem);

    return IINField::Empty();
}
} // namespace opendnp3

#endif
