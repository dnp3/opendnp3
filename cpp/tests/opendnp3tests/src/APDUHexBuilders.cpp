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

#include "APDUHexBuilders.h"

#include <opendnp3/app/APDURequest.h>
#include <opendnp3/app/APDUResponse.h>
#include <opendnp3/app/APDUBuilders.h>
#include <opendnp3/app/AppConstants.h>

#include <opendnp3/objects/Group120.h>
#include <opendnp3/objects/Group120Var1.h>
#include <opendnp3/objects/Group120Var5.h>
#include <opendnp3/objects/Group120Var6.h>

#include <openpal/container/DynamicBuffer.h>

#include <testlib/HexConversions.h>
#include <testlib/BufferHelpers.h>

using namespace std;
using namespace openpal;
using namespace opendnp3;
using namespace testlib;

namespace hex
{
	std::string ClassTask(FunctionCode fc, uint8_t seq, const ClassField& field)
	{		
		DynamicBuffer buffer(DEFAULT_MAX_APDU_SIZE);
		APDURequest request(buffer.GetWriteBufferView());
		opendnp3::build::ClassRequest(request, fc, field, seq);
		return ToHex(request.ToReadOnly());
	}

	std::string IntegrityPoll(uint8_t seq, const ClassField& field)
	{
		return ClassTask(FunctionCode::READ, seq, field);
	}

	std::string ClassPoll(uint8_t seq, PointClass pc)
	{
		return ClassTask(FunctionCode::READ, seq, ClassField(pc));
	}

	std::string EventPoll(uint8_t seq, const ClassField& field)
	{
		return ClassTask(FunctionCode::READ, seq, field);
	}

	std::string ClearRestartIIN(uint8_t seq)
	{
		DynamicBuffer buffer(DEFAULT_MAX_APDU_SIZE);
		APDURequest request(buffer.GetWriteBufferView());
		build::ClearRestartIIN(request, seq);
		return ToHex(request.ToReadOnly());
	}

	std::string MeasureDelay(uint8_t seq)
	{
		DynamicBuffer buffer(DEFAULT_MAX_APDU_SIZE);
		APDURequest request(buffer.GetWriteBufferView());
		build::MeasureDelay(request, seq);
		return ToHex(request.ToReadOnly());
	}

	std::string EmptyResponse(uint8_t seq, const opendnp3::IINField& iin)
	{
		DynamicBuffer buffer(DEFAULT_MAX_APDU_SIZE);
		APDUResponse response(buffer.GetWriteBufferView());
		response.SetFunction(FunctionCode::RESPONSE);
		response.SetControl(AppControlField(true, true, false, false, seq));
		response.SetIIN(iin);
		return ToHex(response.ToReadOnly());
	}

	std::string NullUnsolicited(uint8_t seq, const IINField& iin)
	{
		DynamicBuffer buffer(DEFAULT_MAX_APDU_SIZE);
		APDUResponse response(buffer.GetWriteBufferView());
		build::NullUnsolicited(response, seq, iin);
		return ToHex(response.ToReadOnly());
	}	

	std::string SolicitedConfirm(uint8_t seq)
	{
		return Confirm(seq, false);
	}

	std::string UnsolConfirm(uint8_t seq)
	{
		return Confirm(seq, true);
	}

	std::string Confirm(uint8_t seq, bool unsol)
	{
		DynamicBuffer buffer(DEFAULT_MAX_APDU_SIZE);
		APDURequest apdu(buffer.GetWriteBufferView());
		apdu.SetControl(AppControlField(true, true, false, unsol, seq));
		apdu.SetFunction(FunctionCode::CONFIRM);
		return ToHex(apdu.ToReadOnly());
	}

	// ----------- sec auth -------------

	std::string RequestKeyStatus(uint8_t seq, uint16_t user)
	{
		DynamicBuffer buffer(DEFAULT_MAX_APDU_SIZE);
		APDURequest apdu(buffer.GetWriteBufferView());
		apdu.SetControl(AppControlField(true, true, false, false, seq));
		apdu.SetFunction(FunctionCode::AUTH_REQUEST);		
		Group120Var4 status;
		status.userNum = user;		
		apdu.GetWriter().WriteSingleValue<UInt8>(QualifierCode::UINT8_CNT, status);
		return ToHex(apdu.ToReadOnly());
	}

