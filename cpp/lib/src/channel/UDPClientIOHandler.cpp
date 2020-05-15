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

#include "channel/UDPClientIOHandler.h"

#include "channel/UDPSocketChannel.h"

#include <utility>

namespace opendnp3
{

UDPClientIOHandler::UDPClientIOHandler(const Logger& logger,
                                       const std::shared_ptr<IChannelListener>& listener,
                                       const std::shared_ptr<exe4cpp::StrandExecutor>& executor,
                                       const ChannelRetry& retry,
                                       const IPEndpoint& localEndpoint,
                                       const IPEndpoint& remoteEndpoint)
    : IOHandler(logger, false, listener),
      executor(executor),
      retry(retry),
      localEndpoint(localEndpoint),
      remoteEndpoint(remoteEndpoint)
{
}

void UDPClientIOHandler::ShutdownImpl()
{
    this->ResetState();
}

void UDPClientIOHandler::BeginChannelAccept()
{
    client = std::make_shared<UDPClient>(logger, executor);
    this->TryOpen(this->retry.minOpenRetry);
}

void UDPClientIOHandler::SuspendChannelAccept()
{
    this->ResetState();
}

void UDPClientIOHandler::OnChannelShutdown()
{
    this->BeginChannelAccept();
}

bool UDPClientIOHandler::TryOpen(const TimeDuration& delay)
{
    if (!client)
    {
        return false;
    }

    auto cb = [=, self = shared_from_this()](const std::shared_ptr<exe4cpp::StrandExecutor>& executor,
                                             asio::ip::udp::socket socket, const std::error_code& ec) -> void {
        if (ec)
        {
            FORMAT_LOG_BLOCK(this->logger, flags::WARN, "Error opening UDP socket: %s", ec.message().c_str());

            ++this->statistics.numOpenFail;

            const auto newDelay = this->retry.NextDelay(delay);

            if (client)
            {
                auto retry_cb = [self, newDelay, this]() { this->TryOpen(newDelay); };

                this->retrytimer = this->executor->start(delay.value, retry_cb);
            }
        }
        else
        {
            FORMAT_LOG_BLOCK(this->logger, flags::INFO, "UDP socket binded to: %s, port %u, sending to %s, port %u",
                             localEndpoint.address.c_str(), localEndpoint.port, remoteEndpoint.address.c_str(),
                             remoteEndpoint.port);

            if (client)
            {
                this->OnNewChannel(UDPSocketChannel::Create(executor, std::move(socket)));
            }
        }
    };

    FORMAT_LOG_BLOCK(this->logger, flags::INFO, "Binding UDP socket to: %s, port %u, resolving address: %s, port %u",
                     localEndpoint.address.c_str(), localEndpoint.port, remoteEndpoint.address.c_str(),
                     remoteEndpoint.port);

    this->client->Open(localEndpoint, remoteEndpoint, cb);

    return true;
}

void UDPClientIOHandler::ResetState()
{
    if (this->client)
    {
        this->client->Cancel();
        this->client.reset();
    }

    retrytimer.cancel();
}

} // namespace opendnp3
