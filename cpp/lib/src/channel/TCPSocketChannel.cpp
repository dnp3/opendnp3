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

#include "channel/TCPSocketChannel.h"

namespace opendnp3
{

TCPSocketChannel::TCPSocketChannel(const std::shared_ptr<exe4cpp::StrandExecutor>& executor,
                                   asio::ip::tcp::socket socket)
    : IAsyncChannel(executor), socket(std::move(socket))
{
}

void TCPSocketChannel::BeginReadImpl(ser4cpp::wseq_t dest)
{
    auto callback = [this](const std::error_code& ec, size_t num) { this->OnReadCallback(ec, num); };

    socket.async_read_some(asio::buffer(dest, dest.length()), this->executor->wrap(callback));
}

void TCPSocketChannel::BeginWriteImpl(const ser4cpp::rseq_t& buffer)
{
    auto callback = [this](const std::error_code& ec, size_t num) { this->OnWriteCallback(ec, num); };

    asio::async_write(socket, asio::buffer(buffer, buffer.length()), this->executor->wrap(callback));
}

void TCPSocketChannel::ShutdownImpl()
{
    std::error_code ec;
    socket.shutdown(asio::socket_base::shutdown_type::shutdown_both, ec);
    socket.close(ec);
}

} // namespace opendnp3
