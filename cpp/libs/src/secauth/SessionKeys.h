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
#ifndef SECAUTH_SESSIONKEYS_H
#define SECAUTH_SESSIONKEYS_H

#include "AuthSizes.h"
#include "SessionKeysView.h"

#include <openpal/crypto/ISecureRandom.h>

#include <openpal/crypto/SecureStaticBuffer.h>
#include <openpal/util/Uncopyable.h>

namespace secauth
{
class SessionKeySize
{
public:

	SessionKeySize(uint32_t size_) :
		size(AuthSizes::GetBoundedSessionKeySize(size_))
	{}

	operator uint32_t() const
	{
		return size;
	}

private:
	uint32_t size;
};

/// Stores bi-directional session key data in internal buffers
class SessionKeys : private openpal::Uncopyable
{
public:

	void SetKeys(const SessionKeysView& view);

	SessionKeysView GetView() const;

	void DeriveFrom(openpal::ISecureRandom& rs, const SessionKeySize& size, std::error_code& ec);

private:

	openpal::RSlice controlKey;
	openpal::RSlice monitorKey;

	openpal::SecureStaticBuffer<AuthSizes::MAX_SESSION_KEY_SIZE_BYTES> controlBuffer;
	openpal::SecureStaticBuffer<AuthSizes::MAX_SESSION_KEY_SIZE_BYTES> monitorBuffer;
};
}

#endif

