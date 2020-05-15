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
#ifndef OPENDNP3_UDPCLIENTIOHANDLER_H
#define OPENDNP3_UDPCLIENTIOHANDLER_H

#include "channel/IOHandler.h"
#include "channel/UDPClient.h"

#include "opendnp3/channel/ChannelRetry.h"
#include "opendnp3/channel/IPEndpoint.h"

#include <exe4cpp/Timer.h>

namespace opendnp3
{

class UDPClientIOHandler final : public IOHandler
{

public:
    static std::shared_ptr<UDPClientIOHandler> Create(const Logger& logger,
                                                      const std::shared_ptr<IChannelListener>& listener,
                                                      const std::shared_ptr<exe4cpp::StrandExecutor>& executor,
                                                      const ChannelRetry& retry,
                                                      const IPEndpoint& localEndpoint,
                                                      const IPEndpoint& remoteEndpoint)
    {
        return std::make_shared<UDPClientIOHandler>(logger, listener, executor, retry, localEndpoint, remoteEndpoint);
    }

    UDPClientIOHandler(const Logger& logger,
                       const std::shared_ptr<IChannelListener>& listener,
                       const std::shared_ptr<exe4cpp::StrandExecutor>& executor,
                       const ChannelRetry& retry,
                       const IPEndpoint& localEndpoint,
                       const IPEndpoint& remoteEndpoint);

protected:
    void ShutdownImpl() final;
    void BeginChannelAccept() final;
    void SuspendChannelAccept() final;
    void OnChannelShutdown() final;

private:
    bool TryOpen(const TimeDuration& delay);

    void ResetState();

    const std::shared_ptr<exe4cpp::StrandExecutor> executor;
    const ChannelRetry retry;
    const IPEndpoint localEndpoint;
    const IPEndpoint remoteEndpoint;

    // current value of the client
    std::shared_ptr<UDPClient> client;

    // connection retry timer
    exe4cpp::Timer retrytimer;
};

} // namespace opendnp3

#endif
