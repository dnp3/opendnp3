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

#ifndef ASIOPAL_PHYSICAL_LAYER_TLS_BASE_H
#define ASIOPAL_PHYSICAL_LAYER_TLS_BASE_H

#include "asiopal/PhysicalLayerASIO.h"
#include "asiopal/tls/SSLContext.h"

#include <asio.hpp>
#include <asio/ip/tcp.hpp>
#include <asio/ssl.hpp>

namespace asiopal
{

/**
Common TLS stream object and some shared implementations for server/client
*/
class PhysicalLayerTLSBase : public PhysicalLayerASIO
{
public:

	PhysicalLayerTLSBase(
	    openpal::Logger logger,
	    asio::io_service& service,
	    const TLSConfig& config,
	    bool server,
	    std::error_code& ec
	);

	virtual ~PhysicalLayerTLSBase() {}

	// ---- Implement the shared client/server actions ----

	void DoClose() override final;
	void DoRead(openpal::WSlice&) override final;
	void DoWrite(const openpal::RSlice&) override final;
	void DoOpenFailure() override final;

protected:

	bool LogPeerCertificateInfo(bool preverified, asio::ssl::verify_context& ctx);

	SSLContext ctx;
	asio::ssl::stream<asio::ip::tcp::socket> stream;

	void ShutdownTLSStream();
	void ShutdownSocket();
	void CloseSocket();

};
}

#endif
