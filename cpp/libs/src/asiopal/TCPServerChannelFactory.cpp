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

#include "asiopal/TCPServerChannelFactory.h"

#include "asiopal/SocketChannel.h"

#include "openpal/logging/LogMacros.h"

#include "opendnp3/LogLevels.h"

namespace asiopal
{

TCPServerChannelFactory::Server::Server(
    const openpal::Logger& logger,
    const std::shared_ptr<StrandExecutor>& executor,
    const IPEndpoint& endpoint,
    std::error_code& ec
) :
	TCPServer(logger, executor, endpoint, ec)
{}

void TCPServerChannelFactory::Server::AcceptConnection(uint64_t sessionid, const std::shared_ptr<StrandExecutor>& executor, asio::ip::tcp::socket socket)
{
	this->callback(SocketChannel::Create(executor, std::move(socket)));
}

TCPServerChannelFactory::TCPServerChannelFactory(
    const openpal::Logger& logger,
    const std::shared_ptr<StrandExecutor>& executor,
    const IPEndpoint& endpoint,
    std::error_code& ec
) : logger(logger),
	executor(executor),
	endpoint(endpoint),
	server(std::make_shared<Server>(logger, executor, endpoint, ec))
{

}

void TCPServerChannelFactory::Shutdown()
{
	if (this->server)
	{
		this->server->Shutdown();
		this->server.reset();
	}
}

void TCPServerChannelFactory::BeginChannelAccept(const channel_callback_t& callback)
{
	if (server)
	{
		server->StartAcceptingConnection(callback);
	}
	else
	{
		std::error_code ec;
		auto server = std::make_shared<Server>(this->logger, this->executor, this->endpoint, ec);

		if (ec)
		{
			SIMPLE_LOG_BLOCK(this->logger, opendnp3::flags::WARN, ec.message().c_str());

			// TODO - should we retry?
		}
		else
		{
			server->StartAcceptingConnection(callback);
		}
	}
}

void TCPServerChannelFactory::SuspendChannelAccept()
{
	if (this->server)
	{
		this->server->Shutdown();
		this->server.reset();
	}
}

}


