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
#include "channel/ASIOSerialHelpers.h"

namespace opendnp3
{

asio::serial_port_base::stop_bits ConvertStopBits(StopBits stopBits)
{
    asio::serial_port_base::stop_bits::type t = asio::serial_port_base::stop_bits::one;

    switch (stopBits)
    {
    case (StopBits::One):
        t = asio::serial_port_base::stop_bits::one;
        break;
    case (StopBits::OnePointFive):
        t = asio::serial_port_base::stop_bits::onepointfive;
        break;
    case (StopBits::Two):
        t = asio::serial_port_base::stop_bits::two;
        break;
    default:
        break;
    }

    return asio::serial_port_base::stop_bits(t);
}

asio::serial_port_base::flow_control ConvertFlow(FlowControl flowType)
{
    asio::serial_port_base::flow_control::type t = asio::serial_port_base::flow_control::none;

    switch (flowType)
    {
    case (FlowControl::None):
        t = asio::serial_port_base::flow_control::none;
        break;
    case (FlowControl::XONXOFF):
        t = asio::serial_port_base::flow_control::software;
        break;
    case (FlowControl::Hardware):
        t = asio::serial_port_base::flow_control::hardware;
        break;
    default:
        break;
    }

    return asio::serial_port_base::flow_control(t);
}

asio::serial_port_base::character_size ConvertDataBits(int aDataBits)
{
    return asio::serial_port_base::character_size(static_cast<unsigned int>(aDataBits));
}

asio::serial_port_base::baud_rate ConvertBaud(int aBaud)
{
    return asio::serial_port_base::baud_rate(static_cast<unsigned int>(aBaud));
}

asio::serial_port_base::parity ConvertParity(Parity parity)
{
    asio::serial_port_base::parity::type t = asio::serial_port_base::parity::none;

    switch (parity)
    {
    case (Parity::Even):
        t = asio::serial_port_base::parity::even;
        break;
    case (Parity::Odd):
        t = asio::serial_port_base::parity::odd;
        break;
    default:
        break;
    }

    return asio::serial_port_base::parity(t);
}

bool Configure(const SerialSettings& settings, asio::serial_port& port, std::error_code& ec)
{
    // Set all the various options
    port.set_option(ConvertBaud(settings.baud), ec);
    if (ec)
        return false;

    port.set_option(ConvertDataBits(settings.dataBits), ec);
    if (ec)
        return false;

    port.set_option(ConvertParity(settings.parity), ec);
    if (ec)
        return false;

    port.set_option(ConvertFlow(settings.flowType), ec);
    if (ec)
        return false;

    port.set_option(ConvertStopBits(settings.stopBits), ec);
    return !ec;
}

} // namespace opendnp3
