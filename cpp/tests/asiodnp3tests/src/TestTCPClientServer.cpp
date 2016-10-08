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
#include <catch.hpp>

#include "asiopal/TCPServer.h"
#include "asiopal/TCPClient.h"
#include "asiopal/SocketChannel.h"

#include "testlib/MockLogHandler.h"

using namespace openpal;
using namespace asiopal;

class MockIO final : public IO, public std::enable_shared_from_this<MockIO>
{

public:

	static std::shared_ptr<MockIO> Create()
	{
		return std::make_shared<MockIO>();
	}	

	std::shared_ptr<StrandExecutor> Executor()
	{
		return StrandExecutor::Create(this->shared_from_this());
	}
	
	size_t RunUntil(const std::function<bool ()>& condition, std::chrono::steady_clock::duration timeout = std::chrono::seconds(1))
	{		
		size_t iterations = 0;
		const auto start = std::chrono::steady_clock::now();

		while (!condition())
		{
			++iterations;
			std::error_code ec;
			this->service.poll_one(ec);
			if (ec) throw std::logic_error(ec.message());
			this->service.reset();

			const auto now = std::chrono::steady_clock::now();
			if ((now - start) > timeout) {
				throw std::logic_error("timeout while waiting for condition");
			}
		}
	
		return iterations;
	}
};

class MockServerHandler final : public ITCPServerHandler
{

public:

	virtual void AcceptConnection(uint64_t sessionid, const std::shared_ptr<StrandExecutor>& executor, asio::ip::tcp::socket socket) override
	{
		++this->num_accept;

		if (this->channel)
		{
			this->channel->Shutdown();
		}

		this->channel = SocketChannel::Create(executor, std::move(socket));
	}

	size_t num_accept = 0;	
	std::shared_ptr<IAsyncChannel> channel;
};



#define SUITE(name) "TCPClientServerSuite - " name

TEST_CASE(SUITE("TestStateClosed"))
{
	testlib::MockLogHandler log;

	auto io = MockIO::Create();	
	auto shandler = std::make_shared<MockServerHandler>();
	
	std::error_code ec;
	auto server = TCPServer::Create(io->Executor(), shandler, log.root.Clone("server"), IPEndpoint::AllAdapters(20000), ec);
	REQUIRE_FALSE(ec); // now bound and listening
	
	auto client = TCPClient::Create(io->Executor(), "127.0.0.1", "0.0.0.0", 20000);

	uint32_t clientConnect = 0;

	auto callback = [&](asio::ip::tcp::socket socket, const std::error_code& ec)
	{
		++clientConnect;
	};

	REQUIRE(client->BeginConnect(callback));

	auto condition = [&]() -> bool
	{
		return (shandler->num_accept == 1) && (clientConnect == 1);
	};

	REQUIRE(io->RunUntil(condition) == 2);
}







