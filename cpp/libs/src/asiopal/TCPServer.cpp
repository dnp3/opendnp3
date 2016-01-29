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

#include "asiopal/TCPServer.h"

using namespace asio;
using namespace asio::ip;

namespace asiopal
{

	TCPServer::TCPServer(io_service& ioservice, IPEndpoint endpoint, AcceptCallback callback, std::error_code& ec) :
		m_endpoint(ip::tcp::v4(), endpoint.port),
		m_callback(callback),
		m_acceptor(ioservice),
		m_socket(ioservice)
	{
		this->Configure(endpoint.address, ec);

		if (!ec)
		{
			this->StartAccept();
		}
	}

	std::shared_ptr<TCPServer> TCPServer::Create(asio::io_service& ioservice, IPEndpoint endpoint, AcceptCallback callback, std::error_code& ec)
	{
		return std::shared_ptr<TCPServer>(new TCPServer(ioservice, endpoint, callback, ec));
	}

	void TCPServer::Shutdown()
	{
		m_acceptor.close();
	}

	void TCPServer::Configure(const std::string& adapter, std::error_code& ec)
	{
		auto address = asio::ip::address::from_string(adapter, ec);

		if (ec)
		{
			return;
		}

		m_endpoint.address(address);

		m_acceptor.set_option(ip::tcp::acceptor::reuse_address(true));
		m_acceptor.bind(m_endpoint, ec);

		if (ec)
		{
			return;
		}

		m_acceptor.listen(socket_base::max_connections, ec);		
	}
		
	void TCPServer::StartAccept()
	{
		// this ensures that the TCPListener is never deleted during an active callback
		auto self(shared_from_this());
		auto callback = [self](std::error_code ec) 
		{
			if (!ec)
			{
				// client code responsible for closing
				self->m_callback(std::move(self->m_socket));
			}

			// the acceptor may have been closed
			if (self->m_acceptor.is_open())
			{
				self->StartAccept();
			}
		};


		m_acceptor.async_accept(m_socket, callback);
	}

}


