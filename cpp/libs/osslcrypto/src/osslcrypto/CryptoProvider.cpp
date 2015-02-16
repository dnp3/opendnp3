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

bool CryptoProvider::Aes128KeyWrap(const ReadBufferView& kek, const ReadBufferView& input, WriteBufferView& output)
{
	// If iv is null, the default IV is used
	if (kek.Size() != 16)
	{
		return false;
	}	

	AES_KEY key;
	if (AES_set_encrypt_key(kek, 128, &key))
	{
		return false;
	}	

	AES_wrap_key(&key, nullptr, output, input, input.Size());

	output.Advance(input.Size() + 8);

	return true;
}

}
