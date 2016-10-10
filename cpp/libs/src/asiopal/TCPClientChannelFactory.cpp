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

#include "asiopal/TCPClientChannelFactory.h"

#include "asiopal/SocketChannel.h"

namespace asiopal
{

TCPClientChannelFactory::TCPClientChannelFactory(
    const openpal::Logger& logger,
    const std::shared_ptr<StrandExecutor>& executor,
    const ChannelRetry& retry,
    const IPEndpoint& remote,
    const std::string& adapter
) :
	logger(logger),
	executor(executor),
	retry(retry),
	remote(remote),
	adapter(adapter),
	retrytimer(*executor.get())
{}


void TCPClientChannelFactory::Shutdown()
{
	this->Reset();
}

void TCPClientChannelFactory::BeginChannelAccept(const channel_callback_t& callback)
{
	// this call needs to be idempotent
	if (!this->is_connecting)
	{
		if (!this->client)
		{
			this->client = TCPClient::Create(executor, remote, adapter);
		}

		this->StartConnect(this->client, this->retry.minOpenRetry, callback);
	}
}

void TCPClientChannelFactory::SuspendChannelAccept()
{
	this->Reset();
}

void TCPClientChannelFactory::OnChannelShutdown(const channel_callback_t& callback)
{
	this->BeginChannelAccept(callback);
}

void TCPClientChannelFactory::StartConnect(const std::shared_ptr<TCPClient>& client, const openpal::TimeDuration& delay, const channel_callback_t& callback)
{
	this->is_connecting = true;

	auto self(shared_from_this());
	auto cb = [self, delay, client, callback](const std::shared_ptr<StrandExecutor>& executor, asio::ip::tcp::socket socket, const std::error_code & ec) -> void
	{

		if (ec)
		{
			const auto newDelay = self->retry.strategy.GetNextDelay(delay, self->retry.maxOpenRetry);

			auto cb = [self, newDelay, client, callback]()
			{
				self->StartConnect(client, newDelay, callback);
			};

			self->retrytimer.Start(delay, cb);
		}
		else
		{
			self->is_connecting = false;
			callback(SocketChannel::Create(executor, std::move(socket)));
		}
	};

	this->client->BeginConnect(cb);
}

void TCPClientChannelFactory::Reset()
{
	if (this->client)
	{
		this->client->Cancel();
		this->client.reset();
	}

	this->is_connecting = false;

	retrytimer.Cancel();
}

}


