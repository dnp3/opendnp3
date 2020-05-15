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
#ifndef OPENDNP3_DNP3MANAGER_H
#define OPENDNP3_DNP3MANAGER_H

#include "opendnp3/ErrorCodes.h"
#include "opendnp3/channel/ChannelRetry.h"
#include "opendnp3/channel/IChannel.h"
#include "opendnp3/channel/IChannelListener.h"
#include "opendnp3/channel/IListener.h"
#include "opendnp3/channel/IPEndpoint.h"
#include "opendnp3/channel/SerialSettings.h"
#include "opendnp3/channel/TLSConfig.h"
#include "opendnp3/gen/ChannelState.h"
#include "opendnp3/gen/ServerAcceptMode.h"
#include "opendnp3/logging/ILogHandler.h"
#include "opendnp3/logging/LogLevels.h"
#include "opendnp3/master/IListenCallbacks.h"
#include "opendnp3/util/TimeDuration.h"

#include <memory>
#include <system_error>
#include <vector>

namespace opendnp3
{

class DNP3ManagerImpl;

/**
 *	Root DNP3 object used to create channels and sessions.
 */
class DNP3Manager
{

public:
    /**
     *	Construct a manager
     *
     *	@param concurrencyHint How many threads to allocate in the thread pool
     *	@param handler Callback interface for log messages
     *	@param onThreadStart Action to run when a thread pool thread starts
     *	@param onThreadExit Action to run just before a thread pool thread exits
     */
    DNP3Manager(uint32_t concurrencyHint,
                std::shared_ptr<opendnp3::ILogHandler> handler = std::shared_ptr<opendnp3::ILogHandler>(),
                std::function<void(uint32_t)> onThreadStart = [](uint32_t) {},
                std::function<void(uint32_t)> onThreadExit = [](uint32_t) {});

    ~DNP3Manager();

    /**
     * Permanently shutdown the manager and all sub-objects that have been created. Stop
     * the thead pool.
     */
    void Shutdown();

    /**
     * Add a persistent TCP client channel. Automatically attempts to reconnect.
     *
     * @param id Alias that will be used for logging purposes with this channel
     * @param levels Bitfield that describes the logging level for this channel and associated sessions
     * @param retry Retry parameters for failed channels
     * @param hosts List of host addresses to use to connect to the remote outstation (i.e. 127.0.0.1 or www.google.com)
     * @param local adapter address on which to attempt the connection (use 0.0.0.0 for all adapters)
     * @param listener optional callback interface (can be nullptr) for info about the running channel
     * @return shared_ptr to a channel interface
     */
    std::shared_ptr<IChannel> AddTCPClient(const std::string& id,
                                           const opendnp3::LogLevels& levels,
                                           const ChannelRetry& retry,
                                           const std::vector<IPEndpoint>& hosts,
                                           const std::string& local,
                                           std::shared_ptr<IChannelListener> listener);

    /**
     * Add a persistent TCP server channel. Only accepts a single connection at a time.
     *
     * @param id Alias that will be used for logging purposes with this channel
     * @param levels Bitfield that describes the logging level for this channel and associated sessions
     * @param mode Describes how new connections are treated when another session already exists
     * @param endpoint Network adapter to listen on (i.e. 127.0.0.1 or 0.0.0.0) and port
     * @param listener optional callback interface (can be nullptr) for info about the running channel
     * @throw DNP3Error if the manager was already shutdown or if the server could not be binded properly
     * @return shared_ptr to a channel interface
     */
    std::shared_ptr<IChannel> AddTCPServer(const std::string& id,
                                           const opendnp3::LogLevels& levels,
                                           ServerAcceptMode mode,
                                           const IPEndpoint& endpoint,
                                           std::shared_ptr<IChannelListener> listener);

    /**
     * Add a persistent UDP channel.
     *
     * @param id Alias that will be used for logging purposes with this channel
     * @param levels Bitfield that describes the logging level for this channel and associated sessions
     * @param retry Retry parameters for failed channels
     * @param localEndpoint Local endpoint from which datagrams will be received
     * @param remoteEndpoint Remote endpoint where datagrams will be sent to
     * @param listener optional callback interface (can be nullptr) for info about the running channel
     * @throw DNP3Error if the manager was already shutdown
     * @return shared_ptr to a channel interface
     */
    std::shared_ptr<IChannel> AddUDPChannel(const std::string& id,
                                            const opendnp3::LogLevels& levels,
                                            const ChannelRetry& retry,
                                            const IPEndpoint& localEndpoint,
                                            const IPEndpoint& remoteEndpoint,
                                            std::shared_ptr<IChannelListener> listener);

