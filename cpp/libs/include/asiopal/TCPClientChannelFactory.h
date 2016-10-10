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
#ifndef ASIOPAL_TCPCLIENTCHANNELFACTORY_H
#define ASIOPAL_TCPCLIENTCHANNELFACTORY_H

#include "asiopal/IChannelFactory.h"
#include "asiopal/TCPClient.h"
#include "asiopal/ChannelRetry.h"
#include "asiopal/SocketChannel.h"

#include "openpal/logging/Logger.h"
#include "openpal/executor/TimerRef.h"

namespace asiopal
{

class TCPClientChannelFactory final : public IChannelFactory, private ITCPClientHandler, public std::enable_shared_from_this<TCPClientChannelFactory>
{	

public:

	TCPClientChannelFactory(
		const openpal::Logger& logger,
		const std::shared_ptr<StrandExecutor>& executor,
		const ChannelRetry& retry,
		const IPEndpoint& remote,
		const std::string& adapter
	) :		
		logger(logger),
		executor(executor),
		retry(retry),
		currentDelay(retry.minOpenRetry),
		remote(remote),
		adapter(adapter),
		retrytimer(*executor.get())
	{}


	virtual void Shutdown() override
	{
		this->Reset();
	}

	// Begin the process of acquiring channels
	virtual void BeginChannelAccept(const channel_callback_t& callback) override
	{
		this->Reset();

		this->client = TCPClient::Create(executor, remote, adapter);

		this->client->BeginConnect(this->shared_from_this());
	}

	// Stop the process of acquiring channels
	virtual void SuspendChannelAccept() override
	{
		this->Reset();
	}

	// Called when a currently active channel shuts down unexpectedly. May cause the factory to acquire another channel	
	virtual void OnChannelShutdown(const channel_callback_t& callback) override
	{
		this->Reset();
		
		this->client = TCPClient::Create(executor, remote, adapter);

		this->client->BeginConnect(this->shared_from_this());
	}

private:

	virtual void OnConnect(const std::shared_ptr<StrandExecutor>& executor, asio::ip::tcp::socket socket, const std::error_code& ec) override
	{
		if (ec)
		{
			
			const auto delay = retry.strategy.GetNextDelay(this->currentDelay, retry.maxOpenRetry);
			this->Reset();
			this->currentDelay = delay;

			auto callback = [this]() 
			{
				this->client = TCPClient::Create(this->executor, remote, adapter);
				this->client->BeginConnect(this->shared_from_this());
			};

			this->retrytimer.Start(delay, callback);
		}
		else
		{

		}
	}

	void Reset()
	{
		if (this->client)
		{
			this->client->Cancel();
			this->client.reset();			
		}

		retrytimer.Cancel();

		this->currentDelay = retry.minOpenRetry;
	}	

	openpal::Logger logger;

	const std::shared_ptr<StrandExecutor> executor;
	const ChannelRetry retry;
	openpal::TimeDuration currentDelay;
	const IPEndpoint remote;
	const std::string adapter;

	// current value of the client
	std::shared_ptr<TCPClient> client;

	// connection retry timer
	openpal::TimerRef retrytimer;
};

}

#endif
