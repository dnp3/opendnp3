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
#ifndef OPENDNP3_IOHANDLER_H
#define OPENDNP3_IOHANDLER_H

#include "channel/IAsyncChannel.h"
#include "link/ILinkTx.h"
#include "link/LinkLayerParser.h"

#include "opendnp3/channel/IChannelListener.h"
#include "opendnp3/link/Addresses.h"
#include "opendnp3/logging/Logger.h"

#include <deque>
#include <vector>

namespace opendnp3
{

/**

Manages I/O for a number of link contexts

*/
class IOHandler : private IFrameSink, public IChannelCallbacks, public std::enable_shared_from_this<IOHandler>
{

public:
    IOHandler(const Logger& logger, bool close_existing, std::shared_ptr<IChannelListener> listener);

    virtual ~IOHandler() = default;

    LinkStatistics Statistics() const
    {
        return LinkStatistics(this->statistics, this->parser.Statistics());
    }

    void Shutdown();

    /// --- implement ILinkTx ---

    void BeginTransmit(const std::shared_ptr<ILinkSession>& session, const ser4cpp::rseq_t& data);

    // Bind a link layer session to the handler
    bool AddContext(const std::shared_ptr<ILinkSession>& session, const Addresses& addresses);

    // Begin sending messages to the context
    bool Enable(const std::shared_ptr<ILinkSession>& session);

    // Stop sending messages to this session
    bool Disable(const std::shared_ptr<ILinkSession>& session);

    // Remove this session entirely
    bool Remove(const std::shared_ptr<ILinkSession>& session);

    // Query to see if a route is in use
    bool IsRouteInUse(const Addresses& addresses) const;

protected:
    // ------ Implement IChannelCallbacks -----

    void OnReadComplete(const std::error_code& ec, size_t num) final;

    void OnWriteComplete(const std::error_code& ec, size_t num) final;

    // ------ Super classes will implement these -----

    // start getting a new channel
    virtual void BeginChannelAccept() = 0;

    // stop getting new channels
    virtual void SuspendChannelAccept() = 0;

    // shutdown any additional state
    virtual void ShutdownImpl() = 0;

    // the current channel has closed, start getting a new one
    virtual void OnChannelShutdown() = 0;

    // Called by the super class when a new channel is available
    void OnNewChannel(const std::shared_ptr<IAsyncChannel>& channel);

    const bool close_existing;
    Logger logger;
    const std::shared_ptr<IChannelListener> listener;
    LinkStatistics::Channel statistics;

private:
    bool isShutdown = false;

    inline void UpdateListener(ChannelState state)
    {
        if (listener)
            listener->OnStateChange(state);
    }

    // called by the parser when a complete frame is read
    bool OnFrame(const LinkHeaderFields& header, const ser4cpp::rseq_t& userdata) final;

    bool IsSessionInUse(const std::shared_ptr<ILinkSession>& session) const;
    bool IsAnySessionEnabled() const;
    void Reset();
    void BeginRead();
    void CheckForSend();

    bool SendToSession(const Addresses& addresses, const LinkHeaderFields& header, const ser4cpp::rseq_t& userdata);

    class Session
    {

    public:
        Session(const std::shared_ptr<ILinkSession>& session, const Addresses& addresses)
            : addresses(addresses), session(session)
        {
        }

        Session() = default;

        inline bool Matches(const std::shared_ptr<ILinkSession>& session) const
        {
            return this->session == session;
        }
        inline bool Matches(const Addresses& addresses) const
        {
            return this->addresses == addresses;
        }

        inline bool OnFrame(const LinkHeaderFields& header, const ser4cpp::rseq_t& userdata)
        {
            return this->session->OnFrame(header, userdata);
        }

        inline bool LowerLayerUp()
        {
            if (!online)
            {
                online = true;
                return this->session->OnLowerLayerUp();
            }
            else
            {
                return false;
            }
        }

        inline bool LowerLayerDown()
        {
            if (online)
            {
                online = false;
                return this->session->OnLowerLayerDown();
            }
            else
            {
                return false;
            }
        }

        bool enabled = false;

    private:
        Addresses addresses;
        bool online = false;
        std::shared_ptr<ILinkSession> session;
    };

    struct Transmission
    {
        Transmission(const ser4cpp::rseq_t& txdata, const std::shared_ptr<ILinkSession>& session)
            : txdata(txdata), session(session)
        {
        }

        Transmission() = default;

        ser4cpp::rseq_t txdata;
        std::shared_ptr<ILinkSession> session;
    };

    std::vector<Session> sessions;
    std::deque<Transmission> txQueue;

    LinkLayerParser parser;

    // current value of the channel, may be empty
    std::shared_ptr<IAsyncChannel> channel;
};

} // namespace opendnp3

#endif
