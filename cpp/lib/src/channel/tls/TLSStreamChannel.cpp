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

#include "channel/tls/TLSStreamChannel.h"

#include <utility>

namespace opendnp3
{

TLSStreamChannel::TLSStreamChannel(const std::shared_ptr<exe4cpp::StrandExecutor>& executor,
                                   std::shared_ptr<asio::ssl::stream<asio::ip::tcp::socket>> stream)
    : IAsyncChannel(executor), stream(std::move(stream))
{
}

void TLSStreamChannel::BeginReadImpl(ser4cpp::wseq_t dest)
{
    auto callback = [this](const std::error_code& ec, size_t num) { this->OnReadCallback(ec, num); };

    stream->async_read_some(asio::buffer(dest, dest.length()), this->executor->wrap(callback));
}

void TLSStreamChannel::BeginWriteImpl(const ser4cpp::rseq_t& data)
{
    auto callback = [this](const std::error_code& ec, size_t num) { this->OnWriteCallback(ec, num); };

    asio::async_write(*stream, asio::buffer(data, data.length()), this->executor->wrap(callback));
}

void TLSStreamChannel::ShutdownImpl()
{
    // Don't bother with a TLS shutdown. Just shutdown the socket.
    std::error_code ec;
    stream->lowest_layer().shutdown(asio::socket_base::shutdown_both, ec);
    stream->lowest_layer().close(ec);
}

} // namespace opendnp3
