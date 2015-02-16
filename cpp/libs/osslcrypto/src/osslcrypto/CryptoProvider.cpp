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

#include "CryptoProvider.h"

#include <openssl/rand.h>
#include <openssl/crypto.h>
#include <openssl/aes.h>
#include <openssl/sha.h>

#include <assert.h>

using namespace openpal;

namespace osslcrypto
{

std::vector < std::unique_ptr<std::mutex> > CryptoProvider::mutexes;

bool CryptoProvider::initialized = Initialize();

bool CryptoProvider::Initialize()
{
	return ConfigureMultithreading();
}

bool CryptoProvider::ConfigureMultithreading()
{
	// openssl is not thread-safe by default.
	// If this isn't done and functions like RAND_bytes are called
	// from multiple threads you'll get random crashes.

	for (int i = 0; i < CRYPTO_num_locks(); ++i)
	{
		mutexes.push_back(std::make_unique<std::mutex>());
	}

	// specific the function that will lock and unlock the various mutexes
	
	CRYPTO_set_locking_callback(LockingFunction);

	return true;
}

void CryptoProvider::LockingFunction(int mode, int n, const char *file, int line)
{
	assert(n > 0);
	assert(static_cast<size_t>(n) < mutexes.size());

	if (mode & CRYPTO_LOCK) 
	{
		mutexes[n]->lock();		
	}
	else 
	{
		mutexes[n]->unlock();		
	}
}

bool CryptoProvider::GetSecureRandom(WriteBufferView& buffer)
{	
	return RAND_bytes(buffer, buffer.Size()) > 0;
}

bool CryptoProvider::WrapKeyAES128(const openpal::ReadBufferView& kek, const openpal::ReadBufferView& input, openpal::WriteBufferView& output)
{
	return WrapKeyAES(AESKeyLength::L128, kek, input, output);
}

bool CryptoProvider::WrapKeyAES256(const openpal::ReadBufferView& kek, const openpal::ReadBufferView& input, openpal::WriteBufferView& output)
{
	return WrapKeyAES(AESKeyLength::L256, kek, input, output);
}

bool CryptoProvider::UnwrapKeyAES128(const openpal::ReadBufferView& kek, const openpal::ReadBufferView& input, openpal::WriteBufferView& output)
{
	return UnwrapKeyAES(AESKeyLength::L128, kek, input, output);
}

bool CryptoProvider::UnwrapKeyAES256(const openpal::ReadBufferView& kek, const openpal::ReadBufferView& input, openpal::WriteBufferView& output)
{
	return UnwrapKeyAES(AESKeyLength::L256, kek, input, output);
}

bool CryptoProvider::CalcSHA1(const ReadBufferView& input, WriteBufferView& output)
{
	if (output.Size() < 20)
	{
		return false;
	}
	else
	{
		SHA1(input, input.Size(), output);
		output.Advance(20);
		return true;
	}	
}

bool CryptoProvider::WrapKeyAES(AESKeyLength length, const ReadBufferView& kek, const ReadBufferView& input, WriteBufferView& output)
{
	const int KEY_SIZE = (length == AESKeyLength::L128) ? 16 : 32;
	const int KEY_SIZE_BITS = (length == AESKeyLength::L128) ? 128 : 256;

	// the key size must match
	if (kek.Size() != KEY_SIZE)
	{
		return false;
	}	

	// can only wrap things pre-padded into 64-bit blocks
	if (input.Size() % 8 != 0)
	{
		return false;
	}

	const uint32_t OUTPUT_SIZE = input.Size() + 8;

	// the wrapped data is always 8 bytes larger than the input
	if (output.Size() < OUTPUT_SIZE)
	{
		return false;
	}

	AES_KEY key;
	if (AES_set_encrypt_key(kek, KEY_SIZE_BITS, &key))
	{
		return false;
	}	

	// If iv is null, the default IV is used
	AES_wrap_key(&key, nullptr, output, input, input.Size());

	output.Advance(OUTPUT_SIZE);

	return true;
}

bool CryptoProvider::UnwrapKeyAES(AESKeyLength length, const openpal::ReadBufferView& kek, const openpal::ReadBufferView& input, openpal::WriteBufferView& output)
{
	const int KEY_SIZE = (length == AESKeyLength::L128) ? 16 : 32;
	const int KEY_SIZE_BITS = (length == AESKeyLength::L128) ? 128 : 256;

	// the key size must match
	if (kek.Size() != KEY_SIZE)
	{
		return false;
	}

	// can only unwrap things pre-padded into 64-bit blocks
	if ((input.Size() < 8) && input.Size() % 8 != 0)
	{
		return false;
	}

	// the wrapped data is always 8 bytes larger than what is output
	const uint32_t OUTPUT_SIZE = input.Size() - 8;
	
	if (output.Size() < OUTPUT_SIZE)
	{
		return false;
	}

	AES_KEY key;
	if (AES_set_decrypt_key(kek, KEY_SIZE_BITS, &key))
	{
		return false;
	}

	// If iv is null, the default IV is used
	AES_unwrap_key(&key, nullptr, output, input, input.Size());

	output.Advance(OUTPUT_SIZE);

	return true;
}

}
