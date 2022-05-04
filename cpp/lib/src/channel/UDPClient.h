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
#ifndef OPENDNP3_UDPCLIENT_H
#define OPENDNP3_UDPCLIENT_H

#include "channel/LoggingConnectionCondition.h"

#include "opendnp3/channel/IPEndpoint.h"
#include "opendnp3/util/Uncopyable.h"

#include <exe4cpp/asio/StrandExecutor.h>

namespace opendnp3
{

class UDPClient final : public std::enable_shared_from_this<UDPClient>, private Uncopyable
{

public:
    typedef std::function<void(
        const std::shared_ptr<exe4cpp::StrandExecutor>& executor, asio::ip::udp::socket, const std::error_code& ec)>
        connect_callback_t;

    static std::shared_ptr<UDPClient> Create(const Logger& logger,
                                             const std::shared_ptr<exe4cpp::StrandExecutor>& executor)
    {
        return std::make_shared<UDPClient>(logger, executor);
    }

    UDPClient(const Logger& logger, const std::shared_ptr<exe4cpp::StrandExecutor>& executor);

    bool Cancel();

    bool Open(const IPEndpoint& localEndpoint, const IPEndpoint& remoteEndpoint, connect_callback_t callback);

private:
    void HandleResolveResult(const connect_callback_t& callback,
                             const asio::ip::udp::resolver::iterator& endpoints,
                             const std::error_code& ec);

    bool PostConnectError(const connect_callback_t& callback, const std::error_code& ec);

    bool connecting = false;
    bool canceled = false;

    LoggingConnectionCondition condition;
    const std::shared_ptr<exe4cpp::StrandExecutor> executor;
    asio::ip::udp::socket socket;
    asio::ip::udp::resolver resolver;
};

} // namespace opendnp3

#endif
