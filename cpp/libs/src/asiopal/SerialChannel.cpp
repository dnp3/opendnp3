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

#include "asiopal/SerialChannel.h"

#include "asiopal/ASIOSerialHelpers.h"

#ifdef USE_FLOCK
#include <sys/file.h>

#include <cerrno>
#endif

namespace asiopal
{

SerialChannel::SerialChannel(const std::shared_ptr<Executor>& executor)
    : IAsyncChannel(executor), port(executor->strand.get_io_context())
{
}

bool SerialChannel::Open(const SerialSettings& settings, std::error_code& ec)
{
    port.open(settings.deviceName, ec);
    if (ec)
        return false;

#ifdef USE_FLOCK
    auto r = flock(port.native_handle(), LOCK_EX | LOCK_NB);
    if (r < 0)
    {
        ec = std::make_error_code(std::errc::device_or_resource_busy);
        return false;
    }
#endif

    Configure(settings, port, ec);

    if (ec)
    {
        port.close();
        return false;
    }

    return true;
}

void SerialChannel::BeginReadImpl(openpal::WSlice buffer)
{
    auto callback = [this](const std::error_code& ec, size_t num) { this->OnReadCallback(ec, num); };

    port.async_read_some(asio::buffer(buffer, buffer.Size()), this->executor->strand.wrap(callback));
}

void SerialChannel::BeginWriteImpl(const openpal::RSlice& buffer)
{
    auto callback = [this](const std::error_code& ec, size_t num) { this->OnWriteCallback(ec, num); };

    async_write(port, asio::buffer(buffer, buffer.Size()), this->executor->strand.wrap(callback));
}

void SerialChannel::ShutdownImpl()
{
    std::error_code ec;
#ifdef USE_FLOCK
    /* Explicitly unlock serial device handler before exiting.*/
    flock(port.native_handle(), LOCK_UN);
#endif
    port.close(ec);
}

} // namespace asiopal
