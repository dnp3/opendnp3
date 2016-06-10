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
#ifndef ASIOPAL_TCPSERVER_H
#define ASIOPAL_TCPSERVER_H

#include "asiopal/ThreadPool.h"
#include "asiopal/IPEndpoint.h"
#include "asiopal/IListener.h"

#include <openpal/util/Uncopyable.h>
#include <openpal/logging/LogRoot.h>

#include <memory>

namespace asiopal
{
/**
* Binds and listens on an IPv4 TCP port
*
* Meant to be used exclusively as a shared_ptr
*/
class TCPServer :
	public std::enable_shared_from_this<TCPServer>,
	public IListener,
	private openpal::Uncopyable
{

public:

	/// Stop listening for connections, permanently shutting down the listener
	void BeginShutdown() override final;

protected:

	TCPServer(
	    std::shared_ptr<ThreadPool> pool,
	    openpal::LogRoot root,
	    IPEndpoint endpoint,
	    std::error_code& ec
	);

	void StartAccept();

	/// inherited flass defines what to do with
	virtual void AcceptConnection(uint64_t sessionid, asio::ip::tcp::socket) = 0;

	/// Inherited class defines what happens when the server shuts down
	virtual void OnShutdown() = 0;

private:
	void Configure(const std::string& adapter, std::error_code& ec);

protected:

	std::shared_ptr<ThreadPool> m_pool;
	openpal::LogRoot m_root;

private:

	asio::ip::tcp::endpoint m_endpoint;
	asio::ip::tcp::acceptor m_acceptor;
	asio::ip::tcp::socket m_socket;
	uint64_t m_session_id;
};

}

#endif
