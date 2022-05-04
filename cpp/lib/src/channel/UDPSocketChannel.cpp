/*
 * Copyright 2013-2022 Step Function I/O, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Step Function I/O
 * LLC (https://stepfunc.io) under one or more contributor license agreements.
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Step Function I/O LLC license
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

#include "channel/UDPSocketChannel.h"

#include "logging/LogMacros.h"

static constexpr uint8_t MAX_FIRST_READ_RETRIES = 5;

namespace opendnp3
{

UDPSocketChannel::UDPSocketChannel(const std::shared_ptr<exe4cpp::StrandExecutor>& executor,
                                   const Logger& logger, asio::ip::udp::socket socket)
    : IAsyncChannel(executor), logger(logger), socket(std::move(socket)), first_successful_read(false), num_first_read_retries(0)
{
}

void UDPSocketChannel::BeginReadImpl(ser4cpp::wseq_t dest)
{
    auto callback = [this](const std::error_code& ec, size_t num) {
        if(!this->first_successful_read && this->num_first_read_retries < MAX_FIRST_READ_RETRIES)
        {
            if(ec)
            {
                FORMAT_LOG_BLOCK(this->logger, flags::WARN, "read error: %s", ec.message().c_str());
                FORMAT_LOG_BLOCK(this->logger, flags::DBG, "UDP ignoring initial errors (%d of %d)", this->num_first_read_retries + 1, MAX_FIRST_READ_RETRIES);

                // We ignore failed reads until we get a successful one
                const auto no_ec = std::error_code{};
                this->OnReadCallback(no_ec, num);

                // Avoid infinite loop
                ++this->num_first_read_retries;

                return;
            }
            else
            {
                this->first_successful_read = true;
            }
        }

        this->OnReadCallback(ec, num);
    };

    socket.async_receive(asio::buffer(dest, dest.length()), this->executor->wrap(callback));
}

void UDPSocketChannel::BeginWriteImpl(const ser4cpp::rseq_t& buffer)
{
    auto callback = [this](const std::error_code& ec, size_t num) { this->OnWriteCallback(ec, num); };

    socket.async_send(asio::buffer(buffer, buffer.length()), this->executor->wrap(callback));
}

void UDPSocketChannel::ShutdownImpl()
{
    std::error_code ec;
    socket.shutdown(asio::socket_base::shutdown_type::shutdown_both, ec);
    socket.close(ec);
}

} // namespace opendnp3
