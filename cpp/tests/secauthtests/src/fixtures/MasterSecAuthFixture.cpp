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

#include "MasterSecAuthFixture.h"

#include <catch.hpp>

#include <testlib/BufferHelpers.h>
#include <dnp3mocks/APDUHexBuilders.h>

using namespace testlib;
using namespace secauth;

namespace opendnp3
{	
	const std::string MasterSecAuthFixture::MOCK_KEY_WRAP_DATA("DEADBEEF");

	MasterSecAuthFixture::MasterSecAuthFixture(const MasterParams& params, const MasterAuthSettings& authSettings, ITaskLock& lock) :
		log(),
		exe(),
		meas(),
		lower(log.root),
		application(),
		crypto(),		
		context(exe, log.root, lower, meas, application, params, lock, authSettings, crypto)					
	{
		
	}

	void MasterSecAuthFixture::SendToMaster(const std::string& hex)
	{
		HexSequence hs(hex);
		context.OnReceive(hs.ToRSlice());
	}

	bool MasterSecAuthFixture::ConfigureUser(opendnp3::User user, UpdateKeyMode mode, uint8_t keyRepeat)
	{		
		return context.AddUser(user, secauth::UpdateKey(keyRepeat, mode));
	}

	void MasterSecAuthFixture::TestRequestAndReply(const std::string& request, const std::string& response)
	{
		this->exe.RunMany();
		auto asdu = this->lower.PopWriteAsHex();
		REQUIRE(asdu == request);
		this->context.OnSendResult(true);
		this->SendToMaster(response);
		this->exe.RunMany();
	}

	void MasterSecAuthFixture::TestSessionKeyExchange(AppSeqNum& seq, User user)
	{
		this->crypto.aes128.hexOutput = MOCK_KEY_WRAP_DATA; // set mock key wrap data

		
		auto requestKeyStatus = hex::RequestKeyStatus(seq, user.GetId());
		auto keyStatusResponse = hex::KeyStatusResponse(
			IINField::Empty(),
			seq,
			0, // ksq
			user.GetId(),
			KeyWrapAlgorithm::AES_128,
			KeyStatus::NOT_INIT,
			HMACType::HMAC_SHA1_TRUNC_10,
			hex::repeat(0xFF, 4), // challenge
			"" // no hmac
			);

		this->TestRequestAndReply(requestKeyStatus, keyStatusResponse);

		seq.Increment();

		auto keyChangeRequest = hex::KeyChangeRequest(seq, 0, user.GetId(), MOCK_KEY_WRAP_DATA);
		auto finalKeyStatusResponse = hex::KeyStatusResponse(
			IINField::Empty(),
			seq,
			0, // ksq
			user.GetId(),
			KeyWrapAlgorithm::AES_128,
			KeyStatus::OK,
			HMACType::HMAC_SHA1_TRUNC_10,
			hex::repeat(0xFF, 4),	// challenge
			hex::repeat(0xFF, 10) // hmac
		);		

		this->TestRequestAndReply(keyChangeRequest, finalKeyStatusResponse);

		seq.Increment();
	}

}



