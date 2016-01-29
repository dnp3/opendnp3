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
#ifndef ASIOPAL_TCPLISTENER_H
#define ASIOPAL_TCPLISTENER_H

#include <asio.hpp>
#include <utility>
#include <functional>

#include "asiopal/IPEndpoint.h"

#include <openpal/util/Uncopyable.h>

namespace asiopal
{			
	/**	
	* Binds and listens on an IPv4 TCP port
	*
	* Meant to be used exclusively as a shared_ptr
	*/
	class TCPListener : public std::enable_shared_from_this<TCPListener>, private openpal::Uncopyable
	{		

	public:

		/// Callback invoked when a new connection is received
		/// The accepting process is blocked by this callback
		/// Client code is respon
		typedef std::function<void (asio::ip::tcp::socket)> AcceptCallback;

		/// Factory function ensures that only managed instances are created
		static std::shared_ptr<TCPListener> Start(asio::io_service& ioservice, IPEndpoint endpoint, AcceptCallback callback, std::error_code& ec);

		// Stop listening for connections, permanently shutting down the listener
		void Shutdown();
		
	private:		

		TCPListener(asio::io_service& ioservice, IPEndpoint endpoint, AcceptCallback callback, std::error_code& ec);

		void Configure(const std::string& adapter, std::error_code& ec);

		void DoAccept();

		asio::ip::tcp::endpoint m_endpoint;
		AcceptCallback m_callback;
		asio::ip::tcp::acceptor m_acceptor;
		asio::ip::tcp::socket m_socket;		
	};

}

#endif
