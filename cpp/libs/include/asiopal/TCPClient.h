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
#ifndef ASIOPAL_TCPCLIENT_H
#define ASIOPAL_TCPCLIENT_H

#include "asiopal/StrandExecutor.h"
#include "asiopal/ITCPClientHandler.h"

namespace asiopal
{

class TCPClient final : public std::enable_shared_from_this<TCPClient>, private openpal::Uncopyable
{

public:

	static std::shared_ptr<TCPClient> Create(
	    std::shared_ptr<StrandExecutor> executor,
	    const std::string& host,
	    const std::string& adapter,
	    uint16_t port)
	{
		return std::make_shared<TCPClient>(executor, host, adapter, port);
	}

	TCPClient(
	    std::shared_ptr<StrandExecutor> executor,
	    const std::string& host,
	    const std::string& adapter,
	    uint16_t port
	);

	bool Cancel();
	
	bool BeginConnect(const std::shared_ptr<ITCPClientHandler>& handler);

private:
	
	bool PostConnectError(const std::shared_ptr<ITCPClientHandler>& handler, const std::error_code& ec);

	bool connecting = false;
	bool canceled = false;

	std::shared_ptr<StrandExecutor> executor;
	asio::ip::tcp::socket socket;
	const std::string host;
	const std::string adapter;
	asio::ip::tcp::endpoint remoteEndpoint;
	asio::ip::tcp::endpoint localEndpoint;
	asio::ip::tcp::resolver resolver;
};

}

#endif
