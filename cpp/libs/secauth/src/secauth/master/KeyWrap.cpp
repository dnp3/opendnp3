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

#include "KeyWrap.h"

#include <openpal/logging/LogMacros.h>

#include <opendnp3/LogLevels.h>

#include <openpal/serialization/Serialization.h>

using namespace opendnp3;
using namespace openpal;

namespace secauth
{

bool KeyWrapBuffer::Wrap(
	openpal::IKeyWrapAlgo& algo,
	const openpal::ReadBufferView& updateKey,
	const SessionKeysView& sessionKeys,
	const openpal::ReadBufferView& keyStatus,
	openpal::Logger logger
	)
{
	if (!sessionKeys.IsValid())
	{
		SIMPLE_LOG_BLOCK(logger, flags::ERR, "Cannot wrap invalid session keys");
		return false;
	}

	const uint32_t WRAPPED_DATA_SIZE = 2 + sessionKeys.TotalSize() + keyStatus.Size();
	const uint32_t WRAPPED_DATA_SIZE_MOD8 = WRAPPED_DATA_SIZE % 8;
	const uint32_t WRAPPED_DATA_SIZE_WITH_PADDING = (WRAPPED_DATA_SIZE_MOD8 == 0) ? WRAPPED_DATA_SIZE : (WRAPPED_DATA_SIZE + (8 - WRAPPED_DATA_SIZE_MOD8));

	StaticBuffer<AuthConstants::MAX_KEY_WRAP_BUFFER_SIZE> dataToWrap;

	auto dest = dataToWrap.GetWriteBuffer(WRAPPED_DATA_SIZE_WITH_PADDING);

	// write the fields
	UInt16::WriteBuffer(dest, static_cast<uint16_t>(sessionKeys.controlKey.Size()));
	sessionKeys.controlKey.CopyTo(dest);
	sessionKeys.monitorKey.CopyTo(dest);
	keyStatus.CopyTo(dest);

	// set the remainder to a padding value of 0
	dest.SetAllTo(0);

	// a handle to what we just wrote
	auto input = dataToWrap.ToReadOnly().Take(WRAPPED_DATA_SIZE_WITH_PADDING);
	auto output = this->buffer.GetWriteBuffer();

	// save the view of the wrapped data internally
	std::error_code ec;

	this->data = algo.WrapKey(updateKey, input, output, ec);

	if (ec)
	{
		SIMPLE_LOG_BLOCK(logger, flags::ERR, ec.message().c_str());
		return false;
	}
	
	return data.IsNotEmpty();
}


}


