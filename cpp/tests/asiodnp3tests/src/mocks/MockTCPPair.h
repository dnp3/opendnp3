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

#ifndef ASIOPAL_MOCKTCPPAIR_H
#define ASIOPAL_MOCKTCPPAIR_H

#include "MockIO.h"
#include "MockTCPClientHandler.h"
#include "MockTCPServerHandler.h"

#include "asiopal/TCPClient.h"
#include "asiopal/TCPServer.h"

#include "testlib/MockLogHandler.h"

namespace asiopal
{

class MockTCPPair
{

public:

	MockTCPPair(std::shared_ptr<MockIO> io, uint16_t port) :
		ec(),
		io(io),
		log(),
		chandler(std::make_shared<MockTCPClientHandler>()),
		shandler(std::make_shared<MockTCPServerHandler>()),
		client(TCPClient::Create(io->Executor(), IPEndpoint::Localhost(port), "127.0.0.1")),
		server(TCPServer::Create(io->Executor(), this->shandler, this->log.root.Clone("server"), IPEndpoint::Localhost(20000), ec))
	{
		if (ec)
		{
			throw std::logic_error(ec.message());
		}
	}

	~MockTCPPair()
	{
		this->server->BeginShutdown();
		this->client->Cancel();

		// drop all of our shared_ptrs
		this->server.reset();
		this->client.reset();
		this->chandler.reset();
		this->shandler.reset();

		this->io->RunUntilOutOfWork();
	}

	void Connect(size_t num = 1)
	{
		if (!this->client->BeginConnect(this->chandler))
		{
			throw std::logic_error("BeginConnect returned false");
		}

		auto connected = [this, num]() -> bool
		{
			return this->NumConnectionsEqual(num);
		};

		io->CompleteInXIterations(2, connected);
	}

	bool NumConnectionsEqual(size_t num) const
	{
		return (this->shandler->channels.size() == num) && (this->chandler->channels.size() == num);
	}

private:

	std::error_code ec;

	std::shared_ptr<MockIO> io;
	testlib::MockLogHandler log;
	std::shared_ptr<MockTCPClientHandler> chandler;
	std::shared_ptr<MockTCPServerHandler> shandler;
	std::shared_ptr<TCPClient> client;
	std::shared_ptr<TCPServer> server;
};

}

#endif







