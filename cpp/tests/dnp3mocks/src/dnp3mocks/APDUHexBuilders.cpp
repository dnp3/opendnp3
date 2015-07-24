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

#include <openpal/container/Buffer.h>

#include <testlib/HexConversions.h>
#include <testlib/BufferHelpers.h>

using namespace std;
using namespace openpal;
using namespace opendnp3;
using namespace testlib;

namespace hex
{
	std::string repeat(uint8_t value, uint16_t count)
	{
		Buffer buffer(count);
		buffer.GetWriteBufferView().SetAllTo(value);
		return ToHex(buffer.ToReadOnly());
	}

	std::string ClassTask(FunctionCode fc, uint8_t seq, const ClassField& field)
	{		
		Buffer buffer(DEFAULT_MAX_APDU_SIZE);
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
		Buffer buffer(DEFAULT_MAX_APDU_SIZE);
		APDURequest request(buffer.GetWriteBufferView());
		build::ClearRestartIIN(request, seq);
		return ToHex(request.ToReadOnly());
	}

	std::string MeasureDelay(uint8_t seq)
	{
		Buffer buffer(DEFAULT_MAX_APDU_SIZE);
		APDURequest request(buffer.GetWriteBufferView());
		build::MeasureDelay(request, seq);
		return ToHex(request.ToReadOnly());
	}

	std::string EmptyResponse(uint8_t seq, const opendnp3::IINField& iin)
	{
		Buffer buffer(DEFAULT_MAX_APDU_SIZE);
		APDUResponse response(buffer.GetWriteBufferView());
		response.SetFunction(FunctionCode::RESPONSE);
		response.SetControl(AppControlField(true, true, false, false, seq));
		response.SetIIN(iin);
		return ToHex(response.ToReadOnly());
	}

	std::string NullUnsolicited(uint8_t seq, const IINField& iin)
	{
		Buffer buffer(DEFAULT_MAX_APDU_SIZE);
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
		Buffer buffer(DEFAULT_MAX_APDU_SIZE);
		APDURequest apdu(buffer.GetWriteBufferView());
		apdu.SetControl(AppControlField(true, true, false, unsol, seq));
		apdu.SetFunction(FunctionCode::CONFIRM);
		return ToHex(apdu.ToReadOnly());
	}

	// ----------- sec auth -------------

	std::string RequestKeyStatus(uint8_t seq, uint16_t user)
	{
		Buffer buffer(DEFAULT_MAX_APDU_SIZE);
		APDURequest apdu(buffer.GetWriteBufferView());
		apdu.SetControl(AppControlField(true, true, false, false, seq));
		apdu.SetFunction(FunctionCode::AUTH_REQUEST);		
		Group120Var4 status;
		status.userNum = user;		
		apdu.GetWriter().WriteSingleValue<UInt8>(QualifierCode::UINT8_CNT, status);
		return ToHex(apdu.ToReadOnly());
	}

	std::string AuthErrorResponse(
		opendnp3::IINField iin,
		uint8_t appSeq,
		uint32_t challengeSeqNum,
		uint16_t user,
		uint16_t assocId,
		opendnp3::AuthErrorCode code,
		opendnp3::DNPTime timestamp,
		std::string hexErrorText)
	{
		Buffer buffer(DEFAULT_MAX_APDU_SIZE);
		APDUResponse apdu(buffer.GetWriteBufferView());

		apdu.SetControl(AppControlField(true, true, false, false, appSeq));
		apdu.SetFunction(FunctionCode::AUTH_RESPONSE);
		apdu.SetIIN(iin);

		HexSequence hexErrorTextBuff(hexErrorText);

		Group120Var7 error(
			challengeSeqNum,
			user,
			assocId,
			code,
			timestamp,
			hexErrorTextBuff.ToReadOnly()
			);

		apdu.GetWriter().WriteFreeFormat(error);

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
		Buffer buffer(DEFAULT_MAX_APDU_SIZE);
		APDUResponse apdu(buffer.GetWriteBufferView());

		apdu.SetControl(AppControlField(true, true, false, false, seq));
		apdu.SetFunction(FunctionCode::AUTH_RESPONSE);
		apdu.SetIIN(iin);

		HexSequence challengeBuff(challengeDataHex);

		Group120Var1 rsp(
			csq,
			user,
			hmacType,
			reason,
			challengeBuff.ToReadOnly()
		);

		apdu.GetWriter().WriteFreeFormat(rsp);

		return ToHex(apdu.ToReadOnly());
	}

