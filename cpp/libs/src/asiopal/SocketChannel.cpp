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

#include "asiopal/SocketChannel.h"

namespace asiopal
{
std::unique_ptr<IAsyncChannel> SocketChannel::Create(asio::ip::tcp::socket socket)
{
	return std::unique_ptr<IAsyncChannel>(new SocketChannel(std::move(socket)));
}

SocketChannel::SocketChannel(asio::ip::tcp::socket socket) : m_socket(std::move(socket))
{

}

void SocketChannel::BeginRead(openpal::WSlice& dest, const ReadCallbackT& callback)
{
	m_socket.async_read_some(asio::buffer(dest, dest.Size()), callback);
}

void SocketChannel::BeginWrite(const openpal::RSlice& buffer, const WriteCallbackT& callback)
{
	asio::async_write(m_socket, asio::buffer(buffer, buffer.Size()), callback);
}

void SocketChannel::BeginShutdown(const ShutdownCallbackT& callback)
{
	m_socket.shutdown(asio::socket_base::shutdown_type::shutdown_both);
	m_socket.close();
	callback();
}

}
