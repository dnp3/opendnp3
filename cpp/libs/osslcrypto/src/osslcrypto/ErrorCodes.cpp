/**
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

#include "ErrorCodes.h"

namespace osslcrypto
{
	CryptoErrorCategory CryptoErrorCategory::instance;

	std::string CryptoErrorCategory::message(int ev) const
	{
		switch (ev)
		{
			case(errors::HMAC_INSUFFICIENT_OUTPUT_BUFFER_SIZE) :
				return "insufficient output buffer size for HMAC calculation";
			case(errors::OPENSSL_HMAC_INIT_EX_ERROR) :
				return "openssl: failure calling HMAC_Init_ex()";
			case(errors::OPENSSL_HMAC_UPDATE_ERROR) :
				return "openssl: failure calling HMAC_Update()";
			case(errors::OPENSSL_HMAC_FINAL_ERROR) :
				return "openssl: failure calling HMAC_Final()";

			case(errors::OPENSSL_RAND_BYTES_ERROR) :
				return "openssl: failure calling RAND_bytes()";

			case(errors::AES_WRAPKEY_KEK_SIZE_MISMATCH) :
				return "unexpected KEK in WrapKeyAES()";
			case(errors::AES_WRAPKEY_INPUT_NOT_DIV8) :
				return "input to WrapKeyAES() not divisible by 8";
			case(errors::AES_WRAPKEY_INSUFFICIENT_OUTPUT_BUFFER_SIZE):
				return "insufficient output buffer size for WrapKey()";
			case(errors::AES_WRAPKEY_AES_SET_ENCRYPT_KEY_ERROR) :
				return "openssl: failure calling AES_set_encrypt_key()";
			case(errors::AES_WRAPKEY_AES_WRAP_KEY_ERROR) :
				return "openssl: failure calling AES_wrap_key()";

			case(errors::AES_UNWRAPKEY_KEK_SIZE_MISMATCH):
				return "unexpected KEK in UnwrapKeyAES()";			
			case(errors::AES_UNWRAPKEY_INPUT_NOT_DIV8) :
				return  "Input to UnwrapKeyAES() not divisible by 8";
			case(errors::AES_UNWRAPKEY_INSUFFICIENT_OUTPUT_BUFFER_SIZE) :
				return "insufficient output buffer size for UnwrapKey()";
			case(errors::AES_UNWRAPKEY_AES_SET_DECRYPT_KEY_ERROR) :
				return "openssl: failure calling AES_set_decrypt_key()";
			case(errors::AES_UNWRAPKEY_AES_UNWRAP_KEY_IV_ERROR) :
				return "openssl: decryption of key data failed in aes_key_unwrap(). Do you have the right KEK?";
			case(errors::AES_UNWRAPKEY_AES_UNWRAP_KEY_PARAM_ERROR) :
				return "openssl: failure calling aes_key_unwrap()";

			default:
				return "unknown error";
		};
	}
}

namespace std
{
	std::error_code make_error_code(osslcrypto::errors::Error err)
	{
		return std::error_code(err, osslcrypto::CryptoErrorCategory::Instance());
	}
}



