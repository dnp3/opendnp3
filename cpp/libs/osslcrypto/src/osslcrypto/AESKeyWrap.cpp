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

#include "AESKeyWrap.h"

#include <openssl/aes.h>

#include <openpal/logging/LogMacros.h>
#include <openpal/logging/LogLevels.h>

using namespace openpal;

namespace osslcrypto
{		
	bool AESKeyWrap::WrapKeyAES(AESKeyLength length, const openpal::ReadBufferView& kek, const openpal::ReadBufferView& input, openpal::WriteBufferView& output, openpal::Logger* pLogger)
	{
		const int KEY_SIZE = (length == AESKeyLength::L128) ? 16 : 32;
		const int KEY_SIZE_BITS = (length == AESKeyLength::L128) ? 128 : 256;

		// the key size must match
		if (kek.Size() != KEY_SIZE)
		{
			FORMAT_LOGGER_BLOCK(pLogger, logflags::WARN, "KEK size (%u) does not match expected size (%u)", kek.Size(), KEY_SIZE);
			return false;
		}

		// can only wrap things pre-padded into 8-byte blocks
		if (input.Size() % 8 != 0)
		{
			FORMAT_LOGGER_BLOCK(pLogger, logflags::WARN, "Input data size (%u) not evenly divisible by 8", input.Size());
			return false;
		}

		const uint32_t OUTPUT_SIZE = input.Size() + 8;

		// the wrapped data is always 8 bytes larger than the input
		if (output.Size() < OUTPUT_SIZE)
		{
			FORMAT_LOGGER_BLOCK(pLogger, logflags::WARN, "Not enough space in buffer (%u) to write output (%u)", output.Size(), OUTPUT_SIZE);
			return false;
		}

		AES_KEY key;
		if (AES_set_encrypt_key(kek, KEY_SIZE_BITS, &key))
		{
			SIMPLE_LOGGER_BLOCK(pLogger, logflags::WARN, "Unable to create AES encryption key from kek");
			return false;
		}

		// If iv is null, the default IV is used
		bool success = AES_wrap_key(&key, nullptr, output, input, input.Size()) > 0;
		if (success)
		{
			output.Advance(OUTPUT_SIZE);
		}
		else
		{
			SIMPLE_LOGGER_BLOCK(pLogger, logflags::WARN, "Unspecified error wrapping key data");
		}

		return success;
	}
	
	bool AESKeyWrap::UnwrapKeyAES(AESKeyLength length, const openpal::ReadBufferView& kek, const openpal::ReadBufferView& input, openpal::WriteBufferView& output, openpal::Logger* pLogger)
	{
		const int KEY_SIZE = (length == AESKeyLength::L128) ? 16 : 32;
		const int KEY_SIZE_BITS = (length == AESKeyLength::L128) ? 128 : 256;

		// the key size must match
		if (kek.Size() != KEY_SIZE)
		{
			FORMAT_LOGGER_BLOCK(pLogger, logflags::WARN, "KEK size (%u) does not match expected size (%u)", kek.Size(), KEY_SIZE);
			return false;
		}

		// can only unwrap things pre-padded into 64-bit blocks
		if ((input.Size() < 8) && input.Size() % 8 != 0)
		{
			FORMAT_LOGGER_BLOCK(pLogger, logflags::WARN, "Input data size (%u) not evenly divisible by 8", input.Size());
			return false;
		}

		// the wrapped data is always 8 bytes larger than what is output
		const uint32_t OUTPUT_SIZE = input.Size() - 8;

		if (output.Size() < OUTPUT_SIZE)
		{
			FORMAT_LOGGER_BLOCK(pLogger, logflags::WARN, "Not enough space in buffer (%u) to write output (%u)", output.Size(), OUTPUT_SIZE);
			return false;
		}

		AES_KEY key;
		if (AES_set_decrypt_key(kek, KEY_SIZE_BITS, &key))
		{
			SIMPLE_LOGGER_BLOCK(pLogger, logflags::WARN, "Unable to create AES decryption key from kek");
			return false;
		}

		// If iv is null, the default IV is used
		bool success = AES_unwrap_key(&key, nullptr, output, input, input.Size()) > 0;
		if (success)
		{
			output.Advance(OUTPUT_SIZE);
		}		
		else
		{
			SIMPLE_LOGGER_BLOCK(pLogger, logflags::WARN, "Unspecified error wrapping key data");
		}		

		return true;
	}
}

