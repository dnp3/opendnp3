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
#include "asiodnp3/DNP3Manager.h"

#include "asiodnp3/DNP3ManagerImpl.h"

#include <utility>

namespace asiodnp3
{

DNP3Manager::DNP3Manager(uint32_t concurrencyHint,
                         std::shared_ptr<openpal::ILogHandler> handler,
                         std::function<void()> onThreadStart,
                         std::function<void()> onThreadExit)
    : impl(std::make_unique<DNP3ManagerImpl>(concurrencyHint, handler, onThreadStart, onThreadExit))
{
}

DNP3Manager::~DNP3Manager() = default;

void DNP3Manager::Shutdown()
{
    impl->Shutdown();
}

std::shared_ptr<IChannel> DNP3Manager::AddTCPClient(const std::string& id,
                                                    int32_t levels,
                                                    const asiopal::ChannelRetry& retry,
                                                    const std::string& host,
                                                    const std::string& local,
                                                    uint16_t port,
                                                    std::shared_ptr<IChannelListener> listener)
{
    return this->impl->AddTCPClient(id, levels, retry, {asiopal::IPEndpoint(host, port)}, local, std::move(listener));
}

std::shared_ptr<IChannel> DNP3Manager::AddTCPClient(const std::string& id,
                                                    int32_t levels,
                                                    const asiopal::ChannelRetry& retry,
                                                    const std::vector<asiopal::IPEndpoint>& hosts,
                                                    const std::string& local,
                                                    std::shared_ptr<IChannelListener> listener)
{
    return this->impl->AddTCPClient(id, levels, retry, hosts, local, std::move(listener));
}

std::shared_ptr<IChannel> DNP3Manager::AddTCPServer(const std::string& id,
                                                    int32_t levels,
                                                    opendnp3::ServerAcceptMode mode,
                                                    const std::string& endpoint,
                                                    uint16_t port,
                                                    std::shared_ptr<IChannelListener> listener)
{
    return this->impl->AddTCPServer(id, levels, mode, endpoint, port, std::move(listener));
}

std::shared_ptr<IChannel> DNP3Manager::AddSerial(const std::string& id,
                                                 int32_t levels,
                                                 const asiopal::ChannelRetry& retry,
                                                 asiopal::SerialSettings settings,
                                                 std::shared_ptr<IChannelListener> listener)
{
    return this->impl->AddSerial(id, levels, retry, std::move(settings), std::move(listener));
}

std::shared_ptr<IChannel> DNP3Manager::AddTLSClient(const std::string& id,
                                                    int32_t levels,
                                                    const asiopal::ChannelRetry& retry,
                                                    const std::string& host,
                                                    const std::string& local,
                                                    uint16_t port,
                                                    const asiopal::TLSConfig& config,
                                                    std::shared_ptr<IChannelListener> listener,
                                                    std::error_code& ec)
{
    return this->impl->AddTLSClient(id, levels, retry, {asiopal::IPEndpoint(host, port)}, local, config,
                                    std::move(listener), ec);
}

std::shared_ptr<IChannel> DNP3Manager::AddTLSClient(const std::string& id,
                                                    int32_t levels,
                                                    const asiopal::ChannelRetry& retry,
                                                    const std::vector<asiopal::IPEndpoint>& hosts,
                                                    const std::string& local,
                                                    const asiopal::TLSConfig& config,
                                                    std::shared_ptr<IChannelListener> listener,
                                                    std::error_code& ec)
{
    return this->impl->AddTLSClient(id, levels, retry, hosts, local, config, std::move(listener), ec);
}

std::shared_ptr<IChannel> DNP3Manager::AddTLSServer(const std::string& id,
                                                    int32_t levels,
                                                    opendnp3::ServerAcceptMode mode,
                                                    const std::string& endpoint,
                                                    uint16_t port,
                                                    const asiopal::TLSConfig& config,
                                                    std::shared_ptr<IChannelListener> listener,
                                                    std::error_code& ec)
{
    return this->impl->AddTLSServer(id, levels, mode, endpoint, port, config, std::move(listener), ec);
}

std::shared_ptr<asiopal::IListener> DNP3Manager::CreateListener(std::string loggerid,
                                                                openpal::LogFilters loglevel,
                                                                asiopal::IPEndpoint endpoint,
                                                                const std::shared_ptr<IListenCallbacks>& callbacks,
                                                                std::error_code& ec)
{
    return impl->CreateListener(std::move(loggerid), loglevel, std::move(endpoint), callbacks, ec);
}

std::shared_ptr<asiopal::IListener> DNP3Manager::CreateListener(std::string loggerid,
                                                                openpal::LogFilters loglevel,
                                                                asiopal::IPEndpoint endpoint,
                                                                const asiopal::TLSConfig& config,
                                                                const std::shared_ptr<IListenCallbacks>& callbacks,
                                                                std::error_code& ec)
{
    return impl->CreateListener(std::move(loggerid), loglevel, std::move(endpoint), config, callbacks, ec);
}

} // namespace asiodnp3
