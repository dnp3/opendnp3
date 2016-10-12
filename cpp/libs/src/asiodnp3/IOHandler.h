/*
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */
#ifndef ASIODNP3_IOHANDLER_H
#define ASIODNP3_IOHANDLER_H

#include "opendnp3/link/ILinkTx.h"
#include "opendnp3/Route.h"

#include "asiodnp3/IChannelListener.h"

#include "openpal/logging/Logger.h"
#include "opendnp3/link/LinkLayerParser.h"
#include "opendnp3/master/MultidropTaskLock.h"

#include "asiopal/IChannelFactory.h"
#include "asiopal/IAsyncChannel.h"

#include <vector>
#include <deque>

namespace asiodnp3
{

/**

Manages I/O for a number of link contexts

*/
class IOHandler : private opendnp3::IFrameSink, public std::enable_shared_from_this<IOHandler>
{

public:

	IOHandler(
	    const openpal::Logger& logger,
	    const std::shared_ptr<asiopal::IChannelFactory>& factory,
	    const std::shared_ptr<IChannelListener>& listener
	);

	static std::shared_ptr<IOHandler> Create(
	    const openpal::Logger& logger,
	    const std::shared_ptr<asiopal::IChannelFactory>& factory,
	    const std::shared_ptr<IChannelListener>& listener
	)
	{
		return std::make_shared<IOHandler>(logger, factory, listener);
	}

	const opendnp3::LinkChannelStatistics& Statistics() const
	{
		return this->statistics;
	}

	opendnp3::ITaskLock& TaskLock()
	{
		return this->taskLock;
	}

	void Shutdown();

	/// --- implement ILinkTx ---

	void BeginTransmit(const std::shared_ptr<opendnp3::ILinkSession>& session, const openpal::RSlice& data);

	// Bind a link layer session to the handler
	bool AddContext(const std::shared_ptr<opendnp3::ILinkSession>& session, const opendnp3::Route& route);

	// Begin sending messages to the context
	bool Enable(const std::shared_ptr<opendnp3::ILinkSession>& session);

	// Stop sending messages to this session
	bool Disable(const std::shared_ptr<opendnp3::ILinkSession>& session);

	// Remove this session entirely
	bool Remove(const std::shared_ptr<opendnp3::ILinkSession>& session);

	// Query to see if a route is in use
	bool IsRouteInUse(const opendnp3::Route& route) const;

private:

	// Called when a new channel is available
	void OnNewChannel(const std::shared_ptr<asiopal::IAsyncChannel>& channel);

	inline void UpdateListener(opendnp3::ChannelState state)
	{
		if (listener) listener->OnStateChange(state);
	}

	// called by the parser when a complete frame is read
	virtual bool OnFrame(const opendnp3::LinkHeaderFields& header, const openpal::RSlice& userdata) override final;


	bool IsSessionInUse(const std::shared_ptr<opendnp3::ILinkSession>& session) const;
	bool IsAnySessionEnabled() const;
	void Reset();
	void BeginRead();
	void CheckForSend();

	opendnp3::ILinkSession* GetEnabledSession(const opendnp3::Route&);

	struct Session
	{
		Session(const std::shared_ptr<opendnp3::ILinkSession>& session, const opendnp3::Route& route) :
			route(route),
			session(session)
		{}

		Session() = default;

		bool enabled = false;
		opendnp3::Route route;
		std::shared_ptr<opendnp3::ILinkSession> session;
	};

	struct Transmission
	{
		Transmission(const openpal::RSlice& txdata, const std::shared_ptr<opendnp3::ILinkSession>& session) :
			txdata(txdata),
			session(session)
		{}

		Transmission() = default;

		openpal::RSlice txdata;
		std::shared_ptr<opendnp3::ILinkSession> session;
	};

	std::vector<Session> sessions;
	std::deque<Transmission>  txQueue;

	openpal::Logger logger;
	const std::shared_ptr<asiopal::IChannelFactory> factory;
	const std::shared_ptr<IChannelListener> listener;

	opendnp3::LinkChannelStatistics statistics;
	opendnp3::LinkLayerParser parser;

	// current value of the channel, may be empty
	std::shared_ptr<asiopal::IAsyncChannel> channel;

	opendnp3::MultidropTaskLock taskLock;
};

}

#endif

