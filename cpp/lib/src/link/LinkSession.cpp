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

#include "link/LinkSession.h"

#include "logging/LogMacros.h"
#include "master/MasterSchedulerBackend.h"

#include "opendnp3/logging/LogLevels.h"

#include <utility>

namespace opendnp3
{

LinkSession::LinkSession(const Logger& logger,
                         uint64_t sessionid,
                         std::shared_ptr<IResourceManager> manager,
                         std::shared_ptr<IListenCallbacks> callbacks,
                         const std::shared_ptr<IAsyncChannel>& channel)
    : logger(logger),
      session_id(sessionid),
      manager(std::move(manager)),
      callbacks(std::move(callbacks)),
      channel(channel),
      parser(logger)
{
}

void LinkSession::Shutdown()
{
    auto shutdown = [self = shared_from_this()]() { self->ShutdownImpl(); };

    this->channel->executor->block_until_and_flush(shutdown);
}

void LinkSession::ShutdownImpl()
{
    if (this->is_shutdown)
        return;

    this->is_shutdown = true;

    this->callbacks->OnConnectionClose(this->session_id, this->stack);

    if (this->stack)
    {
        this->stack->OnLowerLayerDown();
        this->stack->BeginShutdown();
    }

    this->first_frame_timer.cancel();

    this->channel->Shutdown();

    auto detach = [self = shared_from_this()]() { self->manager->Detach(self); };

    this->channel->executor->post(detach);
}

void LinkSession::SetLogFilters(const LogLevels& filters)
{
    this->logger.set_levels(filters);
}

void LinkSession::OnReadComplete(const std::error_code& ec, size_t num)
{
    if (ec)
    {
        SIMPLE_LOG_BLOCK(this->logger, flags::WARN, ec.message().c_str());
        this->ShutdownImpl();
    }
    else
    {
        this->parser.OnRead(num, *this);
        this->BeginReceive();
    }
}

void LinkSession::OnWriteComplete(const std::error_code& ec, size_t /*num*/)
{
    if (ec)
    {
        SIMPLE_LOG_BLOCK(this->logger, flags::WARN, ec.message().c_str());
        this->ShutdownImpl();
    }
    else
    {
        this->stack->OnTxReady();
    }
}

void LinkSession::BeginTransmit(const ser4cpp::rseq_t& buffer, ILinkSession& /*session*/)
{
    this->channel->BeginWrite(buffer);
}

bool LinkSession::OnFrame(const LinkHeaderFields& header, const ser4cpp::rseq_t& userdata)
{
    if (this->stack)
    {
        this->stack->OnFrame(header, userdata);
    }
    else
    {
        this->first_frame_timer.cancel();

        this->callbacks->OnFirstFrame(this->session_id, header, *this);

        if (this->stack)
        {
            this->stack->OnLowerLayerUp();

            // push the frame into the newly created stack
            this->stack->OnFrame(header, userdata);
        }
        else
        {
            SIMPLE_LOG_BLOCK(this->logger, flags::WARN, "No master created. Closing socket.");
            this->ShutdownImpl();
        }
    }

    return true;
}

std::shared_ptr<IMasterSession> LinkSession::AcceptSession(const std::string& loggerid,
                                                           std::shared_ptr<ISOEHandler> SOEHandler,
                                                           std::shared_ptr<IMasterApplication> application,
                                                           const MasterStackConfig& config)
{
    if (this->stack)
    {
        SIMPLE_LOG_BLOCK(this->logger, flags::ERR, "SocketSession already has a master bound");
        return nullptr;
    }

    // rename the logger id to something meaningful
    this->logger.rename(loggerid);

    this->stack = MasterSessionStack::Create(this->logger, this->channel->executor, SOEHandler, application,
                                             std::make_shared<MasterSchedulerBackend>(this->channel->executor),
                                             shared_from_this(), *this, config);

    return stack;
}

void LinkSession::Start()
{
    this->channel->SetCallbacks(shared_from_this());

    auto timeout = [self = shared_from_this()]() {
        SIMPLE_LOG_BLOCK(self->logger, flags::ERR, "Timed out before receving a frame. Closing socket.");
        self->channel->Shutdown();
    };

    this->first_frame_timer = this->channel->executor->start(this->callbacks->GetFirstFrameTimeout().value, timeout);

    this->BeginReceive();
}

void LinkSession::BeginReceive()
{
    auto dest = parser.WriteBuff();
    channel->BeginRead(dest);
}

} // namespace opendnp3
