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
#ifndef SECAUTH_KEY_CHANGE_CONFIRMATION_HMAC_H
#define SECAUTH_KEY_CHANGE_CONFIRMATION_HMAC_H

#include <cstdint>

#include "AuthSizes.h"

#include <openpal/crypto/IHMACAlgo.h>
#include <openpal/container/StaticBuffer.h>

#include <opendnp3/app/User.h>

namespace secauth
{
	/**
	*  Class for calculating an HMAC as defined in 1815 pg 757
	*/
	class KeyChangeConfirmationHMAC
	{
		public:
			KeyChangeConfirmationHMAC(openpal::IHMACAlgo& algorithm);						
			
			openpal::RSlice Compute(
				const openpal::RSlice& key, 
				const std::string& name,
				const openpal::RSlice& senderNonce,
				const openpal::RSlice& receiverNonce,
				uint32_t keyChangeSeqNum,
				opendnp3::User user,
				std::error_code& ec
			);
			
		private:
			
			openpal::IHMACAlgo* m_algorithm;
			
			openpal::StaticBuffer<AuthSizes::MAX_HMAC_OUTPUT_SIZE> m_buffer;
	};
}

#endif

