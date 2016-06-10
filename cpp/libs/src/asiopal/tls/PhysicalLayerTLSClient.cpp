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

#include "asiopal/tls/PhysicalLayerTLSClient.h"

#include "asiopal/SocketHelpers.h"

using namespace asio;
using namespace asiopal;

namespace asiopal
{

PhysicalLayerTLSClient::PhysicalLayerTLSClient(
    openpal::Logger logger,
    asio::io_service& service,
    const std::string& host_,
    const std::string& localAddress_,
    uint16_t port,
    const TLSConfig& config,
    std::error_code& ec
) :
	PhysicalLayerTLSBase(logger, service, config, false, ec),
	condition(logger),
	host(host_),
	localAddress(localAddress_),
	remoteEndpoint(ip::tcp::v4(), port),
	localEndpoint(),
	resolver(service)
{


}

void PhysicalLayerTLSClient::DoOpen()
{
	std::error_code ec;
	SocketHelpers::BindToLocalAddress(localAddress, localEndpoint, this->stream.lowest_layer(), ec);
	if (ec)
	{
		auto callback = [this, ec]()
		{
			this->OnOpenCallback(ec);
		};
		executor.strand.post(callback);
		return;
	}

	auto address = asio::ip::address::from_string(host, ec);
	if (ec)
	{
		auto callback = [this](const std::error_code & ec, ip::tcp::resolver::iterator endpoints)
		{
			this->HandleResolveResult(ec, endpoints);
		};
		ip::tcp::resolver::query query(host, "20000");
		resolver.async_resolve(query, executor.strand.wrap(callback));
	}
	else
	{
		remoteEndpoint.address(address);
		auto callback = [this](const std::error_code & ec)
		{
			this->HandleConnectResult(ec);
		};

		stream.lowest_layer().async_connect(remoteEndpoint, executor.strand.wrap(callback));
	}

}

void PhysicalLayerTLSClient::DoOpeningClose()
{
	this->ShutdownSocket();
	this->CloseSocket();
}

void PhysicalLayerTLSClient::DoOpenSuccess()
{
	SIMPLE_LOG_BLOCK(logger, openpal::logflags::INFO, "Connected to host");
}

void PhysicalLayerTLSClient::HandleResolveResult(const std::error_code& ec, asio::ip::tcp::resolver::iterator endpoints)
{
	if (ec)
	{
		this->OnOpenCallback(ec);
	}
	else
	{
		// attempt a connection to each endpoint in the iterator until we connect
		auto callback = [this](const std::error_code & code, ip::tcp::resolver::iterator endpoints)
		{
			this->HandleConnectResult(code);
		};

		asio::async_connect(stream.lowest_layer(), endpoints, condition, executor.strand.wrap(callback));
	}
}

void PhysicalLayerTLSClient::HandleConnectResult(const std::error_code& ec)
{
	if (ec)
	{
		this->OnOpenCallback(ec);
	}
	else
	{
		auto callback = [this](const std::error_code & code)
		{
			this->OnOpenCallback(code);
		};

		this->stream.async_handshake(asio::ssl::stream_base::client, executor.strand.wrap(callback));
	}
}

}


