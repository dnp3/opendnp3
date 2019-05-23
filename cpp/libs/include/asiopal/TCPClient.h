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
#ifndef ASIOPAL_TCPCLIENT_H
#define ASIOPAL_TCPCLIENT_H

#include "asiopal/Executor.h"
#include "asiopal/IPEndpoint.h"
#include "asiopal/LoggingConnectionCondition.h"

namespace asiopal
{

class TCPClient final : public std::enable_shared_from_this<TCPClient>, private openpal::Uncopyable
{

public:
    typedef std::function<void(
        const std::shared_ptr<Executor>& executor, asio::ip::tcp::socket, const std::error_code& ec)>
        connect_callback_t;

    static std::shared_ptr<TCPClient> Create(const openpal::Logger& logger,
                                             const std::shared_ptr<Executor>& executor,
                                             const std::string& adapter)
    {
        return std::make_shared<TCPClient>(logger, executor, adapter);
    }

    TCPClient(const openpal::Logger& logger, const std::shared_ptr<Executor>& executor, std::string adapter);

    bool Cancel();

    bool BeginConnect(const IPEndpoint& remote, const connect_callback_t& callback);

private:
    void HandleResolveResult(const connect_callback_t& callback,
                             const asio::ip::tcp::resolver::iterator& endpoints,
                             const std::error_code& ec);

    bool PostConnectError(const connect_callback_t& callback, const std::error_code& ec);

    bool connecting = false;
    bool canceled = false;

    LoggingConnectionCondition condition;
    const std::shared_ptr<Executor> executor;
    const std::string adapter;
    asio::ip::tcp::socket socket;
    asio::ip::tcp::endpoint localEndpoint;
    asio::ip::tcp::resolver resolver;
};

} // namespace asiopal

#endif