	std::string ChallengeReply(
		uint8_t appSeq,
		uint32_t challengeSeqNum,
		uint16_t userNum,
		std::string hmacHex
		)
	{
		Buffer buffer(DEFAULT_MAX_APDU_SIZE);
		APDURequest apdu(buffer.GetWriteBufferView());

		apdu.SetControl(AppControlField(true, true, false, false, appSeq));
		apdu.SetFunction(FunctionCode::AUTH_REQUEST);

		HexSequence hmacBuff(hmacHex);

		Group120Var2 rsp(challengeSeqNum, userNum, hmacBuff.ToReadOnly());
			
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
		Buffer buffer(DEFAULT_MAX_APDU_SIZE);
		APDUResponse apdu(buffer.GetWriteBufferView());
		apdu.SetControl(AppControlField(true, true, false, false, seq));
		apdu.SetFunction(FunctionCode::AUTH_RESPONSE);
		apdu.SetIIN(iin);

		HexSequence challengeBuff(challenge);
		HexSequence hmacBuff(hmac);

		Group120Var5 rsp;
		rsp.keyChangeSeqNum = ksq;
		rsp.userNum = user;
		rsp.keyWrapAlgo = keyWrap;
		rsp.keyStatus = status;
		rsp.hmacAlgo = hmacType;
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
		Buffer buffer(DEFAULT_MAX_APDU_SIZE);
		APDURequest apdu(buffer.GetWriteBufferView());
		apdu.SetControl(AppControlField(true, true, false, false, seq));
		apdu.SetFunction(FunctionCode::AUTH_REQUEST);

		HexSequence keyBuffer(keyWrapData);
		
		Group120Var6 rsp;
		rsp.keyChangeSeqNum = ksq;
		rsp.userNum = user;
		rsp.keyWrapData = keyBuffer.ToReadOnly();		

		apdu.GetWriter().WriteFreeFormat(rsp);

		return ToHex(apdu.ToReadOnly());
	}

	std::string UserStatusChangeRequest(
		uint8_t seq,
		opendnp3::KeyChangeMethod keyChangeMethod,
		opendnp3::UserOperation userOperation,
		uint32_t statusChangeSeqNum,
		uint16_t userRole,
		uint16_t userRoleExpDays,
		const std::string& userName,
		const std::string& userPublicKeyHex,
		const std::string& certificationDataHex
		)
	{
		Buffer buffer(DEFAULT_MAX_APDU_SIZE);
		APDURequest apdu(buffer.GetWriteBufferView());
		apdu.SetControl(AppControlField(true, true, false, false, seq));
		apdu.SetFunction(FunctionCode::AUTH_REQUEST);

		ReadBufferView name(reinterpret_cast<const uint8_t*>(userName.c_str()), userName.size());
		HexSequence userPublicKeyBuffer(userPublicKeyHex);
		HexSequence certificationDataBuffer(certificationDataHex);

		Group120Var10 statusChange(
			keyChangeMethod,
			userOperation,
			statusChangeSeqNum,
			userRole,
			userRoleExpDays,
			name,
			userPublicKeyBuffer.ToReadOnly(),
			certificationDataBuffer.ToReadOnly()
		);

		
		apdu.GetWriter().WriteFreeFormat(statusChange);

		return ToHex(apdu.ToReadOnly());
	}


	std::string KeyWrapData(
		uint16_t keyLengthBytes,
		uint8_t keyRepeatValue,
		std::string keyStatusMsg
		)
	{
		Buffer key(keyLengthBytes);
		key.GetWriteBufferView().SetAllTo(keyRepeatValue);			
		auto keyHex = ToHex(key.ToReadOnly());
		HexSequence statusBuffer(keyStatusMsg);

		Buffer lengthBuff(2);
		auto lenDest = lengthBuff.GetWriteBufferView();
		UInt16::WriteBuffer(lenDest, keyLengthBytes);
		auto lengthHex = ToHex(lengthBuff.ToReadOnly());
		 
		return AppendHex({lengthHex, keyHex, keyHex, keyStatusMsg});
	}	

}


