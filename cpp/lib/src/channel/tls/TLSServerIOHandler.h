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

#ifndef OPENDNP3_TLSSERVERIOHANDLER_H
#define OPENDNP3_TLSSERVERIOHANDLER_H

#include "channel/IOHandler.h"
#include "channel/tls/TLSServer.h"

#include "opendnp3/channel/ChannelRetry.h"
#include "opendnp3/channel/IPEndpoint.h"
#include "opendnp3/gen/ServerAcceptMode.h"

#include <exe4cpp/Timer.h>

namespace opendnp3
{

class TLSServerIOHandler final : public IOHandler
{
    class Server final : public TLSServer
    {
    public:
        typedef std::function<void(const std::shared_ptr<IAsyncChannel>&)> callback_t;

        Server(const Logger& logger,
               const std::shared_ptr<exe4cpp::StrandExecutor>& executor,
               const IPEndpoint& endpoint,
               const TLSConfig& config,
               std::error_code& ec)
            : TLSServer(logger, executor, endpoint, config, ec)
        {
        }

        void StartAcceptingConnection(const callback_t& callback)
        {
            this->callback = callback;
            this->StartAccept();
        }

    private:
        callback_t callback;

        virtual bool AcceptConnection(uint64_t sessionid, const asio::ip::tcp::endpoint& remote) override
        {
            return true;
        }
        virtual bool VerifyCallback(uint64_t sessionid, bool preverified, asio::ssl::verify_context& ctx) override
        {
            return preverified;
        }
        virtual void OnShutdown() override {}

        virtual void AcceptStream(uint64_t sessionid,
                                  const std::shared_ptr<exe4cpp::StrandExecutor>& executor,
                                  std::shared_ptr<asio::ssl::stream<asio::ip::tcp::socket>> stream) override;
    };

public:
    static std::shared_ptr<TLSServerIOHandler> Create(const Logger& logger,
                                                      ServerAcceptMode mode,
                                                      const std::shared_ptr<IChannelListener>& listener,
                                                      const std::shared_ptr<exe4cpp::StrandExecutor>& executor,
                                                      const IPEndpoint& endpoint,
                                                      const TLSConfig& config,
                                                      std::error_code& ec)
    {
        return std::make_shared<TLSServerIOHandler>(logger, mode, listener, executor, endpoint, config, ec);
    }

    TLSServerIOHandler(const Logger& logger,
                       ServerAcceptMode mode,
                       const std::shared_ptr<IChannelListener>& listener,
                       std::shared_ptr<exe4cpp::StrandExecutor> executor,
                       IPEndpoint endpoint,
                       TLSConfig config,
                       std::error_code& ec);

protected:
    virtual void ShutdownImpl() override;
    virtual void BeginChannelAccept() override;
    virtual void SuspendChannelAccept() override;
    virtual void OnChannelShutdown() override {} // do nothing, always accepting new connections

private:
    const std::shared_ptr<exe4cpp::StrandExecutor> executor;
    const IPEndpoint endpoint;
    const TLSConfig config;
    std::shared_ptr<Server> server;
};

} // namespace opendnp3

#endif
