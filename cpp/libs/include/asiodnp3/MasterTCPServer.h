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
#ifndef ASIOPAL_MASTERTCPSERVER_H
#define ASIOPAL_MASTERTCPSERVER_H

#include <openpal/logging/Logger.h>

#include <asiopal/TCPServer.h>
#include <asiopal/IResourceManager.h>

#include "asiodnp3/IListenCallbacks.h"

namespace asiodnp3
{
/**
* Binds and listens on an IPv4 TCP port
*
* Meant to be used exclusively as a shared_ptr
*/
class MasterTCPServer final : public asiopal::TCPServer
{

public:

	static std::shared_ptr<MasterTCPServer> Create(
	    asiopal::IResourceManager& shutdown,
	    std::shared_ptr<IListenCallbacks> callbacks,
	    std::shared_ptr<asiopal::ThreadPool> pool,
	    openpal::LogRoot root,
	    asiopal::IPEndpoint endpoint,
	    std::error_code& ec
	);

private:

	asiopal::IResourceManager* m_manager;
	std::shared_ptr<IListenCallbacks> m_callbacks;

	static std::string SessionIdToString(uint64_t sessionid);

	MasterTCPServer(
	    asiopal::IResourceManager& shutdown,
	    std::shared_ptr<IListenCallbacks> callbacks,
	    std::shared_ptr<asiopal::ThreadPool> pool,
	    openpal::LogRoot root,
	    asiopal::IPEndpoint endpoint,
	    std::error_code& ec
	);

	virtual void AcceptConnection(uint64_t sessionid, asio::ip::tcp::socket) override;
	virtual void OnShutdown() override;
};

}

#endif
