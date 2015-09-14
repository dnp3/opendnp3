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

#ifndef OSSLCRYPTO_ERRORCODES_H
#define OSSLCRYPTO_ERRORCODES_H

#include <system_error>

#ifndef _MSC_VER
#define NOEXCEPT noexcept
#else
#define NOEXCEPT
#endif

namespace osslcrypto
{
namespace errors
{
enum Error : int
{
    // --- hmac errors ---

    HMAC_INSUFFICIENT_OUTPUT_BUFFER_SIZE,
    OPENSSL_HMAC_INIT_EX_ERROR,
    OPENSSL_HMAC_UPDATE_ERROR,
    OPENSSL_HMAC_FINAL_ERROR,

    // --- secure random errors ---
    OPENSSL_RAND_BYTES_ERROR,

    AES_WRAPKEY_AES_SET_KEY_ERROR,

    // --- key wrap errors ----
    AES_WRAPKEY_INPUT_NOT_DIV8,
    AES_WRAPKEY_INSUFFICIENT_OUTPUT_BUFFER_SIZE,
    AES_WRAPKEY_AES_WRAP_KEY_ERROR,

    AES_UNWRAPKEY_INPUT_NOT_DIV8,
    AES_UNWRAPKEY_INSUFFICIENT_OUTPUT_BUFFER_SIZE,
    AES_UNWRAPKEY_AES_UNWRAP_KEY_IV_ERROR,
    AES_UNWRAPKEY_AES_UNWRAP_KEY_PARAM_ERROR
};
}

class CryptoErrorCategory final : public std::error_category
{
public:

	static const std::error_category& Instance()
	{
		return instance;
	}

	virtual const char* name() const NOEXCEPT
	{
		return "OSSL Errors";
	}
	virtual std::string message(int ev) const;

private:

	CryptoErrorCategory() {}
	CryptoErrorCategory(const CryptoErrorCategory&) = delete;

	static CryptoErrorCategory instance;
};

std::error_code make_error_code(osslcrypto::errors::Error err);
}

namespace std
{
template <>
struct is_error_code_enum<osslcrypto::errors::Error> : public true_type {};
}




#endif
