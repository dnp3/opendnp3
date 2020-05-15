/*
 * Copyright 2013-2020 Automatak, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
 * LLC (www.automatak.com) under one or more contributor license agreements.
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Automatak LLC license
 * this file to you under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License. You may obtain
 * a copy of the License at:
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "channel/tls/TLSServer.h"

#include "logging/LogMacros.h"

#include "opendnp3/logging/LogLevels.h"

#include <sstream>

namespace opendnp3
{

TLSServer::TLSServer(const Logger& logger,
                     const std::shared_ptr<exe4cpp::StrandExecutor>& executor,
                     const IPEndpoint& endpoint,
                     const TLSConfig& config,
                     std::error_code& ec)
    : logger(logger),
      executor(executor),
      ctx(logger, true, config, ec),
      endpoint(asio::ip::tcp::v4(), endpoint.port),
      acceptor(*executor->get_context())
{
    if (!ec)
    {
        this->ConfigureListener(endpoint.address, ec);
    }
}

void TLSServer::Shutdown()
{
    if (this->isShutdown)
        return;

    this->isShutdown = true;
    std::error_code ec;
    this->acceptor.close(ec);

    if (ec)
    {
        SIMPLE_LOG_BLOCK(logger, flags::ERR, ec.message().c_str());
    }
}

std::error_code TLSServer::ConfigureListener(const std::string& adapter, std::error_code& ec)
{
    auto address = asio::ip::address::from_string(adapter, ec);

    if (ec)
    {
        return ec;
    }

    this->endpoint.address(address);

    if (this->acceptor.open(this->endpoint.protocol(), ec))
        return ec;
    if (this->acceptor.set_option(asio::ip::tcp::acceptor::reuse_address(true), ec))
        return ec;
    if (this->acceptor.bind(this->endpoint, ec))
        return ec;

    if (this->acceptor.listen(asio::socket_base::max_connections, ec))
        return ec;

    std::ostringstream oss;
    oss << this->endpoint;
    FORMAT_LOG_BLOCK(this->logger, flags::INFO, "Listening on: %s", oss.str().c_str());
    return ec;
}

void TLSServer::StartAccept()
{
    std::error_code ec;

    const auto ID = this->session_id;
    ++this->session_id;

    // this ensures that the TCPListener is never deleted during an active callback
    auto self(shared_from_this());

    // this could be a unique_ptr once move semantics are supported in lambdas
    auto stream
        = std::make_shared<asio::ssl::stream<asio::ip::tcp::socket>>(*this->executor->get_context(), self->ctx.value);

    auto verify = [this, ID](bool preverified, asio::ssl::verify_context& ctx) {
        return this->VerifyCallback(ID, preverified, ctx);
    };

    stream->set_verify_callback(verify, ec);

    if (ec)
        return;

    auto accept_cb = [self, stream, ID](std::error_code ec) -> void {
        if (ec)
        {
            SIMPLE_LOG_BLOCK(self->logger, flags::INFO, ec.message().c_str());
            self->OnShutdown();
            return;
        }

        // With epoll, even if the acceptor was closed, if a socket was accepted
        // and put in ASIO handler queue, it will survive up to here.
        // So we need to make sure we are still alive before really accepting the connection.
        if (self->isShutdown)
        {
            return;
        }

        // For an unknown reason, the socket may not be properly opened when accepted.
        // We simply ignore it.
        if (!stream->lowest_layer().is_open())
        {
            self->StartAccept();
            return;
        }

        // begin accepting another session
        self->StartAccept();

        if (!self->AcceptConnection(ID, stream->lowest_layer().remote_endpoint()))
        {
            std::ostringstream oss;
            oss << stream->lowest_layer().remote_endpoint();

            FORMAT_LOG_BLOCK(self->logger, flags::INFO, "Remote endpoint rejected: %s", oss.str().c_str());

            stream->lowest_layer().close();
            return;
        }

        auto handshake_cb = [stream, ID, self](const std::error_code& ec) {
            if (ec)
            {
                FORMAT_LOG_BLOCK(self->logger, flags::INFO, "TLS handshake failed: %s", ec.message().c_str());
                return;
            }

            self->AcceptStream(ID, self->executor, stream);
        };

        // Begin the TLS handshake
        stream->async_handshake(asio::ssl::stream_base::server, self->executor->wrap(handshake_cb));
    };

    this->acceptor.async_accept(stream->lowest_layer(), this->executor->wrap(accept_cb));
}

} // namespace opendnp3
