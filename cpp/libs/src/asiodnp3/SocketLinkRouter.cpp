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

#include "asiodnp3/SocketLinkRouter.h"

#include <openpal/logging/LogMacros.h>

#include <opendnp3/LogLevels.h>

using namespace openpal;
using namespace opendnp3;

namespace asiodnp3
{

	SocketLinkRouter::SocketLinkRouter(openpal::Logger logger, asio::ip::tcp::socket socket) :
		m_logger(logger),			
		m_parser(logger, &m_stats),
		m_socket(std::move(socket)),
		m_strand(m_socket.get_io_service())
	{
		
	}

	std::shared_ptr<SocketLinkRouter> SocketLinkRouter::Create(openpal::Logger logger, asio::ip::tcp::socket socket)
	{
		return std::shared_ptr<SocketLinkRouter>(new SocketLinkRouter(logger, std::move(socket)));
	}

	void SocketLinkRouter::BeginTransmit(const openpal::RSlice& buffer, opendnp3::ILinkSession& session)
	{		
		auto self(shared_from_this());
		auto callback = [self, buffer, &session](const std::error_code& ec, std::size_t num) {
			if (ec) {
				// we'll let the failed read close the session
				SIMPLE_LOG_BLOCK(self->m_logger, flags::WARN, ec.message().c_str());
				session.OnTransmitResult(false);
			}
			else {
				assert(num <= buffer.Size());
				auto remainder = buffer.Skip(num);

				if (remainder.IsEmpty())
				{
					session.OnTransmitResult(true);
				}
				else
				{
					self->BeginTransmit(remainder, session);
				}
			}
		};

		m_socket.async_write_some(asio::buffer(buffer, buffer.Size()), m_strand.wrap(callback));
	}


	bool SocketLinkRouter::OnFrame(const LinkHeaderFields& header, const openpal::RSlice& userdata)
	{
		// TODO
		return true;
	}

	void SocketLinkRouter::BeginReceive()
	{
		auto self(shared_from_this());
		auto callback = [self](const std::error_code& ec, std::size_t num) {
			if (ec) {
				SIMPLE_LOG_BLOCK(self->m_logger, flags::WARN, ec.message().c_str());
				// TODO - how do we signal the close of the session?
			}
			else {
				self->m_parser.OnRead(num, *self);
				self->BeginReceive();
			}
		};

		auto dest = m_parser.WriteBuff();
		m_socket.async_read_some(asio::buffer(dest, dest.Size()), m_strand.wrap(callback));
	}
}