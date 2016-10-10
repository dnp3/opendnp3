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
    Logger logger,
    std::shared_ptr<asiopal::IO> io,
    std::shared_ptr<IChannelListener> listener
) :
	logger(logger),
	io(io),
	listener(listener),	
	parser(logger, &statistics)
{

}

void IOHandler::BeginTransmit(const RSlice& buffer, ILinkSession& context)
{

}

bool IOHandler::AddContext(ILinkSession& session, const Route& route)
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

	records.push_back(Session(session, route)); // record is always disabled by default
	return true;
}

bool IOHandler::Enable(ILinkSession& session)
{
	auto matches = [&](const Session & rec)
	{
		return rec.session == &session;
	};

	const auto iter = std::find_if(records.begin(), records.end(), matches);

	if (iter == records.end()) return false;

	if (iter->enabled) return true; // already enabled

	iter->enabled = true;

	if (this->channel) iter->session->OnLowerLayerUp();

	this->Enable();

	return true;
}

bool IOHandler::Disable(ILinkSession& session)
{
	auto matches = [&](const Session & rec)
	{
		return rec.session == &session;
	};

	const auto iter = std::find_if(records.begin(), records.end(), matches);

	if (iter == records.end()) return false;

	if (!iter->enabled) return true; // already disabled

	iter->enabled = false;

	if (channel)
	{
		iter->session->OnLowerLayerDown();
	}

	if (!this->IsAnySessionEnabled()) this->Suspend();

	return true;
}

bool IOHandler::Remove(ILinkSession& session)
{
	auto matches = [&](const Session & rec)
	{
		return rec.session == &session;
	};

	const auto iter = std::find_if(records.begin(), records.end(), matches);

	if (iter == records.end()) return false;

	iter->session->OnLowerLayerDown();

	records.erase(iter);

	if (!this->IsAnySessionEnabled()) this->Suspend();

	return true;
}

void IOHandler::OnNewChannel(std::shared_ptr<asiopal::IAsyncChannel> channel)
{

}

bool IOHandler::IsRouteInUse(const Route& route) const
{
	auto matches = [route](const Session & record)
	{
		return record.route.Equals(route);
	};

	return std::find_if(records.begin(), records.end(), matches) != records.end();
}

bool IOHandler::IsSessionInUse(opendnp3::ILinkSession& session) const
{
	auto matches = [&](const Session & record)
	{
		return (record.session == &session);
	};

	return std::find_if(records.begin(), records.end(), matches) != records.end();
}

bool IOHandler::IsAnySessionEnabled() const
{
	auto matches = [&](const Session & record)
	{
		return record.enabled;
	};

	return std::find_if(records.begin(), records.end(), matches) != records.end();
}

}



