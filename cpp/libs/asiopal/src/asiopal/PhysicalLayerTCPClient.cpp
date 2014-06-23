/**
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
#include "PhysicalLayerTCPClient.h"

#include <asio.hpp>

#include <functional>
#include <string>

#include <openpal/channel/IPhysicalLayerCallbacks.h>
#include <openpal/logging/LogLevels.h>

using namespace asio;
using namespace std;
using namespace openpal;

namespace asiopal
{

PhysicalLayerTCPClient::PhysicalLayerTCPClient(
	openpal::LogRoot& root,
	asio::io_service& service,
    const std::string& host_,
    uint16_t port,
    std::function<void (asio::ip::tcp::socket&)> aConfigure) :

	PhysicalLayerBaseTCP(root, service),
	condition(logger),
	host(host_),
	remoteEndpoint(ip::tcp::v4(), port),
	resolver(service),
	configure(aConfigure)
{

}

/* Implement the actions */
void PhysicalLayerTCPClient::DoOpen()
{
	std::error_code ec;
	auto address = asio::ip::address::from_string(host, ec);
	if (ec)
	{
		auto resolveCallback = [this](const std::error_code & code, ip::tcp::resolver::iterator endpoints)
		{
			this->HandleResolve(code, endpoints);
		};
		ip::tcp::resolver::query query(host, "20000");
		resolver.async_resolve(query, strand.wrap(resolveCallback));		                           		                       
	}
	else
	{
		remoteEndpoint.address(address);
		auto openCallback = [this](const std::error_code & code) { this->OnOpenCallback(code); };
		socket.async_connect(remoteEndpoint, strand.wrap(openCallback));
	}
}

void PhysicalLayerTCPClient::HandleResolve(const std::error_code& code, asio::ip::tcp::resolver::iterator endpoints)
{
	if (code)
	{
		this->OnOpenCallback(code);
	}
	else
	{
		// attempt a connection to each endpoint in the iterator until we connect
		auto callback = [this](const std::error_code & code, ip::tcp::resolver::iterator endpoints)
		{
			this->OnOpenCallback(code);
		};

		asio::async_connect(socket, endpoints, condition, strand.wrap(callback));
	}
}

void PhysicalLayerTCPClient::DoOpeningClose()
{
	this->CloseSocket();
}

void PhysicalLayerTCPClient::DoOpenSuccess()
{
	SIMPLE_LOG_BLOCK(logger, logflags::INFO, "Connected to host");
	configure(socket);
}

}


