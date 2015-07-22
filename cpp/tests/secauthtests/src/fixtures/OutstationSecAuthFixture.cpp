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

#include "OutstationSecAuthFixture.h"

#include <catch.hpp>

#include <dnp3mocks/APDUHexBuilders.h>

#include <testlib/BufferHelpers.h>
#include <testlib/HexConversions.h>

using namespace secauth;
using namespace testlib;

namespace opendnp3
{
	OutstationSecAuthFixture::OutstationSecAuthFixture(		
		const secauth::OutstationAuthSettings& authConfig,
		const DatabaseTemplate& dbTemplate,
		const OutstationConfig& config
	) : 
		log(),
		exe(),
		lower(log.root),
		cmdHandler(),
		application(),		
		crypto(),		
		context(config, dbTemplate, log.GetLogger(), exe, lower, cmdHandler, application, authConfig, crypto)		
	{
		lower.SetUpperLayer(context);
	}

	uint32_t OutstationSecAuthFixture::LowerLayerUp()
	{
		context.OnLowerLayerUp();
		return exe.RunMany();
	}

	uint32_t OutstationSecAuthFixture::LowerLayerDown()
	{
		context.OnLowerLayerDown();
		return exe.RunMany();
	}

	uint32_t OutstationSecAuthFixture::OnSendResult(bool isSuccess)
	{
		context.OnSendResult(isSuccess);
		return exe.RunMany();
	}

	uint32_t OutstationSecAuthFixture::SendToOutstation(const std::string& hex)
	{
		testlib::HexSequence hs(hex);
		context.OnReceive(hs.ToReadOnly());
		return exe.RunMany();
	}

	std::string OutstationSecAuthFixture::SendAndReceive(const std::string& hex)
	{
		this->SendToOutstation(hex);		
		auto ret = this->lower.PopWriteAsHex();
		if (!ret.empty())
		{
			this->OnSendResult(true);
		}
		return ret;
	}

	size_t OutstationSecAuthFixture::NumPendingTimers() const
	{
		return exe.NumPendingTimers();
	}

	bool OutstationSecAuthFixture::AdvanceToNextTimer()
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

	uint32_t OutstationSecAuthFixture::AdvanceTime(const openpal::TimeDuration& td)
	{
		exe.AdvanceTime(td);
		return exe.RunMany();
	}

	void OutstationSecAuthFixture::SetMockKeyWrapData(KeyWrapAlgorithm keyWrap, const std::string& data)
	{
		switch (keyWrap)
		{
		case(KeyWrapAlgorithm::AES_128) :
			crypto.aes128.hexOutput = hex::KeyWrapData(16, 0xBB, data);
			break;
		case(KeyWrapAlgorithm::AES_256) :
			crypto.aes256.hexOutput = hex::KeyWrapData(32, 0xBB, data);
			break;
		default:
			throw std::logic_error("bad param");
		}
	}

	void OutstationSecAuthFixture::TestSessionKeyChange(AppSeqNum& seq, User user, KeyWrapAlgorithm keyWrap, HMACMode hmacMode)
	{
		REQUIRE(this->lower.HasNoData());		

		auto keyStatusRequest = hex::RequestKeyStatus(seq, user.GetId());
		auto keyStatusRsp = hex::KeyStatusResponse(
			IINBit::DEVICE_RESTART,
			seq,
			1, // ksq
			user.GetId(),
			keyWrap,
			KeyStatus::NOT_INIT,
			HMACType::NO_MAC_VALUE,
			hex::repeat(0xAA, 4), // challenge
			"");  // no hmac

		REQUIRE(this->SendAndReceive(keyStatusRequest) == keyStatusRsp);	
		REQUIRE(this->lower.HasNoData());

		this->SetMockKeyWrapData(keyWrap, SkipBytesHex(keyStatusRsp, 10));

		auto keyChangeRequest = hex::KeyChangeRequest(seq, 1, 1, "DE AD BE EF");
		auto keyStatusRspFinal = hex::KeyStatusResponse(
			IINBit::DEVICE_RESTART,
			seq, // seq
			2, // ksq
			user.GetId(), // user
			keyWrap,
			KeyStatus::OK,
			ToHMACType(hmacMode),
			hex::repeat(0xAA, 4), // challenge
			RepeatHex(0xFF, GetTruncationSize(hmacMode)));  // fixed value from hmac mock

		// --- session key change request ---	
		// the key wrap data doesn't matter b/c we've mocked the unwrap call above
		REQUIRE(this->SendAndReceive(keyChangeRequest) == keyStatusRspFinal);				
		REQUIRE(this->lower.HasNoData());
	}
}


