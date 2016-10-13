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

#include "asiopal/tls/TLSClient.h"

#include "asiopal/SocketHelpers.h"

namespace asiopal
{

TLSClient::TLSClient(
    const openpal::Logger& logger,
    const std::shared_ptr<Executor>& executor,
    const IPEndpoint& remote,
    const std::string& adapter,
    const TLSConfig& config,
    std::error_code& ec)
	:
	condition(logger),
	executor(executor),
	host(remote.address),
	adapter(adapter),
	ctx(logger, false, config, ec),
	remoteEndpoint(asio::ip::tcp::v4(), remote.port),
	localEndpoint(),
	resolver(executor->strand.get_io_service())
{

}

bool TLSClient::Cancel()
{
	if (this->canceled)
	{
		return false;
	}

	std::error_code ec;
	resolver.cancel();
	this->canceled = true;
	return true;
}

bool TLSClient::BeginConnect(const connect_callback_t& callback)
{
	if (canceled) return false;

	auto stream = std::make_shared<asio::ssl::stream<asio::ip::tcp::socket>>(this->executor->strand.get_io_service(), this->ctx.value);

	std::error_code ec;
	SocketHelpers::BindToLocalAddress(this->adapter, this->localEndpoint, stream->lowest_layer(), ec);

	if (ec)
	{
		auto cb = [self = shared_from_this(), callback, stream, ec]
		{
			if (!self->canceled)
			{
				callback(self->executor, stream, ec);
			}
		};

		this->executor->strand.post(cb);
		return true;
	}

	const auto address = asio::ip::address::from_string(this->host, ec);
	auto self = this->shared_from_this();
	if (ec)
	{
		// Try DNS resolution instead
		auto cb = [self, callback, stream](const std::error_code & ec, asio::ip::tcp::resolver::iterator endpoints)
		{
			self->HandleResolveResult(callback, stream, endpoints, ec);
		};

		std::stringstream portstr;
		portstr << remoteEndpoint.port();

		resolver.async_resolve(
		    asio::ip::tcp::resolver::query(host, portstr.str()),
		    executor->strand.wrap(cb)
		);

		return true;
	}
	else
	{
		remoteEndpoint.address(address);
		auto cb = [self, stream, callback](const std::error_code & ec)
		{
			self->HandleConnectResult(callback, stream, ec);
		};

		stream->lowest_layer().async_connect(remoteEndpoint, executor->strand.wrap(cb));
		return true;
	}
}

void TLSClient::HandleResolveResult(
    const connect_callback_t& callback,
    const std::shared_ptr<asio::ssl::stream<asio::ip::tcp::socket>>& stream,
    const asio::ip::tcp::resolver::iterator& endpoints,
    const std::error_code& ec)
{
	if (ec)
	{
		if (!this->canceled)
		{
			callback(this->executor, stream, ec);
		}
	}
	else
	{
		// attempt a connection to each endpoint in the iterator until we connect
		auto cb = [self = this->shared_from_this(), callback, stream](const std::error_code & ec, asio::ip::tcp::resolver::iterator endpoints)
		{
			self->HandleConnectResult(callback, stream, ec);
		};

		asio::async_connect(stream->lowest_layer(), endpoints, this->condition, this->executor->strand.wrap(cb));
	}
}

void TLSClient::HandleConnectResult(
    const connect_callback_t& callback,
    const std::shared_ptr<asio::ssl::stream<asio::ip::tcp::socket>>& stream,
    const std::error_code& ec)
{
	if (ec)
	{
		if (!this->canceled)
		{
			callback(this->executor, stream, ec);
		}
	}
	else
	{
		auto cb = [self = shared_from_this(), callback, stream](const std::error_code & ec)
		{
			if (!self->canceled)
			{
				callback(self->executor, stream, ec);
			}
		};

		stream->async_handshake(asio::ssl::stream_base::client, executor->strand.wrap(cb));
	}
}

}