    /**
     * Add a persistent serial channel
     *
     * @param id Alias that will be used for logging purposes with this channel
     * @param levels Bitfield that describes the logging level for this channel and associated sessions
     * @param retry Retry parameters for failed channels
     * @param settings settings object that fully parameterizes the serial port
     * @param listener optional callback interface (can be nullptr) for info about the running channel
     * @throw DNP3Error if the manager was already shutdown
     * @return shared_ptr to a channel interface
     */
    std::shared_ptr<IChannel> AddSerial(const std::string& id,
                                        const opendnp3::LogLevels& levels,
                                        const ChannelRetry& retry,
                                        SerialSettings settings,
                                        std::shared_ptr<IChannelListener> listener);

    /**
     * Add a TLS client channel
     *
     * @throw std::system_error Throws underlying ASIO exception of TLS configuration is invalid
     *
     * @param id Alias that will be used for logging purposes with this channel
     * @param levels Bitfield that describes the logging level for this channel and associated sessions
     * @param retry Retry parameters for failed channels
     * @param hosts IP address of remote outstation (i.e. 127.0.0.1 or www.google.com)
     * @param local adapter address on which to attempt the connection (use 0.0.0.0 for all adapters)
     * @param config TLS configuration information
     * @param listener optional callback interface (can be nullptr) for info about the running channel
     * @throw DNP3Error if the manager was already shutdown or if the library was compiled without TLS support
     * @return shared_ptr to a channel interface
     */
    std::shared_ptr<IChannel> AddTLSClient(const std::string& id,
                                           const opendnp3::LogLevels& levels,
                                           const ChannelRetry& retry,
                                           const std::vector<IPEndpoint>& hosts,
                                           const std::string& local,
                                           const TLSConfig& config,
                                           std::shared_ptr<IChannelListener> listener);

    /**
     * Add a TLS server channel
     *
     * @throw std::system_error Throws underlying ASIO exception of TLS configuration is invalid
     *
     * @param id Alias that will be used for logging purposes with this channel
     * @param levels Bitfield that describes the logging level for this channel and associated sessions
     * @param mode Describes how new connections are treated when another session already exists
     * @param endpoint Network adapter to listen on (i.e. 127.0.0.1 or 0.0.0.0) and port
     * @param config TLS configuration information
     * @param listener optional callback interface (can be nullptr) for info about the running channel
     * @throw DNP3Error if the manager was already shutdown, if the library was compiled without TLS support
     *                  or if the server could not be binded properly
     * @return shared_ptr to a channel interface
     */
    std::shared_ptr<IChannel> AddTLSServer(const std::string& id,
                                           const opendnp3::LogLevels& levels,
                                           ServerAcceptMode mode,
                                           const IPEndpoint& endpoint,
                                           const TLSConfig& config,
                                           std::shared_ptr<IChannelListener> listener);

    /**
     * Create a TCP listener that will be used to accept incoming connections
     * @throw DNP3Error if the manager was already shutdown or if the server could not be binded properly
     */
    std::shared_ptr<IListener> CreateListener(std::string loggerid,
                                              const opendnp3::LogLevels& loglevel,
                                              const IPEndpoint& endpoint,
                                              const std::shared_ptr<IListenCallbacks>& callbacks);

    /**
     * Create a TLS listener that will be used to accept incoming connections
     * @throw DNP3Error if the manager was already shutdown, if the library was compiled without TLS support
     *                  or if the server could not be binded properly
     */
    std::shared_ptr<IListener> CreateListener(std::string loggerid,
                                              const opendnp3::LogLevels& loglevel,
                                              const IPEndpoint& endpoint,
                                              const TLSConfig& config,
                                              const std::shared_ptr<IListenCallbacks>& callbacks);

private:
    std::unique_ptr<DNP3ManagerImpl> impl;
};

} // namespace opendnp3

#endif
