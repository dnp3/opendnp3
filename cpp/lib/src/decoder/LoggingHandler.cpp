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

#include "decoder/LoggingHandler.h"

#include <ctime>
#include <iomanip>

namespace opendnp3
{

LoggingHandler::LoggingHandler(const Logger& logger_, IDecoderCallbacks& callbacks_)
    : logger(logger_), callbacks(&callbacks_)
{
}

void LoggingHandler::OnHeaderResult(const HeaderRecord& /*record*/, const IINField& result)
{
    if (result.Any())
    {
        Indent i(*callbacks);
        SIMPLE_LOG_BLOCK(logger, flags::APP_OBJECT_RX, "Pretty printing not supported for this type");
    }
}

std::string LoggingHandler::ToUTCString(const DNPTime& dnptime)
{
    auto seconds = static_cast<time_t>(dnptime.value / 1000);
    auto milliseconds = static_cast<uint16_t>(dnptime.value % 1000);

#ifdef WIN32
    tm t;
    if (gmtime_s(&t, &seconds) != 0)
    {
        return "BAD TIME";
    }
#else
    tm t;
    if (!gmtime_r(&seconds, &t))
    {
        return "BAD TIME";
    }
#endif

    std::ostringstream oss;
    oss << (1900 + t.tm_year);
    oss << "-" << std::setfill('0') << std::setw(2) << (1 + t.tm_mon);
    oss << "-" << std::setfill('0') << std::setw(2) << t.tm_mday;
    oss << " " << std::setfill('0') << std::setw(2) << t.tm_hour;
    oss << ":" << std::setfill('0') << std::setw(2) << t.tm_min;
    oss << ":" << std::setfill('0') << std::setw(2) << t.tm_sec;
    oss << "." << std::setfill('0') << std::setw(3) << milliseconds;
    return oss.str();
}

IINField LoggingHandler::PrintCrob(const ICollection<Indexed<ControlRelayOutputBlock>>& items)
{
    Indent i(*callbacks);
    auto logItem = [this](const Indexed<ControlRelayOutputBlock>& item) {
        std::ostringstream oss;
        oss << "[" << item.index << "] - code: 0x" << ToHex(item.value.rawCode) << " ("
            << "op type: " << OperationTypeSpec::to_human_string(item.value.opType)
            << ", tcc: " << TripCloseCodeSpec::to_human_string(item.value.tcc) << ", cr: " << item.value.clear << ")";
        oss << " count: " << static_cast<size_t>(item.value.count);
        oss << " on-time: " << static_cast<size_t>(item.value.onTimeMS);
        oss << " off-time: " << static_cast<size_t>(item.value.offTimeMS);
        oss << " status: " << CommandStatusSpec::to_human_string(item.value.status);
        SIMPLE_LOG_BLOCK(logger, flags::APP_OBJECT_RX, oss.str().c_str());
    };

    items.ForeachItem(logItem);

    return IINField::Empty();
}

IINField LoggingHandler::PrintOctets(const ICollection<Indexed<OctetString>>& items)
{
    Indent i(*callbacks);
    auto logItem = [this](const Indexed<OctetString>& item) {
        const auto buffer = item.value.ToBuffer();
        const auto slice = ser4cpp::rseq_t(buffer.data, buffer.length);
        FORMAT_LOG_BLOCK(logger, flags::APP_OBJECT_RX, "[%u] value: (length = %zu)", item.index, slice.length());
        FORMAT_HEX_BLOCK(logger, flags::APP_OBJECT_RX, slice, 18, 18);
    };

    items.ForeachItem(logItem);

    return IINField::Empty();
}

IINField LoggingHandler::PrintTimeAndInterval(const ICollection<Indexed<TimeAndInterval>>& values)
{
    Indent i(*callbacks);
    auto logItem = [this](const Indexed<TimeAndInterval>& item) {
        std::ostringstream oss;
        oss << "[" << item.index << "] - startTime: " << ToUTCString(item.value.time);
        oss << " count: " << item.value.interval;
        oss << " units: " << IntervalUnitsSpec::to_human_string(item.value.GetUnitsEnum()) << " ("
            << static_cast<int>(item.value.units) << ")";
        SIMPLE_LOG_BLOCK(logger, flags::APP_OBJECT_RX, oss.str().c_str());
    };

    values.ForeachItem(logItem);

    return IINField::Empty();
}

IINField LoggingHandler::ProcessHeader(const CountHeader& /*header*/, const ICollection<Group50Var1>& values)
{
    return this->PrintTime(values);
}

IINField LoggingHandler::ProcessHeader(const CountHeader& /*header*/, const ICollection<Group51Var1>& values)
{
    return this->PrintTime(values);
}

IINField LoggingHandler::ProcessHeader(const CountHeader& /*header*/, const ICollection<Group51Var2>& values)
{
    return this->PrintTime(values);
}

IINField LoggingHandler::ProcessHeader(const CountHeader& /*header*/, const ICollection<Group52Var1>& values)
{
    return this->PrintTime16(values);
}

IINField LoggingHandler::ProcessHeader(const CountHeader& /*header*/, const ICollection<Group52Var2>& values)
{
    return this->PrintTime16(values);
}

IINField LoggingHandler::ProcessHeader(const RangeHeader& /*header*/, const ICollection<Indexed<IINValue>>& values)
{
    return this->PrintV(values);
}

IINField LoggingHandler::ProcessHeader(const RangeHeader& header, const ICollection<Indexed<Binary>>& values)
{
    auto stringify = [](bool value) -> const char* { return GetStringValue(value); };
    return this->PrintVQTStringify(header.enumeration, values, stringify);
}

IINField LoggingHandler::ProcessHeader(const RangeHeader& header, const ICollection<Indexed<DoubleBitBinary>>& values)
{
    auto stringify = [](DoubleBit db) -> const char* { return DoubleBitSpec::to_human_string(db); };
    return this->PrintVQTStringify(header.enumeration, values, stringify);
}

IINField LoggingHandler::ProcessHeader(const RangeHeader& header,
                                       const ICollection<Indexed<BinaryOutputStatus>>& values)
{
    return this->PrintVQT(header.enumeration, values);
}

IINField LoggingHandler::ProcessHeader(const RangeHeader& header, const ICollection<Indexed<Counter>>& values)
{
    return this->PrintVQT(header.enumeration, values);
}

IINField LoggingHandler::ProcessHeader(const RangeHeader& header, const ICollection<Indexed<FrozenCounter>>& values)
{
    return this->PrintVQT(header.enumeration, values);
}

IINField LoggingHandler::ProcessHeader(const RangeHeader& header, const ICollection<Indexed<Analog>>& values)
{
    return this->PrintVQT(header.enumeration, values);
}

IINField LoggingHandler::ProcessHeader(const RangeHeader& header,
                                       const ICollection<Indexed<AnalogOutputStatus>>& values)
{
    return this->PrintVQT(header.enumeration, values);
}

IINField LoggingHandler::ProcessHeader(const RangeHeader& /*header*/, const ICollection<Indexed<OctetString>>& values)
{
    return this->PrintOctets(values);
}

IINField LoggingHandler::ProcessHeader(const RangeHeader& /*header*/,
                                       const ICollection<Indexed<TimeAndInterval>>& values)
{
    return this->PrintTimeAndInterval(values);
}

IINField LoggingHandler::ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<Binary>>& values)
{
    return this->PrintVQT(header.enumeration, values);
}