	std::string ChallengeResponse(
		opendnp3::IINField iin,
		uint8_t seq,
		uint32_t csq,
		uint16_t user,
		HMACType hmacType,
		ChallengeReason reason,
		std::string challengeDataHex
		)
	{
		DynamicBuffer buffer(DEFAULT_MAX_APDU_SIZE);
		APDUResponse apdu(buffer.GetWriteBufferView());

		apdu.SetControl(AppControlField(true, true, false, false, seq));
		apdu.SetFunction(FunctionCode::AUTH_RESPONSE);
		apdu.SetIIN(iin);

		HexSequence challengeBuff(challengeDataHex);

		Group120Var1Def rsp(
			csq,
			user,
			hmacType,
			reason,
			challengeBuff.ToReadOnly()
		);

		apdu.GetWriter().WriteFreeFormat(rsp);

		return ToHex(apdu.ToReadOnly());
	}

	std::string KeyStatusResponse(
		IINField iin,
		uint8_t seq,
		uint32_t ksq,
		uint16_t user,
		KeyWrapAlgorithm keyWrap,
		KeyStatus status,
		HMACType hmacType,
		const std::string& challenge,
		const std::string& hmac
		)
	{
		DynamicBuffer buffer(DEFAULT_MAX_APDU_SIZE);
		APDUResponse apdu(buffer.GetWriteBufferView());
		apdu.SetControl(AppControlField(true, true, false, false, seq));
		apdu.SetFunction(FunctionCode::AUTH_RESPONSE);
		apdu.SetIIN(iin);

		HexSequence challengeBuff(challenge);
		HexSequence hmacBuff(hmac);

		Group120Var5Def rsp;
		rsp.keyChangeSeqNum = ksq;
		rsp.userNum = user;
		rsp.keywrapAlgorithm = keyWrap;
		rsp.keyStatus = status;
		rsp.hmacType = hmacType;
		rsp.challengeData = challengeBuff.ToReadOnly();
		rsp.hmacValue = hmacBuff.ToReadOnly();
		
		apdu.GetWriter().WriteFreeFormat(rsp);

		return ToHex(apdu.ToReadOnly());
	}

	std::string KeyChangeRequest(
		uint8_t seq,
		uint32_t ksq,
		uint16_t user,
		const std::string& keyWrapData
		)
	{
		DynamicBuffer buffer(DEFAULT_MAX_APDU_SIZE);
		APDURequest apdu(buffer.GetWriteBufferView());
		apdu.SetControl(AppControlField(true, true, false, false, seq));
		apdu.SetFunction(FunctionCode::AUTH_REQUEST);

		HexSequence keyBuffer(keyWrapData);
		
		Group120Var6Def rsp;
		rsp.seq = ksq;
		rsp.user = user;
		rsp.data = keyBuffer.ToReadOnly();		

		apdu.GetWriter().WriteFreeFormat(rsp);

		return ToHex(apdu.ToReadOnly());
	}


	std::string KeyWrapData(
		uint16_t keyLengthBytes,
		uint8_t keyRepeatValue,
		std::string keyStatusMsg
		)
	{
		DynamicBuffer key(keyLengthBytes);
		key.GetWriteBufferView().SetAllTo(keyRepeatValue);			
		auto keyHex = ToHex(key.ToReadOnly());
		HexSequence statusBuffer(keyStatusMsg);

		DynamicBuffer lengthBuff(2);
		auto lenDest = lengthBuff.GetWriteBufferView();
		UInt16::WriteBuffer(lenDest, keyLengthBytes);
		auto lengthHex = ToHex(lengthBuff.ToReadOnly());
		 
		return AppendHex({lengthHex, keyHex, keyHex, keyStatusMsg});
	}	

}


