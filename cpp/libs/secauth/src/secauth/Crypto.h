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
#ifndef SECAUTH_CRYPTO_H
#define SECAUTH_CRYPTO_H

#include <openpal/crypto/ICryptoProvider.h>
#include <openpal/util/Uncopyable.h>
#include <openpal/container/Buffer.h>

#include <opendnp3/gen/KeyWrapAlgorithm.h>
#include <opendnp3/gen/HMACType.h>

#include "SessionKeysView.h"

#include "HMACMode.h"

#include <memory>

namespace secauth
{
	// set of helpers on top of ICryptoProvider
	class Crypto : private openpal::StaticOnly
	{
		public:
								
			// return nullptr if the specified KeyWrapAlgorithm isn't supported
			static openpal::IKeyWrapAlgo* TryGetKeyWrap(openpal::ICryptoProvider& crypto, opendnp3::KeyWrapAlgorithm algorithm);
			

			static bool KeyLengthMatchesRequestedAlgorithm(opendnp3::KeyWrapAlgorithm algorithm, uint32_t updateKeyLength);

			static bool TryGetHMACMode(opendnp3::HMACType type, HMACMode& mode);

	};

	
}

#endif
