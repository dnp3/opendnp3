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
#ifndef SECAUTH_MASTERSECAUTHFIXTURE_H_
#define SECAUTH_MASTERSECAUTHFIXTURE_H_

#include <testlib/MockExecutor.h>
#include <testlib/MockLogHandler.h>

#include <opendnp3/master/Master.h>
#include <opendnp3/LogLevels.h>

#include <dnp3mocks/MockLowerLayer.h>
#include <dnp3mocks/MockSOEHandler.h>
#include <dnp3mocks/MockMasterApplication.h>
#include <dnp3mocks/MockCryptoProvider.h>

#include <secauth/master/MasterAuthContext.h>
#include <secauth/master/SimpleMasterUserDatabase.h>

namespace opendnp3
{

	MasterParams NoStartupTasks();

	class MasterSecAuthFixture
	{
	public:

		MasterSecAuthFixture(const MasterParams& params, const secauth::MasterAuthSettings& authSettings = secauth::MasterAuthSettings(), ITaskLock& lock = NullTaskLock::Instance());

		void SendToMaster(const std::string& hex);		

		bool ConfigureUser(opendnp3::User user, UpdateKeyMode mode = UpdateKeyMode::AES128, uint8_t keyRepeat = 0xFF);

		testlib::MockLogHandler log;
		testlib::MockExecutor exe;
		MockSOEHandler meas;
		MockLowerLayer lower;
		MockMasterApplication application;
		MockCryptoProvider crypto;
		secauth::SimpleMasterUserDatabase userDB;
		secauth::MAuthContext context;
		Master master;

		
	};

}

#endif