IINField LoggingHandler::ProcessHeader(const PrefixHeader& header,
                                       const ICollection<Indexed<BinaryOutputStatus>>& values)
{
    return this->PrintVQT(header.enumeration, values);
}

IINField LoggingHandler::ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<DoubleBitBinary>>& values)
{
    auto stringify = [](DoubleBit db) -> const char* { return DoubleBitSpec::to_human_string(db); };
    return this->PrintVQTStringify(header.enumeration, values, stringify);
}

IINField LoggingHandler::ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<Counter>>& values)
{
    return this->PrintVQT(header.enumeration, values);
}

IINField LoggingHandler::ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<FrozenCounter>>& values)
{
    return this->PrintVQT(header.enumeration, values);
}

IINField LoggingHandler::ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<Analog>>& values)
{
    return this->PrintVQT(header.enumeration, values);
}

IINField LoggingHandler::ProcessHeader(const PrefixHeader& header,
                                       const ICollection<Indexed<AnalogOutputStatus>>& values)
{
    return this->PrintVQT(header.enumeration, values);
}

IINField LoggingHandler::ProcessHeader(const PrefixHeader& /*header*/, const ICollection<Indexed<OctetString>>& values)
{
    return this->PrintOctets(values);
}

IINField LoggingHandler::ProcessHeader(const PrefixHeader& /*header*/,
                                       const ICollection<Indexed<TimeAndInterval>>& values)
{
    return this->PrintTimeAndInterval(values);
}

