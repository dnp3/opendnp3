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

#include "asiopal/tls/TLSStreamChannel.h"

namespace asiopal
{

TLSStreamChannel::TLSStreamChannel(std::shared_ptr<asio::ssl::stream<asio::ip::tcp::socket>> stream) : m_stream(stream)
{}

std::unique_ptr<IAsyncChannel> TLSStreamChannel::Create(std::shared_ptr<asio::ssl::stream<asio::ip::tcp::socket>> stream)
{
	return std::unique_ptr<IAsyncChannel>(new TLSStreamChannel(stream));
}

void TLSStreamChannel::BeginRead(openpal::WSlice& dest, const ReadCallbackT& callback)
{
	m_stream->async_read_some(asio::buffer(dest, dest.Size()), callback);
}

void TLSStreamChannel::BeginWrite(const openpal::RSlice& data, const WriteCallbackT& callback)
{
	asio::async_write(*m_stream, asio::buffer(data, data.Size()), callback);
}

void TLSStreamChannel::BeginShutdown(const ShutdownCallbackT& callback)
{
	// TODO - should we perform an async shutdown on the TLS stream?
	std::error_code ec;
	m_stream->lowest_layer().shutdown(asio::socket_base::shutdown_both, ec);
	m_stream->lowest_layer().close(ec);
	callback();
}

}


