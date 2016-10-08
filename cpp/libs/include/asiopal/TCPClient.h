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
#ifndef ASIOPAL_TCPCLIENT_H
#define ASIOPAL_TCPCLIENT_H

#include "asiopal/StrandExecutor.h"
#include "asiopal/SocketHelpers.h"

namespace asiopal
{

class TCPClient final : private std::enable_shared_from_this<TCPClient>
{

public:

	static std::shared_ptr<TCPClient> Create(
		std::shared_ptr<StrandExecutor> executor,
		const std::string& host,
		const std::string& adapter,
		uint16_t port)
	{
		return std::make_shared<TCPClient>(executor, host, adapter, port);
	}

	TCPClient(
		std::shared_ptr<StrandExecutor> executor,
		const std::string& host,
		const std::string& adapter,
		uint16_t port
	);

	bool Cancel();

	template <class Callback>
	bool BeginConnect(const Callback& callback);

private:

	template <class Callback>
	bool PostConnectError(const Callback& callback, const std::error_code& ec);
	
	bool connecting = false;
	bool canceled = false;

	std::shared_ptr<StrandExecutor> executor;
	asio::ip::tcp::socket socket;
	const std::string host;
	const std::string adapter;
	asio::ip::tcp::endpoint remoteEndpoint;
	asio::ip::tcp::endpoint localEndpoint;
	asio::ip::tcp::resolver resolver;
};

template <class Callback>
bool TCPClient::BeginConnect(const Callback& callback)
{
	if (connecting || canceled) return false;

	this->connecting = true;	

	std::error_code ec;
	SocketHelpers::BindToLocalAddress(this->adapter, this->localEndpoint, this->socket, ec);
	
	if (ec) {		
		return this->PostConnectError(callback, ec);
	}

	const auto address = asio::ip::address::from_string(this->host, ec);
	if (ec)
	{
		return this->PostConnectError(callback, ec);
		// TODO handle DNS resolution
	}
	else
	{
		remoteEndpoint.address(address);
		auto self = this->shared_from_this();
		auto cb = [self, callback](const std::error_code & ec)
		{
			self->connecting = false;
			if (!self->canceled) {
				callback(std::move(self->socket), ec);
			}
		};

		socket.async_connect(remoteEndpoint, executor.strand.wrap(callback));
	}
}

template <class Callback>
bool TCPClient::PostConnectError(const Callback& callback, const std::error_code& ec)
{
	auto self = this->shared_from_this();
	auto cb = [self, ec, callback]()
	{
		self->connecting = false;
		if (!self->canceled) {
			callback(std::move(self->socket), ec);
		}
	};
	executor->strand.post(cb);
	return true;
}

}

#endif