IINField LoggingHandler::ProcessHeader(const PrefixHeader& header,
                                       const ICollection<Indexed<BinaryCommandEvent>>& values)
{
    Indent i(*callbacks);
    const bool HAS_TIME = HasAbsoluteTime(header.enumeration);
    auto logItem = [this, HAS_TIME](const Indexed<BinaryCommandEvent>& item) {
        std::ostringstream oss;
        oss << "[" << item.index << "] - value: " << GetStringValue(item.value.value)
            << "  status: " << CommandStatusSpec::to_human_string(item.value.status);
        if (HAS_TIME)
        {
            oss << " time: " << ToUTCString(item.value.time);
        }
        SIMPLE_LOG_BLOCK(logger, flags::APP_OBJECT_RX, oss.str().c_str());
    };

    values.ForeachItem(logItem);

    return IINField::Empty();
}

IINField LoggingHandler::ProcessHeader(const PrefixHeader& header,
                                       const ICollection<Indexed<AnalogCommandEvent>>& values)
{
    Indent i(*callbacks);
    const bool HAS_TIME = HasAbsoluteTime(header.enumeration);
    auto logItem = [this, HAS_TIME](const Indexed<AnalogCommandEvent>& item) {
        std::ostringstream oss;
        oss << "[" << item.index << "] - value: " << item.value.value
            << "  status: " << CommandStatusSpec::to_human_string(item.value.status);
        if (HAS_TIME)
        {
            oss << " time: " << ToUTCString(item.value.time);
        }
        SIMPLE_LOG_BLOCK(logger, flags::APP_OBJECT_RX, oss.str().c_str());
    };

    values.ForeachItem(logItem);

    return IINField::Empty();
}

IINField LoggingHandler::ProcessHeader(const PrefixHeader& /*header*/,
                                       const ICollection<Indexed<ControlRelayOutputBlock>>& values)
{
    return PrintCrob(values);
}

IINField LoggingHandler::ProcessHeader(const PrefixHeader& /*header*/,
                                       const ICollection<Indexed<AnalogOutputInt16>>& values)
{
    return PrintAO(values);
}

IINField LoggingHandler::ProcessHeader(const PrefixHeader& /*header*/,
                                       const ICollection<Indexed<AnalogOutputInt32>>& values)
{
    return PrintAO(values);
}

IINField LoggingHandler::ProcessHeader(const PrefixHeader& /*header*/,
                                       const ICollection<Indexed<AnalogOutputFloat32>>& values)
{
    return PrintAO(values);
}

IINField LoggingHandler::ProcessHeader(const PrefixHeader& /*header*/,
                                       const ICollection<Indexed<AnalogOutputDouble64>>& values)
{
    return PrintAO(values);
}

} // namespace opendnp3
