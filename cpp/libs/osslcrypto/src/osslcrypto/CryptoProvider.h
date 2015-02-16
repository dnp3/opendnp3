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

#include <openpal/crypto/ICryptoProvider.h>
#include <openpal/util/Uncopyable.h>

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

	virtual bool WrapKeyAES128(const openpal::ReadBufferView& kek, const openpal::ReadBufferView& input, openpal::WriteBufferView& output) override final
	{
		return WrapKeyAES(AESKeyLength::L128, kek, input, output);
	}

	virtual bool KeyWrapAES256(const openpal::ReadBufferView& kek, const openpal::ReadBufferView& input, openpal::WriteBufferView& output) override final
	{
		return WrapKeyAES(AESKeyLength::L256, kek, input, output);
	}
	
	private:	

	static bool WrapKeyAES(AESKeyLength length, const openpal::ReadBufferView& kek, const openpal::ReadBufferView& input, openpal::WriteBufferView& output);

	static void LockingFunction(int mode, int n, const char *file, int line);
			
	static bool Initialize();
	static bool ConfigureMultithreading();
		
	static std::vector < std::unique_ptr<std::mutex> > mutexes;
	static bool initialized;
};

}
