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

#include "openpal/logging/Logger.h"
#include "openpal/executor/TimerRef.h"

namespace asiopal
{

class TCPClientChannelFactory final : public IChannelFactory, public std::enable_shared_from_this<TCPClientChannelFactory>
{

public:

	std::shared_ptr<TCPClientChannelFactory> Create(
		const openpal::Logger& logger,
		const std::shared_ptr<StrandExecutor>& executor,
		const ChannelRetry& retry,
		const IPEndpoint& remote,
		const std::string& adapter) 
	{
		return std::make_shared<TCPClientChannelFactory>(logger, executor, retry, remote, adapter);
	}

	TCPClientChannelFactory(
		const openpal::Logger& logger,
		const std::shared_ptr<StrandExecutor>& executor,
		const ChannelRetry& retry,
		const IPEndpoint& remote,
		const std::string& adapter
	);

	virtual void Shutdown() override;	
	virtual void BeginChannelAccept(const channel_callback_t& callback) override;
	virtual void SuspendChannelAccept() override;	
	virtual void OnChannelShutdown(const channel_callback_t& callback) override;

private:

	void StartConnect(const std::shared_ptr<TCPClient>& client, const openpal::TimeDuration& delay, const channel_callback_t& callback);

	void Reset();

	bool is_connecting = false;

	openpal::Logger logger;

	const std::shared_ptr<StrandExecutor> executor;
	const ChannelRetry retry;
	const IPEndpoint remote;
	const std::string adapter;

	// current value of the client
	std::shared_ptr<TCPClient> client;

	// connection retry timer
	openpal::TimerRef retrytimer;
};

}

#endif
