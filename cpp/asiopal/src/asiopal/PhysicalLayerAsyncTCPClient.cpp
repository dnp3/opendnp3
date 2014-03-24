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
#include "PhysicalLayerAsyncTCPClient.h"

#include <asio.hpp>

#include <functional>
#include <string>

#include <openpal/IHandlerAsync.h>
#include <openpal/LogLevels.h>

using namespace asio;
using namespace std;
using namespace openpal;

namespace asiopal
{

PhysicalLayerAsyncTCPClient::PhysicalLayerAsyncTCPClient(
	const openpal::LogConfig& config,
    asio::io_service* pIOService,
    const std::string& host_,
    uint16_t port,
    std::function<void (asio::ip::tcp::socket&)> aConfigure) :

	PhysicalLayerAsyncBaseTCP(config, pIOService),
	condition(logger),
	host(host_),
	remoteEndpoint(ip::tcp::v4(), port),
	resolver(*pIOService),
	configure(aConfigure)
{
	
}

/* Implement the actions */
void PhysicalLayerAsyncTCPClient::DoOpen()
{
	std::error_code ec;
	auto address = asio::ip::address::from_string(host, ec);
	if (ec)
	{
		ip::tcp::resolver::query query(host, "20000");
		resolver.async_resolve(query, strand.wrap(
			[this](const std::error_code& code, ip::tcp::resolver::iterator endpoints) { 
				this->HandleResolve(code, endpoints);
			}
		));
	}
	else
	{
		remoteEndpoint.address(address);
		mSocket.async_connect(remoteEndpoint, strand.wrap(
			[this](const std::error_code & code) { this->OnOpenCallback(code); }
		));
	}	
}

void PhysicalLayerAsyncTCPClient::HandleResolve(const std::error_code& code, asio::ip::tcp::resolver::iterator endpoints)
{
	if (code)
	{
		this->OnOpenCallback(code);
	}
	else
	{		
		// attempt a connection to each endpoint in the iterator until we connect		
		asio::async_connect(mSocket, endpoints, condition, strand.wrap(
			[this](const std::error_code & code, ip::tcp::resolver::iterator endpoints) { 
				this->OnOpenCallback(code); 
			}
		));
	}
}

void PhysicalLayerAsyncTCPClient::DoOpeningClose()
{
	this->CloseSocket();
}

void PhysicalLayerAsyncTCPClient::DoOpenSuccess()
{
	LOG_BLOCK(log::INFO, "Connected to host");
	configure(mSocket);
}

}


