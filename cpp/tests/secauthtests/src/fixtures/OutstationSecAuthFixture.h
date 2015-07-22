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
#ifndef SECAUTH_OUTSTATION_SECAUTH_FIXTURE_H_
#define SECAUTH_OUTSTATION_SECAUTH_FIXTURE_H_

#include <opendnp3/LogLevels.h>
#include <opendnp3/app/ITransactable.h>
#include <opendnp3/outstation/Database.h>

#include <secauth/outstation/OutstationAuthContext.h>

#include <functional>

#include <testlib/MockExecutor.h>
#include <testlib/MockLogHandler.h>

#include <dnp3mocks/MockCommandHandler.h>
#include <dnp3mocks/MockLowerLayer.h>
#include <dnp3mocks/MockCryptoProvider.h>

#include "MockOutstationApplicationSA.h"


namespace opendnp3
{

class OutstationSecAuthFixture
{

public:
	OutstationSecAuthFixture(		
		const secauth::OutstationAuthSettings& authConfig = secauth::OutstationAuthSettings(),
		const DatabaseTemplate& dbTemplate = DatabaseTemplate::BinaryOnly(1),
		const OutstationConfig& config = OutstationConfig()		
	);	
														
	uint32_t SendToOutstation(const std::string& hex);	

	std::string SendAndReceive(const std::string& hex);

	uint32_t LowerLayerUp();

	uint32_t LowerLayerDown();

	uint32_t OnSendResult(bool isSuccess);

	size_t NumPendingTimers() const;

	bool AdvanceToNextTimer();

	uint32_t AdvanceTime(const openpal::TimeDuration& td);

	void AddUser(User user, uint8_t keyRepeat, UpdateKeyMode mode, secauth::Permissions permissions = secauth::Permissions::AllowAll())
	{
		context.AddUser(user, secauth::UpdateKey(keyRepeat, mode), permissions);
	}

	
	void SetMockKeyWrapData(KeyWrapAlgorithm keyWrap, const std::string& data);
	void TestSessionKeyChange(AppSeqNum& seq, User user, opendnp3::KeyWrapAlgorithm keyWrap, secauth::HMACMode hmacMode);

	testlib::MockLogHandler log;
	testlib::MockExecutor exe;
	MockLowerLayer lower;
	MockCommandHandler cmdHandler;
	MockOutstationApplicationSA application;	
	MockCryptoProvider crypto;	
	secauth::OAuthContext context;	
};


}

#endif
