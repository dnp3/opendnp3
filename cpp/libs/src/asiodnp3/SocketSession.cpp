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

#include "asiodnp3/SocketSession.h"

#include <openpal/logging/LogMacros.h>

#include <opendnp3/LogLevels.h>

using namespace openpal;
using namespace asiopal;
using namespace opendnp3;

namespace asiodnp3
{

	SocketSession::SocketSession(openpal::Logger logger, asiopal::IResourceManager& manager, IListenCallbacks& callbacks, asio::ip::tcp::socket socket) :
		m_logger(logger),			
		m_manager(&manager),
		m_callbacks(&callbacks),
		m_parser(logger, &m_stats),		
		m_executor(StrandExecutor::Create(socket.get_io_service())),		
		m_first_frame_timer(*m_executor),
		m_socket(std::move(socket))		
	{
		
	}

	std::shared_ptr<SocketSession> SocketSession::Create(openpal::Logger logger, asiopal::IResourceManager& manager, IListenCallbacks& callbacks, asio::ip::tcp::socket socket)
	{
		auto ret = std::shared_ptr<SocketSession>(new SocketSession(logger, manager, callbacks, std::move(socket)));
		
		if (manager.Register(ret))
		{			
			ret->Start();
		}
		else
		{			
			ret->BeginShutdown();
		}		

		return ret;
	}	

	void SocketSession::BeginShutdown()
	{
		auto self(shared_from_this());
		auto shutdown = [self]()
		{ 
			self->m_socket.close();
			self->m_first_frame_timer.Cancel();
		};
		m_executor->strand.post(shutdown);
	}

	void SocketSession::BeginTransmit(const openpal::RSlice& buffer, opendnp3::ILinkSession& session)
	{		
		auto self(shared_from_this());
		auto callback = [self, buffer, &session](const std::error_code& ec, std::size_t num) {
			if (ec) {
				// we'll let the failed read close the session
				SIMPLE_LOG_BLOCK(self->m_logger, flags::WARN, ec.message().c_str());
				session.OnTransmitResult(false);
			}
			else {				
				session.OnTransmitResult(true);				
			}
		};

		// this writes all the data	
		asio::async_write(m_socket, asio::buffer(buffer, buffer.Size()), m_executor->strand.wrap(callback));
	}

	bool SocketSession::OnFrame(const LinkHeaderFields& header, const openpal::RSlice& userdata)
	{
		if (m_stack)
		{
			m_stack->OnFrame(header, userdata);
		}
		else
		{			
			// TODO - ask application if we should create a master		
			// Cancel the first frame timer
		}

		return true;
	}

	void SocketSession::Start()
	{
		auto timeout = [this]()
		{
			this->OnFirstFrameTimeout();
		};

		m_first_frame_timer.Start(m_callbacks->GetFirstFrameTimeout(), timeout);

		this->BeginReceive();
	}

	void SocketSession::BeginReceive()
	{
		auto self(shared_from_this());
		auto callback = [self](const std::error_code& ec, std::size_t num) {
			if (ec) {
				SIMPLE_LOG_BLOCK(self->m_logger, flags::WARN, ec.message().c_str());
				// TODO - how do we signal the close of the session?
				self->m_manager->Unregister(self);
			}
			else {
				self->m_parser.OnRead(num, *self);
				self->BeginReceive();
			}
		};

		auto dest = m_parser.WriteBuff();
		m_socket.async_read_some(asio::buffer(dest, dest.Size()), m_executor->strand.wrap(callback));
	}

	void SocketSession::OnFirstFrameTimeout()
	{
		SIMPLE_LOG_BLOCK(m_logger, flags::ERR, "Timed out before receving a frame. Closing socket.");
		this->m_socket.close();
	}
}
