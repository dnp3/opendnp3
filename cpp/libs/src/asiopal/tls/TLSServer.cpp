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

#include "asiopal/tls/TLSServer.h"

#include <openpal/logging/LogMacros.h>
#include <opendnp3/LogLevels.h>

#include <sstream>

using namespace asio;
using namespace asio::ip;
using namespace opendnp3;

namespace asiopal
{

TLSServer::TLSServer(
    std::shared_ptr<ThreadPool> pool,
    openpal::LogRoot root,
    IPEndpoint endpoint,
    const TLSConfig& config,
    std::error_code& ec
) :
	m_pool(pool),
	m_root(std::move(root)),
	m_ctx(m_root.logger, true, config, ec),
	m_endpoint(ip::tcp::v4(), endpoint.port),
	m_acceptor(pool->GetIOService()),
	m_session_id(0)
{
	if (!ec)
	{
		this->ConfigureListener(endpoint.address, ec);
	}
}

void TLSServer::BeginShutdown()
{
	m_acceptor.close();
}

std::error_code TLSServer::ConfigureListener(const std::string& adapter, std::error_code& ec)
{
	auto address = asio::ip::address::from_string(adapter, ec);

	if (ec)
	{
		return ec;
	}

	m_endpoint.address(address);

	if (m_acceptor.open(m_endpoint.protocol(), ec)) return ec;
	if (m_acceptor.set_option(ip::tcp::acceptor::reuse_address(true), ec)) return ec;
	if (m_acceptor.bind(m_endpoint, ec)) return ec;

	if (m_acceptor.listen(socket_base::max_connections, ec)) return ec;


	std::ostringstream oss;
	oss << m_endpoint;
	FORMAT_LOG_BLOCK(m_root.logger, flags::INFO, "Listening on: %s", oss.str().c_str());
	return ec;
}

void TLSServer::StartAccept(std::error_code& ec)
{
	const auto ID = this->m_session_id;
	++this->m_session_id;

	// this ensures that the TCPListener is never deleted during an active callback
	auto self(shared_from_this());

	// this could be a unique_ptr once move semantics are supported in lambdas
	auto stream = std::make_shared<asio::ssl::stream<asio::ip::tcp::socket>>(m_pool->GetIOService(), self->m_ctx.value);

	if (!ec)
	{

		auto verify = [this, ID](bool preverified, asio::ssl::verify_context & ctx)
		{
			return this->VerifyCallback(ID, preverified, ctx);
		};

		stream->set_verify_callback(verify, ec);
	}

	auto accept_cb = [self, stream, ID](std::error_code ec) -> void
	{
		if (ec)
		{
			SIMPLE_LOG_BLOCK(self->m_root.logger, flags::INFO, ec.message().c_str());
			self->OnShutdown();
			return;
		}

		// begin accepting another session
		self->StartAccept(ec);

		if (!self->AcceptConnection(ID, stream->lowest_layer().remote_endpoint()))
		{
			std::ostringstream oss;
			oss << stream->lowest_layer().remote_endpoint();

			FORMAT_LOG_BLOCK(self->m_root.logger, flags::INFO, "Remote endpoint rejected: %s", oss.str().c_str());

			stream->lowest_layer().close();
			return;
		}

		// at this point

		auto handshake_cb = [stream, ID, self](const std::error_code & ec)
		{
			if (ec)
			{
				FORMAT_LOG_BLOCK(self->m_root.logger, flags::INFO, "TLS handshake failed: %s", ec.message().c_str());
				return;
			}

			self->AcceptStream(ID, stream);
		};

		// Begin the TLS handshake
		stream->async_handshake(asio::ssl::stream_base::server, handshake_cb);
	};

	if (!ec)
	{
		m_acceptor.async_accept(stream->lowest_layer(), accept_cb);
	}
}
}


