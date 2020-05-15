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

#ifndef OPENDNP3_DNP3MANAGERIMPL_H
#define OPENDNP3_DNP3MANAGERIMPL_H

#include "ResourceManager.h"

#include "opendnp3/channel/ChannelRetry.h"
#include "opendnp3/channel/IChannel.h"
#include "opendnp3/channel/IChannelListener.h"
#include "opendnp3/channel/IListener.h"
#include "opendnp3/channel/IPEndpoint.h"
#include "opendnp3/channel/SerialSettings.h"
#include "opendnp3/channel/TLSConfig.h"
#include "opendnp3/gen/ServerAcceptMode.h"
#include "opendnp3/logging/Logger.h"
#include "opendnp3/master/IListenCallbacks.h"
#include "opendnp3/util/Uncopyable.h"

#include <exe4cpp/asio/ThreadPool.h>

namespace opendnp3
{

class DNP3ManagerImpl : private Uncopyable
{

public:
    DNP3ManagerImpl(uint32_t concurrencyHint,
                    std::shared_ptr<opendnp3::ILogHandler> handler,
                    std::function<void(uint32_t)> onThreadStart,
                    std::function<void(uint32_t)> onThreadExit);

    ~DNP3ManagerImpl();

    void Shutdown();

    std::shared_ptr<IChannel> AddTCPClient(const std::string& id,
                                           const opendnp3::LogLevels& levels,
                                           const ChannelRetry& retry,
                                           const std::vector<IPEndpoint>& hosts,
                                           const std::string& local,
                                           std::shared_ptr<IChannelListener> listener);

    std::shared_ptr<IChannel> AddTCPServer(const std::string& id,
                                           const opendnp3::LogLevels& levels,
                                           ServerAcceptMode mode,
                                           const IPEndpoint& endpoint,
                                           std::shared_ptr<IChannelListener> listener);

    std::shared_ptr<IChannel> AddUDPChannel(const std::string& id,
                                            const opendnp3::LogLevels& levels,
                                            const ChannelRetry& retry,
                                            const IPEndpoint& localEndpoint,
                                            const IPEndpoint& remoteEndpoint,
                                            std::shared_ptr<IChannelListener> listener);

    std::shared_ptr<IChannel> AddSerial(const std::string& id,
                                        const opendnp3::LogLevels& levels,
                                        const ChannelRetry& retry,
                                        SerialSettings settings,
                                        std::shared_ptr<IChannelListener> listener);

    std::shared_ptr<IChannel> AddTLSClient(const std::string& id,
                                           const opendnp3::LogLevels& levels,
                                           const ChannelRetry& retry,
                                           const std::vector<IPEndpoint>& hosts,
                                           const std::string& local,
                                           const TLSConfig& config,
                                           std::shared_ptr<IChannelListener> listener);

    std::shared_ptr<IChannel> AddTLSServer(const std::string& id,
                                           const opendnp3::LogLevels& levels,
                                           ServerAcceptMode mode,
                                           const IPEndpoint& endpoint,
                                           const TLSConfig& config,
                                           std::shared_ptr<IChannelListener> listener);

    std::shared_ptr<IListener> CreateListener(std::string loggerid,
                                              const opendnp3::LogLevels& levels,
                                              const IPEndpoint& endpoint,
                                              const std::shared_ptr<IListenCallbacks>& callbacks);

    std::shared_ptr<IListener> CreateListener(std::string loggerid,
                                              const opendnp3::LogLevels& levels,
                                              const IPEndpoint& endpoint,
                                              const TLSConfig& config,
                                              const std::shared_ptr<IListenCallbacks>& callbacks);

private:
    Logger logger;
    const std::shared_ptr<asio::io_context> io;
    exe4cpp::ThreadPool threadpool;
    std::shared_ptr<ResourceManager> resources;
};

} // namespace opendnp3

#endif
