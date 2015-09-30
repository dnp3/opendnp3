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

#include "asiopal/tls/TLSHelpers.h"

using namespace asio;

namespace asiopal
{

void TLSHelpers::ApplyConfig(const TLSConfig& config, asio::ssl::context& context)
{
	auto OPTIONS = ssl::context::default_workarounds | ssl::context::no_sslv2 | ssl::context::no_sslv3;

	if (!config.allowTLSv10)
	{
		OPTIONS |= ssl::context::no_tlsv1;
	}

	if (!config.allowTLSv11)
	{
		OPTIONS |= ssl::context::no_tlsv1_1;
	}

	if (!config.allowTLSv12)
	{
		OPTIONS |= ssl::context::no_tlsv1_2;
	}

	context.set_options(OPTIONS);

	// optionally, configure the cipher-list
	if (!config.cipherList.empty())
	{
		SSL_CTX_set_cipher_list(context.native_handle(), config.cipherList.c_str());
	}

	// verify the peer certificate
	context.set_verify_mode(ssl::verify_peer);

	// The public certificate file used to verify the peer
	context.load_verify_file(config.peerCertFilePath);

	// the certificate we present to the server + the private key we use are placed into the same file
	context.use_certificate_file(config.localCertFilePath, asio::ssl::context_base::file_format::pem);
	context.use_private_key_file(config.privateKeyFilePath, asio::ssl::context_base::file_format::pem);
}

}


