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

#include "asiopal/tls/SSLContext.h"

namespace asiopal
{			
	SSLContext::SSLContext(bool server, const TLSConfig& config, std::error_code& ec) : 
		value(server ? asio::ssl::context_base::sslv23_server : asio::ssl::context_base::sslv23_client)
	{
		this->ApplyConfig(config, server, ec);
	}

	int SSLContext::GetVerifyMode(bool server)
	{
		return server ? (asio::ssl::verify_peer | asio::ssl::verify_fail_if_no_peer_cert) : asio::ssl::verify_peer;
	}

	std::error_code SSLContext::ApplyConfig(const TLSConfig& config, bool server, std::error_code& ec)
	{		
		auto OPTIONS = asio::ssl::context::default_workarounds | asio::ssl::context::no_sslv2 | asio::ssl::context::no_sslv3 | SSL_OP_NO_TICKET;		

		if (!config.allowTLSv10)
		{
			OPTIONS |= asio::ssl::context::no_tlsv1;
		}

		if (!config.allowTLSv11)
		{
			OPTIONS |= asio::ssl::context::no_tlsv1_1;
		}

		if (!config.allowTLSv12)
		{
			OPTIONS |= asio::ssl::context::no_tlsv1_2;
		}		

		if (value.set_options(OPTIONS, ec)) return ec;

		// optionally, configure the cipher-list
		if (!config.cipherList.empty())
		{
			if (SSL_CTX_set_cipher_list(value.native_handle(), config.cipherList.c_str()) == 0)
			{
				ec = asio::error_code();
				return ec;
			}
		}		

		// verify the peer certificate
		if (value.set_verify_mode(GetVerifyMode(server), ec)) return ec;

		// The public certificate file used to verify the peer
		if (value.load_verify_file(config.peerCertFilePath, ec)) return ec;

		// the certificate we present to the server + the private key we use are placed into the same file
		if (value.use_certificate_chain_file(config.localCertFilePath, ec)) return ec;

		value.use_private_key_file(config.privateKeyFilePath, asio::ssl::context_base::file_format::pem, ec);

		return ec;
	}
}


