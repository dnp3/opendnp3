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
#include "opendnp3/DNP3Manager.h"

#include "DNP3ManagerImpl.h"

#include <utility>

namespace opendnp3
{

DNP3Manager::DNP3Manager(uint32_t concurrencyHint,
                         std::shared_ptr<ILogHandler> handler,
                         std::function<void(uint32_t)> onThreadStart,
                         std::function<void(uint32_t)> onThreadExit)
    : impl(std::make_unique<DNP3ManagerImpl>(concurrencyHint, handler, onThreadStart, onThreadExit))
{
}

DNP3Manager::~DNP3Manager() = default;

void DNP3Manager::Shutdown()
{
    impl->Shutdown();
}

std::shared_ptr<IChannel> DNP3Manager::AddTCPClient(const std::string& id,
                                                    const LogLevels& levels,
                                                    const ChannelRetry& retry,
                                                    const std::vector<IPEndpoint>& hosts,
                                                    const std::string& local,
                                                    std::shared_ptr<IChannelListener> listener)
{
    return this->impl->AddTCPClient(id, levels, retry, hosts, local, std::move(listener));
}

std::shared_ptr<IChannel> DNP3Manager::AddTCPServer(const std::string& id,
                                                    const LogLevels& levels,
                                                    ServerAcceptMode mode,
                                                    const IPEndpoint& endpoint,
                                                    std::shared_ptr<IChannelListener> listener)
{
    return this->impl->AddTCPServer(id, levels, mode, endpoint, std::move(listener));
}

std::shared_ptr<IChannel> DNP3Manager::AddUDPChannel(const std::string& id,
                                                     const LogLevels& levels,
                                                     const ChannelRetry& retry,
                                                     const IPEndpoint& localEndpoint,
                                                     const IPEndpoint& remoteEndpoint,
                                                     std::shared_ptr<IChannelListener> listener)
{
    return this->impl->AddUDPChannel(id, levels, retry, localEndpoint, remoteEndpoint, std::move(listener));
}

std::shared_ptr<IChannel> DNP3Manager::AddSerial(const std::string& id,
                                                 const LogLevels& levels,
                                                 const ChannelRetry& retry,
                                                 SerialSettings settings,
                                                 std::shared_ptr<IChannelListener> listener)
{
    return this->impl->AddSerial(id, levels, retry, std::move(settings), std::move(listener));
}

std::shared_ptr<IChannel> DNP3Manager::AddTLSClient(const std::string& id,
                                                    const LogLevels& levels,
                                                    const ChannelRetry& retry,
                                                    const std::vector<IPEndpoint>& hosts,
                                                    const std::string& local,
                                                    const TLSConfig& config,
                                                    std::shared_ptr<IChannelListener> listener)
{
    return this->impl->AddTLSClient(id, levels, retry, hosts, local, config, std::move(listener));
}

std::shared_ptr<IChannel> DNP3Manager::AddTLSServer(const std::string& id,
                                                    const LogLevels& levels,
                                                    ServerAcceptMode mode,
                                                    const IPEndpoint& endpoint,
                                                    const TLSConfig& config,
                                                    std::shared_ptr<IChannelListener> listener)
{
    return this->impl->AddTLSServer(id, levels, mode, endpoint, config, std::move(listener));
}

std::shared_ptr<IListener> DNP3Manager::CreateListener(std::string loggerid,
                                                       const LogLevels& loglevel,
                                                       const IPEndpoint& endpoint,
                                                       const std::shared_ptr<IListenCallbacks>& callbacks)
{
    return impl->CreateListener(std::move(loggerid), loglevel, endpoint, callbacks);
}

std::shared_ptr<IListener> DNP3Manager::CreateListener(std::string loggerid,
                                                       const LogLevels& loglevel,
                                                       const IPEndpoint& endpoint,
                                                       const TLSConfig& config,
                                                       const std::shared_ptr<IListenCallbacks>& callbacks)
{
    return impl->CreateListener(std::move(loggerid), loglevel, endpoint, config, callbacks);
}

} // namespace opendnp3
