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

#ifndef OPENDNP3_LINKSESSION_H
#define OPENDNP3_LINKSESSION_H

#include "IResourceManager.h"
#include "channel/IAsyncChannel.h"
#include "link/ILinkTx.h"
#include "link/LinkLayerParser.h"
#include "master/MasterSessionStack.h"

#include "opendnp3/link/LinkStatistics.h"
#include "opendnp3/logging/Logger.h"
#include "opendnp3/master/IListenCallbacks.h"

#include <exe4cpp/Timer.h>

namespace opendnp3
{

class LinkSession final : public ILinkTx,
                          public IChannelCallbacks,
                          private IFrameSink,
                          public std::enable_shared_from_this<LinkSession>,
                          public IResource,
                          private ISessionAcceptor,
                          private Uncopyable
{
public:
    static std::shared_ptr<LinkSession> Create(const Logger& logger,
                                               uint64_t sessionid,
                                               const std::shared_ptr<IResourceManager>& manager,
                                               const std::shared_ptr<IListenCallbacks>& callbacks,
                                               const std::shared_ptr<IAsyncChannel>& channel)
    {
        auto session = std::make_shared<LinkSession>(logger, sessionid, manager, callbacks, channel);

        session->Start();

        return session;
    }

    LinkSession(const Logger& logger,
                uint64_t sessionid,
                std::shared_ptr<IResourceManager> manager,
                std::shared_ptr<IListenCallbacks> callbacks,
                const std::shared_ptr<IAsyncChannel>& channel);

    // override IResource
    void Shutdown() final;

    void SetLogFilters(const opendnp3::LogLevels& filters);

private:
    void ShutdownImpl();

    // IChannelCallbacks
    void OnReadComplete(const std::error_code& ec, size_t num) final;

    void OnWriteComplete(const std::error_code& ec, size_t num) final;

    // ILinkTx
    void BeginTransmit(const ser4cpp::rseq_t& buffer, ILinkSession& session) final;

    // IFrameSink
    bool OnFrame(const LinkHeaderFields& header, const ser4cpp::rseq_t& userdata) final;

    // ISessionAcceptor
    std::shared_ptr<IMasterSession> AcceptSession(const std::string& loggerid,
                                                  std::shared_ptr<ISOEHandler> SOEHandler,
                                                  std::shared_ptr<IMasterApplication> application,
                                                  const MasterStackConfig& config) final;

    void Start();

    void BeginReceive();

    bool is_shutdown = false;
    Logger logger;
    const uint64_t session_id;

    const std::shared_ptr<IResourceManager> manager;
    const std::shared_ptr<IListenCallbacks> callbacks;
    const std::shared_ptr<IAsyncChannel> channel;

    LinkLayerParser parser;
    exe4cpp::Timer first_frame_timer;

    std::shared_ptr<MasterSessionStack> stack; // initialized to null
};
} // namespace opendnp3

#endif
