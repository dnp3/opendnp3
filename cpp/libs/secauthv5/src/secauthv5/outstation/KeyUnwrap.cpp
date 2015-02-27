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

#include "KeyUnwrap.h"

#include <openpal/logging/LogMacros.h>
#include <openpal/logging/LogLevels.h>

using namespace openpal;



namespace secauthv5
{
	bool KeyUnwrapBuffer::Unwrap(
		openpal::IKeyWrapAlgo& algo,
		openpal::ReadBufferView updateKey,
		openpal::ReadBufferView inputData,
		UnwrappedKeyData& output,
		openpal::Logger* pLogger)
	{		
		auto dest = buffer.GetWriteBuffer();
		auto unwrapped = algo.UnwrapKey(updateKey, inputData, dest, pLogger);

		if (unwrapped.IsEmpty())
		{
			return false;
		}

		if (unwrapped.Size() < UInt16::SIZE)
		{
			SIMPLE_LOGGER_BLOCK(pLogger, logflags::WARN, "Not enough data for key length");
			return false;
		}

		uint16_t keyLength = UInt16::ReadBuffer(unwrapped);
		
		if (!AuthConstants::SessionKeySizeWithinLimits(keyLength))
		{
			SIMPLE_LOGGER_BLOCK(pLogger, logflags::WARN, "Session key size of %u not within limits");
			return false;
		}

		const uint32_t REQUIRED_KEY_SIZE = 2 * keyLength;

		if (unwrapped.Size() < REQUIRED_KEY_SIZE)
		{
			SIMPLE_LOGGER_BLOCK(pLogger, logflags::WARN, "Not enough data for session keys");
			return false;
		}

		auto controlKey = unwrapped.Take(keyLength);
		unwrapped.Advance(keyLength);

		auto monitorKey = unwrapped.Take(keyLength);
		unwrapped.Advance(keyLength);

		output.keys = SessionKeysView(controlKey, monitorKey);
		
		// anything left over is the key status message
		output.keyStatusObject = unwrapped;
		return true;
	}
}



