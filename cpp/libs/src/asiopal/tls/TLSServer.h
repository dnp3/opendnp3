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
#ifndef ASIOPAL_TLSSERVER_H
#define ASIOPAL_TLSSERVER_H

#include "asiopal/IPEndpoint.h"
#include "asiopal/IListener.h"
#include "asiopal/TLSConfig.h"

#include <openpal/util/Uncopyable.h>
#include <openpal/logging/LogRoot.h>

#include "asiopal/tls/SSLContext.h"
#include "asiopal/tls/ITLSServerHandler.h"

namespace asiopal
{
/**
* Binds and listens on an IPv4 TCP port. Implement TLS negotiation.
*
* Meant to be used exclusively as a shared_ptr
*/
class TLSServer final :
	public IListener,
	private std::enable_shared_from_this<TLSServer>,
	private openpal::Uncopyable
{

public:

	TLSServer(
	    std::shared_ptr<StrandExecutor> executor,
	    std::shared_ptr<ITLSServerHandler> handler,
	    IResourceManager& manager,
	    openpal::LogRoot root,
	    IPEndpoint endpoint,
	    const TLSConfig& tlsConfig,
	    std::error_code& ec
	);

	static std::shared_ptr<TLSServer> Create(
	    std::shared_ptr<StrandExecutor> executor,
	    std::shared_ptr<ITLSServerHandler> handler,
	    IResourceManager& manager,
	    openpal::LogRoot root,
	    IPEndpoint endpoint,
	    const TLSConfig& tlsConfig,
	    std::error_code& ec)
	{
		auto ret = std::make_shared<TLSServer>(executor, handler, manager, std::move(root), endpoint, tlsConfig, ec);
		if (ec) return nullptr;
		else
		{
			ret->StartAccept(ec);
			if (ec) return nullptr;
			return ret;
		}
	}

	/// Stop listening for connections, permanently shutting down the listener
	void BeginShutdown() override;

private:

	void OnShutdown();

	void StartAccept(std::error_code& ec);

	std::error_code ConfigureContext(const TLSConfig& config, std::error_code& ec);
	std::error_code ConfigureListener(const std::string& adapter, std::error_code& ec);

	std::shared_ptr<StrandExecutor> executor;
	std::shared_ptr<ITLSServerHandler> handler;
	IResourceManager& manager;
	openpal::LogRoot root;

	SSLContext ctx;
	asio::ip::tcp::endpoint endpoint;
	asio::ip::tcp::acceptor acceptor;

	uint64_t session_id;
};

}

#endif
