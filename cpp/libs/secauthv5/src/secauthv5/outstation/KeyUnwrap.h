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
#ifndef SECAUTHV5_KEYUNWRAP_H
#define SECAUTHV5_KEYUNWRAP_H

#include <openpal/logging/Logger.h>
#include <openpal/crypto/IKeyWrapAlgo.h>
#include <openpal/container/ReadBufferView.h>
#include <openpal/container/StaticBuffer.h>
#include <openpal/serialization/Serialization.h>


#include <opendnp3/objects/Group120Var6.h>

#include "secauthv5/AuthConstants.h"



namespace secauthv5
{

	class UnwrappedKeyData
	{
	public:
		openpal::ReadBufferView controlSessionKey;
		openpal::ReadBufferView monitorSessionKey;
		openpal::ReadBufferView keyStatusObject;
	};

	class KeyUnwrapBuffer
	{
	public:

		bool Unwrap(
			openpal::IKeyWrapAlgo& algo,
			openpal::ReadBufferView updateKey,
			openpal::ReadBufferView inputData,			
			UnwrappedKeyData& output, 
			openpal::Logger* pLogger);

	private:

		// The takes into account the fields in the unwrapped data
		static const uint32_t MAX_REQUIRED_BUFFER_SIZE = 
			openpal::UInt16::Size + 
			2 * AuthConstants::MAX_SESSION_KEY_SIZE + 
			opendnp3::Group120Var5::FIXED_BASE_SIZE + 
			AuthConstants::MAX_CHALLENGE_DATA_SIZE;

		openpal::StaticBuffer<MAX_REQUIRED_BUFFER_SIZE> buffer;		
	};

}

#endif

