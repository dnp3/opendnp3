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

#ifndef OSSLCRYPTO_CRYPTOPROVIDER_H
#define OSSLCRYPTO_CRYPTOPROVIDER_H

#include <openpal/crypto/ICryptoProvider.h>
#include <openpal/util/Uncopyable.h>

#include "AESKeyWrap.h"
#include "SHA1HMAC.h"
#include "SHA256HMAC.h"

#include <mutex>
#include <vector>
#include <memory>

namespace osslcrypto
{

/**
* An ICryptoProvider based on openssl
*/
class CryptoProvider : public openpal::ICryptoProvider, private openpal::Uncopyable
{	
	enum class AESKeyLength : uint8_t
	{
		L128,
		L256
	};

	public:	

	virtual bool GetSecureRandom(openpal::WriteBufferView& buffer) override final;	
	
	virtual openpal::IKeyWrapAlgo& GetAES128KeyWrap() override final;
	virtual openpal::IKeyWrapAlgo& GetAES256KeyWrap() override final;	

	virtual openpal::IHMACAlgo& GetSHA1HMAC() override final;
	virtual openpal::IHMACAlgo& GetSHA256HMAC() override final;
	
	private:

	static void LockingFunction(int mode, int n, const char *file, int line);

	// singleton values of the various hmac algorithms
	static SHA1HMAC hmacSHA1;
	static SHA256HMAC hmacSHA256;

	// singleton values of the various key wrap algorithms
	static AESKeyWrap128 keywrap128;
	static AESKeyWrap256 keywrap256;
			
	static bool Initialize();
	static bool ConfigureMultithreading();	


	static std::vector < std::unique_ptr<std::mutex> > mutexes;
	static bool initialized;
};

}

#endif
