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

#include "channel/IOHandler.h"

#include "logging/LogMacros.h"

#include "opendnp3/logging/LogLevels.h"

#include <utility>

namespace opendnp3
{

IOHandler::IOHandler(const Logger& logger, bool close_existing, std::shared_ptr<IChannelListener> listener)
    : close_existing(close_existing), logger(logger), listener(std::move(listener)), parser(logger)
{
}

void IOHandler::Shutdown()
{
    if (!isShutdown)
    {
        this->isShutdown = true;

        this->Reset();

        this->ShutdownImpl();

        this->UpdateListener(ChannelState::SHUTDOWN);
    }
}

void IOHandler::OnReadComplete(const std::error_code& ec, size_t num)
{
    if (ec)
    {
        SIMPLE_LOG_BLOCK(this->logger, flags::WARN, ec.message().c_str());

        this->Reset();

        this->UpdateListener(ChannelState::OPENING);
        this->OnChannelShutdown();
    }
    else
    {
        this->statistics.numBytesRx += num;

        this->parser.OnRead(num, *this);
        this->BeginRead();
    }
}

void IOHandler::OnWriteComplete(const std::error_code& ec, size_t num)
{

    if (ec)
    {
        SIMPLE_LOG_BLOCK(this->logger, flags::WARN, ec.message().c_str());
        this->Reset();

        this->UpdateListener(ChannelState::OPENING);
        this->OnChannelShutdown();
    }
    else
    {
        this->statistics.numBytesTx += num;

        if (!this->txQueue.empty())
        {
            const auto session = this->txQueue.front().session;
            this->txQueue.pop_front();
            session->OnTxReady();
        }

        this->CheckForSend();
    }
}

void IOHandler::BeginTransmit(const std::shared_ptr<ILinkSession>& session, const ser4cpp::rseq_t& data)
{
    if (this->channel)
    {
        this->txQueue.emplace_back(data, session);
        this->CheckForSend();
    }
    else
    {
        SIMPLE_LOG_BLOCK(logger, flags::ERR, "Router received transmit request while offline");
    }
}

bool IOHandler::AddContext(const std::shared_ptr<ILinkSession>& session, const Addresses& addresses)
{
    if (this->IsRouteInUse(addresses))
    {
        FORMAT_LOG_BLOCK(logger, flags::ERR, "Route already in use: %u -> %u", addresses.source, addresses.destination);
        return false;
    }

    if (this->IsSessionInUse(session))
    {
        SIMPLE_LOG_BLOCK(logger, flags::ERR, "Context cannot be bound 2x");
        return false;
    }

    sessions.emplace_back(session, addresses); // record is always disabled by default

    return true;
}

bool IOHandler::Enable(const std::shared_ptr<ILinkSession>& session)
{
    auto matches = [&](const Session& rec) { return rec.Matches(session); };

    const auto iter = std::find_if(sessions.begin(), sessions.end(), matches);

    if (iter == sessions.end())
        return false;

    if (iter->enabled)
        return true; // already enabled

    iter->enabled = true;

    if (this->channel)
    {
        iter->LowerLayerUp();
    }
    else
    {
        this->UpdateListener(ChannelState::OPENING);

        this->BeginChannelAccept();
    }

    return true;
}

bool IOHandler::Disable(const std::shared_ptr<ILinkSession>& session)
{
    auto matches = [&](const Session& rec) { return rec.Matches(session); };

    const auto iter = std::find_if(sessions.begin(), sessions.end(), matches);

    if (iter == sessions.end())
        return false;

    if (!iter->enabled)
        return true; // already disabled

    iter->enabled = false;

    if (channel)
    {
        iter->LowerLayerDown();
    }

    if (!this->IsAnySessionEnabled())
    {
        this->Reset();
        this->SuspendChannelAccept();
    }

    return true;
}

bool IOHandler::Remove(const std::shared_ptr<ILinkSession>& session)
{
    auto matches = [&](const Session& rec) { return rec.Matches(session); };

    const auto iter = std::find_if(sessions.begin(), sessions.end(), matches);

    if (iter == sessions.end())
        return false;

    if (channel)
    {
        iter->LowerLayerDown();
    }

    sessions.erase(iter);

    if (!this->IsAnySessionEnabled())
    {
        this->SuspendChannelAccept();
    }

    return true;
}

void IOHandler::OnNewChannel(const std::shared_ptr<IAsyncChannel>& channel)
{
    // if we have an active channel, and we're configured to close new channels
    // close the new channel instead
    if (this->channel && !this->close_existing)
    {
        channel->Shutdown();
        return;
    }

    ++this->statistics.numOpen;

    this->Reset();

    this->channel = channel;

    this->channel->SetCallbacks(shared_from_this());

    this->UpdateListener(ChannelState::OPEN);

    this->BeginRead();

    for (auto& session : this->sessions)
    {
        if (session.enabled)
        {
            session.LowerLayerUp();
        }
    }
}

bool IOHandler::OnFrame(const LinkHeaderFields& header, const ser4cpp::rseq_t& userdata)
{
    if (this->SendToSession(header.addresses, header, userdata))
    {
        return true;
    }

    FORMAT_LOG_BLOCK(this->logger, flags::WARN, "Frame w/ unknown route, source: %i, dest %i", header.addresses.source,
                     header.addresses.destination);
    return false;
}

void IOHandler::BeginRead()
{
    this->channel->BeginRead(this->parser.WriteBuff());
}

void IOHandler::CheckForSend()
{
    if (this->txQueue.empty() || !this->channel || !this->channel->CanWrite())
        return;

    ++statistics.numLinkFrameTx;
    this->channel->BeginWrite(this->txQueue.front().txdata);
}

bool IOHandler::SendToSession(const Addresses& /*addresses*/,
                              const LinkHeaderFields& header,
                              const ser4cpp::rseq_t& userdata)
{
    bool accepted = false;

    for (auto& session : sessions)
    {
        if (session.enabled)
        {
            accepted |= session.OnFrame(header, userdata);
        }
    }

    return accepted;
}

bool IOHandler::IsRouteInUse(const Addresses& addresses) const
{
    auto matches = [addresses](const Session& record) { return record.Matches(addresses); };

    return std::find_if(sessions.begin(), sessions.end(), matches) != sessions.end();
}

bool IOHandler::IsSessionInUse(const std::shared_ptr<ILinkSession>& session) const
{
    auto matches = [&](const Session& record) { return record.Matches(session); };

    return std::find_if(sessions.begin(), sessions.end(), matches) != sessions.end();
}

bool IOHandler::IsAnySessionEnabled() const
{
    auto matches = [&](const Session& record) { return record.enabled; };

    return std::find_if(sessions.begin(), sessions.end(), matches) != sessions.end();
}

void IOHandler::Reset()
{
    if (this->channel)
    {
        // shutdown the existing channel and drop the reference to it
        this->channel->Shutdown();
        this->channel.reset();

        ++this->statistics.numClose;

        this->UpdateListener(ChannelState::CLOSED);

        // notify any sessions that are online that this layer is offline
        for (auto& item : this->sessions)
        {
            item.LowerLayerDown();
        }
    }

    // reset the state of the parser
    this->parser.Reset();

    // clear any pending tranmissions
    this->txQueue.clear();
}

} // namespace opendnp3
