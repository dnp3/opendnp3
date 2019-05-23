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

#ifndef ASIODNP3_LINKSESSION_H
#define ASIODNP3_LINKSESSION_H

#include <openpal/executor/TimerRef.h>
#include <openpal/logging/Logger.h>

#include <asiopal/Executor.h>
#include <asiopal/IAsyncChannel.h>
#include <asiopal/IResourceManager.h>

#include <opendnp3/Route.h>
#include <opendnp3/link/ILinkTx.h>
#include <opendnp3/link/LinkLayerParser.h>
#include <opendnp3/link/LinkStatistics.h>

#include "asiodnp3/IListenCallbacks.h"
#include "asiodnp3/MasterSessionStack.h"

namespace asiodnp3
{
class LinkSession final : public opendnp3::ILinkTx,
                          public asiopal::IChannelCallbacks,
                          private opendnp3::IFrameSink,
                          public std::enable_shared_from_this<LinkSession>,
                          public asiopal::IResource,
                          private ISessionAcceptor,
                          private openpal::Uncopyable
{
public:
    static std::shared_ptr<LinkSession> Create(const openpal::Logger& logger,
                                               uint64_t sessionid,
                                               const std::shared_ptr<asiopal::IResourceManager>& manager,
                                               const std::shared_ptr<IListenCallbacks>& callbacks,
                                               const std::shared_ptr<asiopal::IAsyncChannel>& channel)
    {
        auto session = std::make_shared<LinkSession>(logger, sessionid, manager, callbacks, channel);

        session->Start();

        return session;
    }

    LinkSession(const openpal::Logger& logger,
                uint64_t sessionid,
                std::shared_ptr<asiopal::IResourceManager> manager,
                std::shared_ptr<IListenCallbacks> callbacks,
                const std::shared_ptr<asiopal::IAsyncChannel>& channel);

    // override IResource
    void Shutdown() override;

    void SetLogFilters(openpal::LogFilters filters);

private:
    void ShutdownImpl();

    // IChannelCallbacks
    virtual void OnReadComplete(const std::error_code& ec, size_t num) override;

    virtual void OnWriteComplete(const std::error_code& ec, size_t num) override;

    // ILinkTx
    virtual void BeginTransmit(const openpal::RSlice& buffer, opendnp3::ILinkSession& session) override;

    // IFrameSink
    virtual bool OnFrame(const opendnp3::LinkHeaderFields& header, const openpal::RSlice& userdata) override;

    // ISessionAcceptor
    virtual std::shared_ptr<IMasterSession> AcceptSession(const std::string& loggerid,
                                                          std::shared_ptr<opendnp3::ISOEHandler> SOEHandler,
                                                          std::shared_ptr<opendnp3::IMasterApplication> application,
                                                          const MasterStackConfig& config) override;

    void Start();

    void BeginReceive();

    bool is_shutdown = false;
    openpal::Logger logger;
    const uint64_t session_id;

    const std::shared_ptr<asiopal::IResourceManager> manager;
    const std::shared_ptr<IListenCallbacks> callbacks;
    const std::shared_ptr<asiopal::IAsyncChannel> channel;

    opendnp3::LinkLayerParser parser;
    openpal::TimerRef first_frame_timer;
    opendnp3::Route route;

    std::shared_ptr<MasterSessionStack> stack; // initialized to null
};
} // namespace asiodnp3

#endif
