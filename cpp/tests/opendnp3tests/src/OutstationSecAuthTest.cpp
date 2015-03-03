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

#include "OutstationSecAuthTest.h"

#include <testlib/BufferHelpers.h>

using namespace secauthv5;

namespace opendnp3
{
	OutstationSecAuthTest::OutstationSecAuthTest(
		const secauthv5::OutstationAuthSettings& authConfig,
		const OutstationConfig& config
	) : 
		log(),
		exe(),
		lower(log.root),
		cmdHandler(),
		application(),
		utc(),
		users(),
		crypto(),		
		auth(authConfig, log.GetLogger(), exe, utc, users, crypto),
		outstation(config, DatabaseTemplate::BinaryOnly(1), log.GetLogger(), exe, lower, cmdHandler, application, &auth)
	{
		lower.SetUpperLayer(outstation);
	}

	void OutstationSecAuthTest::AddUser(secauthv5::User, secauthv5::UpdateKeyMode mode, uint8_t keyRepeat)
	{
		auto keySize = (mode == UpdateKeyMode::AES128) ? 16 : 32;
		openpal::DynamicBuffer key(keySize);
		key.GetWriteBufferView().SetAllTo(keyRepeat);
		users.ConfigureUser(
			secauthv5::User::Default(),
			mode,
			key.ToReadOnly()
		);		
	}

	uint32_t OutstationSecAuthTest::LowerLayerUp()
	{
		outstation.OnLowerLayerUp();
		return exe.RunMany();
	}

	uint32_t OutstationSecAuthTest::LowerLayerDown()
	{
		outstation.OnLowerLayerDown();
		return exe.RunMany();
	}

	uint32_t OutstationSecAuthTest::OnSendResult(bool isSuccess)
	{
		outstation.OnSendResult(isSuccess);
		return exe.RunMany();
	}

	uint32_t OutstationSecAuthTest::SendToOutstation(const std::string& hex)
	{
		testlib::HexSequence hs(hex);
		outstation.OnReceive(hs.ToReadOnly());
		return exe.RunMany();
	}

	size_t OutstationSecAuthTest::NumPendingTimers() const
	{
		return exe.NumPendingTimers();
	}

	bool OutstationSecAuthTest::AdvanceToNextTimer()
	{
		if (exe.AdvanceToNextTimer())
		{
			return exe.RunMany() > 0;
		}
		else
		{
			return false;
		}
	}

	uint32_t OutstationSecAuthTest::AdvanceTime(const openpal::TimeDuration& td)
	{
		exe.AdvanceTime(td);
		return exe.RunMany();
	}
}


