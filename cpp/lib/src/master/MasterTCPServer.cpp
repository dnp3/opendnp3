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

#include "master/MasterTCPServer.h"

#include "channel/TCPSocketChannel.h"
#include "link/LinkSession.h"
#include "logging/LogMacros.h"

#include "opendnp3/logging/LogLevels.h"

#include <sstream>
#include <utility>

namespace opendnp3
{

MasterTCPServer::MasterTCPServer(const Logger& logger,
                                 const std::shared_ptr<exe4cpp::StrandExecutor>& executor,
                                 const IPEndpoint& endpoint,
                                 std::shared_ptr<IListenCallbacks> callbacks,
                                 std::shared_ptr<ResourceManager> manager,
                                 std::error_code& ec)
    : TCPServer(logger, executor, endpoint, ec), callbacks(std::move(callbacks)), manager(std::move(manager))
{
}

void MasterTCPServer::OnShutdown()
{
    this->manager->Detach(this->shared_from_this());
}

void MasterTCPServer::AcceptConnection(uint64_t sessionid,
                                       const std::shared_ptr<exe4cpp::StrandExecutor>& executor,
                                       asio::ip::tcp::socket socket)
{
    std::ostringstream oss;
    oss << socket.remote_endpoint();

    if (this->callbacks->AcceptConnection(sessionid, socket.remote_endpoint().address().to_string()))
    {
        FORMAT_LOG_BLOCK(this->logger, flags::INFO, "Accepted connection from: %s", oss.str().c_str());

        auto channel
            = TCPSocketChannel::Create(executor->fork(), std::move(socket)); // run the link session in its own strand

        auto create = [&]() -> std::shared_ptr<LinkSession> {
            return LinkSession::Create(this->logger.detach(SessionIdToString(sessionid)), sessionid, this->manager,
                                       this->callbacks, channel);
        };

        if (!this->manager->Bind<LinkSession>(create))
        {
            channel->Shutdown();
        }
    }
    else
    {
        socket.close();
        FORMAT_LOG_BLOCK(this->logger, flags::INFO, "Rejected connection from: %s", oss.str().c_str());
    }
}

std::string MasterTCPServer::SessionIdToString(uint64_t sessionid)
{
    std::ostringstream oss;
    oss << "session-" << sessionid;
    return oss.str();
}

} // namespace opendnp3
