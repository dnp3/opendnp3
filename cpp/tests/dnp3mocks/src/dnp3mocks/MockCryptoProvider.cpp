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

#include "MockCryptoProvider.h"

#include <cstring>

namespace opendnp3
{	
	MockCryptoProvider::MockCryptoProvider() :
		secureFill(0xAA),
		sha1(20),
		sha256(32)
	{}
	

	openpal::RSlice MockCryptoProvider::GetSecureRandom(openpal::WSlice& buffer, std::error_code& ec)
	{
		memset(buffer, secureFill, buffer.Size());
		auto ret = buffer.ToReadOnly();
		buffer.Advance(buffer.Size());
		return ret;		
	}

	openpal::IHMACAlgo& MockCryptoProvider::GetSHA1HMAC()
	{
		return sha1;
	}
	
	openpal::IHMACAlgo& MockCryptoProvider::GetSHA256HMAC()
	{
		return sha256;
	}

	openpal::IKeyWrapAlgo& MockCryptoProvider::GetAES256KeyWrap()
	{
		return aes256;
	}
	
	openpal::IKeyWrapAlgo& MockCryptoProvider::GetAES128KeyWrap()
	{
		return aes128;
	}	
}


