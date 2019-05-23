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

#include "TCPClientIOHandler.h"

#include "asiopal/SocketChannel.h"

#include <utility>

using namespace asiopal;

namespace asiodnp3
{

TCPClientIOHandler::TCPClientIOHandler(const openpal::Logger& logger,
                                       const std::shared_ptr<IChannelListener>& listener,
                                       const std::shared_ptr<asiopal::Executor>& executor,
                                       const asiopal::ChannelRetry& retry,
                                       const asiodnp3::IPEndpointsList& remotes,
                                       std::string adapter)
    : IOHandler(logger, false, listener),
      executor(executor),
      retry(retry),
      remotes(remotes),
      adapter(std::move(adapter)),
      retrytimer(*executor)
{
}

void TCPClientIOHandler::ShutdownImpl()
{
    this->ResetState();
}

void TCPClientIOHandler::BeginChannelAccept()
{
    this->client = TCPClient::Create(logger, executor, adapter);
    this->StartConnect(this->retry.minOpenRetry);
}

void TCPClientIOHandler::SuspendChannelAccept()
{
    this->ResetState();
}

void TCPClientIOHandler::OnChannelShutdown()
{
    this->BeginChannelAccept();
}

bool TCPClientIOHandler::StartConnect(const openpal::TimeDuration& delay)
{
    if (!client)
    {
        return false;
    }

    auto cb = [=, self = shared_from_this()](const std::shared_ptr<Executor>& executor, asio::ip::tcp::socket socket,
                                             const std::error_code& ec) -> void {
        if (ec)
        {
            FORMAT_LOG_BLOCK(this->logger, openpal::logflags::WARN, "Error Connecting: %s", ec.message().c_str());

            ++this->statistics.numOpenFail;

            const auto newDelay = this->retry.NextDelay(delay);

            if (client)
            {
                auto retry_cb = [self, newDelay, this]() {
                    this->remotes.Next();
                    this->StartConnect(newDelay);
                };

                this->retrytimer.Start(delay, retry_cb);
            }
        }
        else
        {
            FORMAT_LOG_BLOCK(this->logger, openpal::logflags::INFO, "Connected to: %s, port %u",
                             this->remotes.GetCurrentEndpoint().address.c_str(),
                             this->remotes.GetCurrentEndpoint().port);

            if (client)
            {
                this->OnNewChannel(SocketChannel::Create(executor, std::move(socket)));
            }
        }
    };

    FORMAT_LOG_BLOCK(this->logger, openpal::logflags::INFO, "Connecting to: %s, port %u",
                     this->remotes.GetCurrentEndpoint().address.c_str(), this->remotes.GetCurrentEndpoint().port);

    this->client->BeginConnect(this->remotes.GetCurrentEndpoint(), cb);

    return true;
}

void TCPClientIOHandler::ResetState()
{
    if (this->client)
    {
        this->client->Cancel();
        this->client.reset();
    }

    this->remotes.Reset();

    retrytimer.Cancel();
}

} // namespace asiodnp3
