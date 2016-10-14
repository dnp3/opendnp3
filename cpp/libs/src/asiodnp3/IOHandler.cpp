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

#include "asiodnp3/IOHandler.h"

#include "openpal/logging/LogMacros.h"
#include "opendnp3/LogLevels.h"

using namespace openpal;
using namespace opendnp3;

namespace asiodnp3
{

IOHandler::IOHandler(
    const openpal::Logger& logger,
    const std::shared_ptr<IChannelListener>& listener
) :
	logger(logger),
	listener(listener),
	parser(logger, &statistics)
{

}

void IOHandler::Shutdown()
{
	if (!isShutdown)
	{
		this->isShutdown = true;

		this->ShutdownImpl();

		this->UpdateListener(ChannelState::SHUTDOWN);

		if (channel)
		{
			this->channel->Shutdown();
		}
	}
}

void IOHandler::BeginTransmit(const std::shared_ptr<opendnp3::ILinkSession>& session, const RSlice& data)
{
	if (this->channel)
	{
		this->txQueue.push_back(Transmission(data, session));
		this->CheckForSend();
	}
	else
	{
		SIMPLE_LOG_BLOCK(logger, flags::ERR, "Router received transmit request while offline");
	}
}

bool IOHandler::AddContext(const std::shared_ptr<opendnp3::ILinkSession>& session, const Route& route)
{
	if (this->IsRouteInUse(route))
	{
		FORMAT_LOG_BLOCK(logger, flags::ERR, "Route already in use: %u -> %u", route.source, route.destination);
		return false;
	}

	if (this->IsSessionInUse(session))
	{
		SIMPLE_LOG_BLOCK(logger, flags::ERR, "Context cannot be bound 2x");
		return false;
	}

	sessions.push_back(Session(session, route)); // record is always disabled by default

	return true;
}

bool IOHandler::Enable(const std::shared_ptr<opendnp3::ILinkSession>& session)
{
	auto matches = [&](const Session & rec)
	{
		return rec.Matches(session);
	};

	const auto iter = std::find_if(sessions.begin(), sessions.end(), matches);

	if (iter == sessions.end()) return false;

	if (iter->enabled) return true; // already enabled

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

bool IOHandler::Disable(const std::shared_ptr<opendnp3::ILinkSession>& session)
{
	auto matches = [&](const Session & rec)
	{
		return rec.Matches(session);
	};

	const auto iter = std::find_if(sessions.begin(), sessions.end(), matches);

	if (iter == sessions.end()) return false;

	if (!iter->enabled) return true; // already disabled

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

bool IOHandler::Remove(const std::shared_ptr<opendnp3::ILinkSession>& session)
{
	auto matches = [&](const Session & rec)
	{
		return rec.Matches(session);
	};

	const auto iter = std::find_if(sessions.begin(), sessions.end(), matches);

	if (iter == sessions.end()) return false;

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

void IOHandler::OnNewChannel(const std::shared_ptr<asiopal::IAsyncChannel>& channel)
{
	this->Reset();

	this->channel = channel;

	this->UpdateListener(ChannelState::OPEN);

	this->BeginRead();

	this->taskLock.SetOnline();

	for (auto& session : this->sessions)
	{
		session.LowerLayerUp();
	}
}

bool IOHandler::OnFrame(const LinkHeaderFields& header, const openpal::RSlice& userdata)
{
	if (this->SendToSession(Route(header.src, header.dest), header, userdata))
	{
		return true;
	}
	else
	{
		FORMAT_LOG_BLOCK_WITH_CODE(this->logger, flags::WARN, DLERR_UNKNOWN_ROUTE, "Frame w/ unknown route, source: %i, dest %i", header.src, header.dest);
		return false;
	}
}

void IOHandler::BeginRead()
{
	// we don't need to capture shared_ptr here, b/c shutting down an
	// IAsyncChannel guarantees that the callback isn't invoked
	auto cb = [self = shared_from_this()](const std::error_code & ec, std::size_t num)
	{
		if (ec)
		{
			SIMPLE_LOG_BLOCK(self->logger, flags::WARN, ec.message().c_str());

			self->Reset();

			self->UpdateListener(ChannelState::OPENING);
			self->OnChannelShutdown();
		}
		else
		{
			self->parser.OnRead(static_cast<uint32_t>(num), *self);
			self->BeginRead();
		}
	};

	this->channel->BeginRead(this->parser.WriteBuff(), cb);
}

void IOHandler::CheckForSend()
{
	if (this->txQueue.empty() || !this->channel || !this->channel->CanWrite()) return;

	++statistics.numLinkFrameTx;
	const auto tx = this->txQueue.front();
	this->txQueue.pop_front();

	// we don't need to capture shared_ptr here, b/c shutting down an
	// IAsyncChannel guarantees that the callback isn't invoked
	auto cb = [self = this->shared_from_this(), session = tx.session](const std::error_code & ec)
	{
		if (ec)
		{
			SIMPLE_LOG_BLOCK(self->logger, flags::WARN, ec.message().c_str());
			self->Reset();

			auto cb = [self](const std::shared_ptr<asiopal::IAsyncChannel>& channel)
			{
				self->OnNewChannel(channel);
			};

			self->UpdateListener(ChannelState::OPENING);
			self->OnChannelShutdown();
		}
		else
		{
			self->CheckForSend();
			session->OnTransmitResult(true);
		}
	};

	this->channel->BeginWrite(tx.txdata, cb);
}

bool IOHandler::SendToSession(const opendnp3::Route& route, const opendnp3::LinkHeaderFields& header, const openpal::RSlice& userdata)
{
	auto matches = [route](const Session & session)
	{
		return session.enabled && session.Matches(route);
	};

	const auto iter = std::find_if(sessions.begin(), sessions.end(), matches);

	if (iter == sessions.end())
	{
		return false;
	}
	else
	{
		return iter->OnFrame(header, userdata);
	}
}

bool IOHandler::IsRouteInUse(const Route& route) const
{
	auto matches = [route](const Session & record)
	{
		return record.Matches(route);
	};

	return std::find_if(sessions.begin(), sessions.end(), matches) != sessions.end();
}

bool IOHandler::IsSessionInUse(const std::shared_ptr<opendnp3::ILinkSession>& session) const
{
	auto matches = [&](const Session & record)
	{
		return record.Matches(session);
	};

	return std::find_if(sessions.begin(), sessions.end(), matches) != sessions.end();
}

bool IOHandler::IsAnySessionEnabled() const
{
	auto matches = [&](const Session & record)
	{
		return record.enabled;
	};

	return std::find_if(sessions.begin(), sessions.end(), matches) != sessions.end();
}

void IOHandler::Reset()
{
	if (this->channel)
	{
		// shutdown the existing channel and drop the reference to it
		this->channel->Shutdown();
		this->channel.reset();

		this->UpdateListener(ChannelState::CLOSED);

		// notify any sessions that are online that this layer is offline
		for (auto& item : this->sessions)
		{
			item.LowerLayerDown();
		}
	}

	this->taskLock.SetOffline();

	// reset the state of the parser
	this->parser.Reset();

	// clear any pending tranmissions
	this->txQueue.clear();
}

}



