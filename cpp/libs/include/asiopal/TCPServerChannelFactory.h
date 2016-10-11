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

#ifndef ASIOPAL_TCPSERVERCHANNELFACTORY_H
#define ASIOPAL_TCPSERVERCHANNELFACTORY_H

#include "asiopal/IChannelFactory.h"
#include "asiopal/TCPServer.h"

#include "openpal/logging/Logger.h"


namespace asiopal
{

class TCPServerChannelFactory final : public IChannelFactory, public std::enable_shared_from_this<TCPServerChannelFactory>
{
	class Server final : public TCPServer
	{
		public:

			Server(
				const openpal::Logger& logger,
				const std::shared_ptr<StrandExecutor>& executor,
				const IPEndpoint& endpoint,
				std::error_code& ec				
			);

			void StartAcceptingConnection(const channel_callback_t& callback)
			{
				this->callback = callback;
				this->StartAccept();
			}
				
		private:

			channel_callback_t callback;

			virtual void OnShutdown() override {}

			virtual void AcceptConnection(uint64_t sessionid, const std::shared_ptr<StrandExecutor>& executor, asio::ip::tcp::socket) override;
	};

public:

	std::shared_ptr<TCPServerChannelFactory> Create(
	    const openpal::Logger& logger,
	    const std::shared_ptr<StrandExecutor>& executor,
	    const IPEndpoint& endpoint,
		std::error_code& ec)
	{
		return std::make_shared<TCPServerChannelFactory>(logger, executor, endpoint, ec);
	}

	TCPServerChannelFactory(
	    const openpal::Logger& logger,
	    const std::shared_ptr<StrandExecutor>& executor,
	    const IPEndpoint& endpoint,
		std::error_code& ec
	);


private:

	virtual void Shutdown() override;
	virtual void BeginChannelAccept(const channel_callback_t& callback) override;
	virtual void SuspendChannelAccept() override;
	virtual void OnChannelShutdown(const channel_callback_t& callback) override {} // do nothing as we're always accepting if we're online	

	openpal::Logger logger;
	const std::shared_ptr<StrandExecutor> executor;
	const IPEndpoint endpoint;
	
	std::shared_ptr<Server> server;

};

}

#endif
