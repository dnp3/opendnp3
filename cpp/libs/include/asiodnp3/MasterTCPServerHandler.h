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
#ifndef ASIOPAL_MASTERTCPSERVERHANDLER_H
#define ASIOPAL_MASTERTCPSERVERHANDLER_H

#include <openpal/logging/Logger.h>

#include <asiopal/ITCPServerHandler.h>
#include <asiopal/ResourceManager.h>
#include <asiopal/IPEndpoint.h>

#include "asiodnp3/IListenCallbacks.h"

namespace asiodnp3
{
/**
* Binds and listens on an IPv4 TCP port
*
* Meant to be used exclusively as a shared_ptr
*/
class MasterTCPServerHandler final : public asiopal::ITCPServerHandler
{

public:

	MasterTCPServerHandler(
	    const openpal::Logger& logger,
	    std::shared_ptr<IListenCallbacks> callbacks,
	    const std::shared_ptr<asiopal::ResourceManager>& manager
	);

	static std::shared_ptr<MasterTCPServerHandler> Create(
		const openpal::Logger& logger,
	    std::shared_ptr<IListenCallbacks> callbacks,
		const std::shared_ptr<asiopal::ResourceManager>& manager)
	{
		return std::make_shared<MasterTCPServerHandler>(logger, callbacks, manager);
	}


private:

	openpal::Logger logger;
	std::shared_ptr<IListenCallbacks> callbacks;
	std::shared_ptr<asiopal::ResourceManager> manager;


	static std::string SessionIdToString(uint64_t sessionid);

	virtual void OnShutdown(const std::shared_ptr<asiopal::IResource>& server) override;

	virtual void AcceptConnection(uint64_t sessionid, const std::shared_ptr<asiopal::StrandExecutor>& executor, asio::ip::tcp::socket) override;
};

}

#endif
