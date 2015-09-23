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


#include "asiotls/PhysicalLayerTLSClient.h"

using namespace asio;

namespace asiotls
{

	PhysicalLayerTLSClient::PhysicalLayerTLSClient(
		openpal::LogRoot& root,
		asio::io_service& service,
		const std::string& host_,
		const std::string& localAddress_,
		uint16_t port,
		const std::string& peerCertFilePath,		
		const std::string& privateKeyFilePath
	) :
			PhysicalLayerTLSBase(root, service, ssl::context_base::sslv23_client),
			condition(logger),
			host(host_),
			localAddress(localAddress_),
			remoteEndpoint(ip::tcp::v4(), port),
			localEndpoint(),
			resolver(service)			
	{		

		const auto OPTIONS = ssl::context::default_workarounds | ssl::context::no_sslv2 | ssl::context::no_sslv3;

		ctx.set_options(OPTIONS);

		// configure us to verify the peer certificate
		stream.set_verify_mode(ssl::verify_peer);

		// The public certificate file used to verify the peer
		ctx.load_verify_file(peerCertFilePath);

		// additionally, call this callback for the purposes of logging only
		ctx.set_verify_callback(
			[this](bool preverified, asio::ssl::verify_context& ctx) 
			{ 
					return this->VerifyServerCertificate(preverified, ctx); 
			}
		);
		
		// the certificate we present to the server + the private key we use
		ctx.use_certificate_file(privateKeyFilePath, asio::ssl::context_base::file_format::pem);
	}

	bool PhysicalLayerTLSClient::VerifyServerCertificate(bool preverified, asio::ssl::verify_context& ctx)
	{
		// The verify callback can be used to check whether the certificate that is
		// being presented is valid for the peer. For example, RFC 2818 describes
		// the steps involved in doing this for HTTPS. Consult the OpenSSL
		// documentation for more details. Note that the callback is called once
		// for each certificate in the certificate chain, starting from the root
		// certificate authority.

		// In this example we will simply print the certificate's subject name.
		
		X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());

		char subjectName[256];
		X509_NAME_oneline(X509_get_subject_name(cert), subjectName, 256);
						
		FORMAT_LOG_BLOCK(logger, openpal::logflags::EVENT, "Verifying %s", subjectName);

		return preverified;
	}
	
	void PhysicalLayerTLSClient::DoOpen()
	{
		std::error_code ec;
		this->BindToLocalAddress(ec);
		if (ec)
		{
			auto callback = [this, ec]()
			{
				this->OnOpenCallback(ec);
			};
			executor.strand.post(callback);
			return;
		}
		
		auto address = asio::ip::address::from_string(host, ec);
		if (ec)
		{
			auto callback = [this](const std::error_code & ec, ip::tcp::resolver::iterator endpoints)
			{
				this->HandleResolveResult(ec, endpoints);
			};
			ip::tcp::resolver::query query(host, "20000");
			resolver.async_resolve(query, executor.strand.wrap(callback));
		}
		else
		{
			remoteEndpoint.address(address);
			auto callback = [this](const std::error_code & ec)
			{
				this->HandleConnectResult(ec);
			};

			stream.lowest_layer().async_connect(remoteEndpoint, executor.strand.wrap(callback));			
		}

	}

	void PhysicalLayerTLSClient::DoOpeningClose()
	{
		this->Shutdown();
	}
	
	void PhysicalLayerTLSClient::DoOpenSuccess()
	{
		SIMPLE_LOG_BLOCK(logger, openpal::logflags::INFO, "Connected to host");		
	}

	void PhysicalLayerTLSClient::BindToLocalAddress(std::error_code& ec)
	{
		auto string = localAddress.empty() ? "0.0.0.0" : localAddress;
		auto addr = asio::ip::address::from_string(string, ec);
		if (!ec)
		{
			localEndpoint.address(addr);
			stream.lowest_layer().open(ip::tcp::v4(), ec);
			if (!ec)
			{
				stream.lowest_layer().bind(localEndpoint, ec);
			}
		}
	}
	
	void PhysicalLayerTLSClient::HandleResolveResult(const std::error_code& ec, asio::ip::tcp::resolver::iterator endpoints)
	{
		if (ec)
		{
			this->OnOpenCallback(ec);
		}
		else
		{
			// attempt a connection to each endpoint in the iterator until we connect
			auto callback = [this](const std::error_code & code, ip::tcp::resolver::iterator endpoints)
			{
				this->HandleConnectResult(code);
			};

			asio::async_connect(stream.lowest_layer(), endpoints, condition, executor.strand.wrap(callback));
		}
	}

	void PhysicalLayerTLSClient::HandleConnectResult(const std::error_code& ec)
	{
		if (ec)
		{
			this->OnOpenCallback(ec);
		}
		else
		{
			auto callback = [this](const std::error_code& code)
			{
				this->OnOpenCallback(code);
			};

			this->stream.async_handshake(asio::ssl::stream_base::client, executor.strand.wrap(callback));
		}
	}
	
}


