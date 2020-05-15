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
#ifndef OPENDNP3_MASTERTLSSERVER_H
#define OPENDNP3_MASTERTLSSERVER_H

#include "ResourceManager.h"
#include "channel/tls/TLSServer.h"

#include "opendnp3/channel/IPEndpoint.h"
#include "opendnp3/channel/TLSConfig.h"
#include "opendnp3/logging/Logger.h"
#include "opendnp3/master/IListenCallbacks.h"

namespace opendnp3
{

class MasterTLSServer final : public TLSServer
{

public:
    MasterTLSServer(const Logger& logger,
                    const std::shared_ptr<exe4cpp::StrandExecutor>& executor,
                    const IPEndpoint& endpoint,
                    const TLSConfig& tlsConfig,
                    std::shared_ptr<IListenCallbacks> callbacks,
                    std::shared_ptr<ResourceManager> manager,
                    std::error_code& ec);

    static std::shared_ptr<MasterTLSServer> Create(const Logger& logger,
                                                   const std::shared_ptr<exe4cpp::StrandExecutor> executor,
                                                   const IPEndpoint endpoint,
                                                   const TLSConfig& tlsConfig,
                                                   const std::shared_ptr<IListenCallbacks> callbacks,
                                                   const std::shared_ptr<ResourceManager>& manager,
                                                   std::error_code& ec)
    {
        auto ret = std::make_shared<MasterTLSServer>(logger, executor, endpoint, tlsConfig, callbacks, manager, ec);

        if (ec)
            return nullptr;

        ret->StartAccept();

        return ret;
    }

    virtual bool AcceptConnection(uint64_t sessionid, const asio::ip::tcp::endpoint& remote) override;

    virtual bool VerifyCallback(uint64_t sessionid, bool preverified, asio::ssl::verify_context& ctx) override;

    virtual void AcceptStream(uint64_t sessionid,
                              const std::shared_ptr<exe4cpp::StrandExecutor>& executor,
                              std::shared_ptr<asio::ssl::stream<asio::ip::tcp::socket>> stream) override;

    virtual void OnShutdown() override;

private:
    std::shared_ptr<IListenCallbacks> callbacks;
    std::shared_ptr<ResourceManager> manager;

    static std::string SessionIdToString(uint64_t sessionid);
};

} // namespace opendnp3

#endif
