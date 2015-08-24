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

#include "IOAuthState.h"

#include <opendnp3/LogLevels.h>

#include <openpal/logging/LogMacros.h>

using namespace opendnp3;

namespace secauth
{

	IOAuthState* IOAuthState::IgnoreRegularRequest(openpal::Logger& logger)
	{
		FORMAT_LOG_BLOCK(logger, flags::WARN, "AuthState: %s - Ignoring regular request", this->GetName());
		return this;
	}

	IOAuthState* IOAuthState::IgnoreAggModeRequest(openpal::Logger& logger)
	{
		FORMAT_LOG_BLOCK(logger, flags::WARN, "AuthState: %s - Ignoring Aggressive mode request", this->GetName());
		return this;
	}

	IOAuthState* IOAuthState::IgnoreAuthChallenge(openpal::Logger& logger)
	{
		FORMAT_LOG_BLOCK(logger, flags::WARN, "AuthState: %s - Ignoring Auth challenge", this->GetName());
		return this;
	}

	IOAuthState* IOAuthState::IgnoreAuthReply(openpal::Logger& logger)
	{
		FORMAT_LOG_BLOCK(logger, flags::WARN, "AuthState: %s - Ignoring Auth reply", this->GetName());
		return this;
	}

	IOAuthState* IOAuthState::IgnoreRequestKeyStatus(openpal::Logger& logger, uint16_t user)
	{
		FORMAT_LOG_BLOCK(logger, flags::WARN, "AuthState: %s - Ignoring key status request for user %u", this->GetName(), user);
		return this;
	}

	IOAuthState* IOAuthState::IgnoreChangeSessionKeys(openpal::Logger& logger, uint16_t user)
	{
		FORMAT_LOG_BLOCK(logger, flags::WARN, "AuthState: %s - Ignoring change session keys for user %u", this->GetName(), user);
		return this;
	}

	IOAuthState* IOAuthState::IgnoreChallengeTimeout(openpal::Logger& logger)
	{
		FORMAT_LOG_BLOCK(logger, flags::WARN, "AuthState: %s - Ignoring challenge timeout", this->GetName());
		return this;
	}

}

