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

#include "osslcrypto/AESKeyWrap.h"

#include <openssl/aes.h>

#include "osslcrypto/ErrorCodes.h"

using namespace openpal;

namespace osslcrypto
{				
	openpal::RSlice AESKeyWrap::WrapKey(const openpal::RSlice& kek, const openpal::RSlice& input, openpal::WSlice& output, std::error_code& ec) const
	{		
		const uint32_t KEY_SIZE_BYTES = kek.Size();
		const int KEY_SIZE_BITS = static_cast<int>(KEY_SIZE_BYTES*8);		

		// can only wrap things pre-padded into 8-byte blocks
		if ((input.Size() % 8) != 0)
		{
			ec = make_error_code(errors::AES_WRAPKEY_INPUT_NOT_DIV8);
			return RSlice::Empty();
		}

		const uint32_t OUTPUT_SIZE = input.Size() + 8;

		// the wrapped data is always 8 bytes larger than the input
		if (output.Size() < OUTPUT_SIZE)
		{
			ec = make_error_code(errors::AES_WRAPKEY_INSUFFICIENT_OUTPUT_BUFFER_SIZE);
			return RSlice::Empty();
		}

		AES_KEY key;
		if (AES_set_encrypt_key(kek, KEY_SIZE_BITS, &key))
		{
			ec = make_error_code(errors::AES_WRAPKEY_AES_SET_KEY_ERROR);
			return RSlice::Empty();
		}

		// If iv is null, the default IV is used
		const int LEN = AES_wrap_key(&key, nullptr, output, input, input.Size());
		if (LEN > 0)
		{
			auto ret = output.ToRSlice().Take(OUTPUT_SIZE);
			output.Advance(OUTPUT_SIZE);
			return ret;
		}
		else
		{
			ec = make_error_code(errors::AES_WRAPKEY_AES_WRAP_KEY_ERROR);
			return RSlice::Empty();
		}		
	}
	
	openpal::RSlice AESKeyWrap::UnwrapKey(const openpal::RSlice& kek, const openpal::RSlice& input, openpal::WSlice& output, std::error_code& ec) const
	{
		const uint32_t KEY_SIZE_BYTES = kek.Size();
		const int KEY_SIZE_BITS = static_cast<int>(KEY_SIZE_BYTES*8);			

		// can only unwrap things pre-padded into 64-bit blocks
		if ((input.Size() < 8) && input.Size() % 8 != 0)
		{
			ec = make_error_code(errors::AES_UNWRAPKEY_INPUT_NOT_DIV8);
			return RSlice::Empty();
		}

		// the wrapped data is always 8 bytes larger than what is output
		const uint32_t OUTPUT_SIZE = input.Size() - 8;

		if (output.Size() < OUTPUT_SIZE)
		{
			ec = make_error_code(errors::AES_UNWRAPKEY_INSUFFICIENT_OUTPUT_BUFFER_SIZE);
			return RSlice::Empty();
		}

		AES_KEY key;
		if (AES_set_decrypt_key(kek, KEY_SIZE_BITS, &key))
		{
			ec = make_error_code(errors::AES_WRAPKEY_AES_SET_KEY_ERROR);
			return RSlice::Empty();
		}		

		// If iv is null, the default IV is used
		const int RESULT = AES_unwrap_key(&key, nullptr, output, input, input.Size());


		if (RESULT > 0)
		{
			auto ret = output.ToRSlice().Take(OUTPUT_SIZE);
			output.Advance(OUTPUT_SIZE);
			return ret;
		}		
		else
		{
			auto code = (RESULT == 0) ? errors::AES_UNWRAPKEY_AES_UNWRAP_KEY_IV_ERROR : errors::AES_UNWRAPKEY_AES_UNWRAP_KEY_PARAM_ERROR;
			ec = make_error_code(code);
			return RSlice::Empty();
		}				
	}
}

