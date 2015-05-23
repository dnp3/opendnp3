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
#ifndef __OUTSTATION_SECAUTH_TEST_H_
#define __OUTSTATION_SECAUTH_TEST_H_

#include <opendnp3/LogLevels.h>
#include <opendnp3/app/ITransactable.h>
#include <opendnp3/outstation/Outstation.h>
#include <opendnp3/outstation/Database.h>

#include <secauthv5/outstation/OutstationAuthProvider.h>
#include <secauthv5/SimpleUserDatabase.h>

#include <functional>

#include <testlib/MockExecutor.h>
#include <testlib/MockLogHandler.h>
#include <testlib/MockUTCTimeSource.h>

#include <dnp3mocks/MockCommandHandler.h>
#include <dnp3mocks/MockLowerLayer.h>
#include <dnp3mocks/MockCryptoProvider.h>
#include <dnp3mocks/MockOutstationApplication.h>


namespace opendnp3
{

class OutstationSecAuthTest
{

public:
	OutstationSecAuthTest(
		const secauthv5::OutstationAuthSettings& authConfig = secauthv5::OutstationAuthSettings(),
		const OutstationConfig& config = OutstationConfig()		
	);

	void AddUser(secauthv5::User, secauthv5::UpdateKeyMode mode, uint8_t keyRepeat);
														
	uint32_t SendToOutstation(const std::string& hex);

	uint32_t LowerLayerUp();

	uint32_t LowerLayerDown();

	uint32_t OnSendResult(bool isSuccess);

	size_t NumPendingTimers() const;

	bool AdvanceToNextTimer();

	uint32_t AdvanceTime(const openpal::TimeDuration& td);

	testlib::MockLogHandler log;
	testlib::MockExecutor exe;
	MockLowerLayer lower;
	MockCommandHandler cmdHandler;
	MockOutstationApplication application;
	testlib::MockUTCTimeSource utc;
	secauthv5::SimpleUserDatabase users;
	MockCryptoProvider crypto;	
	secauthv5::OutstationAuthProvider auth;
	Outstation outstation;	
};


}

#endif
