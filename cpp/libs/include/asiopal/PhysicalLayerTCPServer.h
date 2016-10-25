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
#ifndef ASIOPAL_PHYSICAL_LAYER_TCP_SERVER_H
#define ASIOPAL_PHYSICAL_LAYER_TCP_SERVER_H

#include "PhysicalLayerBaseTCP.h"

#include <asio.hpp>
#include <asio/ip/tcp.hpp>

namespace asiopal
{

/**
* Implementation of a TCP server
*/
class PhysicalLayerTCPServer final : public PhysicalLayerBaseTCP
{
public:
	PhysicalLayerTCPServer(
	    openpal::Logger logger,
	    asio::io_service& service,
	    const std::string& endpoint,
	    uint16_t port,
	std::function<void (asio::ip::tcp::socket&)> configure = [](asio::ip::tcp::socket&) {}
	);

	// --- Implement the remainging actions ---
	void DoOpen() override;
	void DoOpeningClose() override; //override this to cancel the acceptor instead of the socket
	void DoOpenSuccess() override;
	void DoOpenCallback() override;

private:

	void CloseAcceptor();

	std::string localEndpointString;
	asio::ip::tcp::endpoint localEndpoint;
	asio::ip::tcp::endpoint remoteEndpoint;
	asio::ip::tcp::acceptor acceptor;
	std::function<void (asio::ip::tcp::socket&)> configure;
};
}

#endif
