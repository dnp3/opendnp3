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

#include "asiopal/tls/PhysicalLayerTLSBase.h"

#include <openpal/logging/LogMacros.h>
#include <openpal/logging/LogLevels.h>

using namespace asio;
using namespace openpal;
using namespace asio::ssl;

namespace asiopal
{

PhysicalLayerTLSBase::PhysicalLayerTLSBase(
    openpal::Logger logger,
    asio::io_service& service,
    const TLSConfig& config,
    bool server,
    std::error_code& ec) :

	PhysicalLayerASIO(logger, service),
	ctx(logger, server, config, ec),
	stream(service, ctx.value)
{
	stream.set_verify_callback(
	    [this](bool preverified, asio::ssl::verify_context & ctx)
	{
		return this->LogPeerCertificateInfo(preverified, ctx);
	}
	);
}

bool PhysicalLayerTLSBase::LogPeerCertificateInfo(bool preverified, asio::ssl::verify_context& ctx)
{
	// This is just for logging purposes to log the subject name of the certificate if verifies or not

	X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());

	char subjectName[256];
	X509_NAME_oneline(X509_get_subject_name(cert), subjectName, 256);

	FORMAT_LOG_BLOCK(logger, openpal::logflags::INFO, preverified ? "Verified subject_name: %s" : "Did not verify subject_name: %s", subjectName);

	return preverified;
}

// ---- Implement the shared client/server actions ----

void PhysicalLayerTLSBase::DoClose()
{
	this->ShutdownTLSStream();
	this->ShutdownSocket();
	this->CloseSocket();
}

void PhysicalLayerTLSBase::DoRead(openpal::WSlice& dest)
{
	uint8_t* pBuff = dest;

	auto callback = [this, pBuff](const std::error_code & ec, size_t  numRead)
	{
		this->OnReadCallback(ec, pBuff, static_cast<uint32_t>(numRead));
	};

	stream.async_read_some(buffer(pBuff, dest.Size()), executor.strand.wrap(callback));
}

void PhysicalLayerTLSBase::DoWrite(const openpal::RSlice& data)
{
	auto callback = [this](const std::error_code & code, size_t  numWritten)
	{
		this->OnWriteCallback(code, static_cast<uint32_t>(numWritten));
	};

	async_write(stream, buffer(data, data.Size()), executor.strand.wrap(callback));
}

void PhysicalLayerTLSBase::DoOpenFailure()
{
	SIMPLE_LOG_BLOCK(logger, logflags::DBG, "Failed socket open, closing socket");
	this->ShutdownSocket();
	this->CloseSocket();
}

void PhysicalLayerTLSBase::ShutdownTLSStream()
{
	std::error_code ec;
	stream.shutdown(ec);
	if (ec)
	{
		FORMAT_LOG_BLOCK(logger, logflags::DBG, "Error while shutting down TLS stream: %s", ec.message().c_str());
	}
}

void PhysicalLayerTLSBase::ShutdownSocket()
{

	std::error_code ec;
	stream.lowest_layer().shutdown(ip::tcp::socket::shutdown_both, ec);
	if (ec)
	{
		FORMAT_LOG_BLOCK(logger, logflags::DBG, "Error shutting down socket: %s", ec.message().c_str());
	}
}

void PhysicalLayerTLSBase::CloseSocket()
{
	std::error_code ec;
	stream.lowest_layer().close(ec);
	if (ec)
	{
		FORMAT_LOG_BLOCK(logger, logflags::DBG, "Error closing socket: %s", ec.message().c_str());
	}
}

}

