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
#ifndef __MOCK_CRYPTOPROVIDER_H_
#define __MOCK_CRYPTOPROVIDER_H_

#include <openpal/crypto/ICryptoProvider.h>

#include "MockHMAC.h"
#include "MockKeyWrap.h"

namespace opendnp3
{	

	class MockCryptoProvider : public openpal::ICryptoProvider
	{
		public:

			MockCryptoProvider();

			virtual openpal::RSlice GetSecureRandom(openpal::WSlice& buffer, std::error_code& ec) override final;			

			virtual openpal::IHMACAlgo& GetSHA1HMAC() override final;
			virtual openpal::IHMACAlgo& GetSHA256HMAC() override final;

			virtual openpal::IKeyWrapAlgo& GetAESKeyWrap() override final;		

			/// -- fixed fill data, defaults to 0xAA --
			uint8_t secureFill;
		
			/// --- algo mocks ----

			MockHMAC sha1;
			MockHMAC sha256;
			MockKeyWrap keyWrap;						
	};
}

#endif

