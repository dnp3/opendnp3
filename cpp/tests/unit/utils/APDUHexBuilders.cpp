/*
 * Copyright 2013-2019 Automatak, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
 * LLC (www.automatak.com) under one or more contributor license agreements.
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Automatak LLC license
 * this file to you under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License. You may obtain
 * a copy of the License at:
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "utils/APDUHexBuilders.h"

#include <ser4cpp/container/Buffer.h>
#include <ser4cpp/util/HexConversions.h>

#include <app/APDUBuilders.h>
#include <app/APDURequest.h>
#include <app/APDUResponse.h>
#include <gen/objects/Group12.h>
#include <gen/objects/Group120.h>
#include <opendnp3/app/AppConstants.h>

#include "utils/BufferHelpers.h"

using namespace opendnp3;
using namespace ser4cpp;

namespace hex
{

std::string repeat(uint8_t value, uint16_t count)
{
    Buffer buffer(count);
    buffer.as_wslice().set_all_to(value);
    return HexConversions::to_hex(buffer.as_rslice());
}

std::string ClassTask(FunctionCode fc, uint8_t seq, const ClassField& field)
{
    Buffer buffer(DEFAULT_MAX_APDU_SIZE);
    APDURequest request(buffer.as_wslice());
    opendnp3::build::ClassRequest(request, fc, field, seq);
    return HexConversions::to_hex(request.ToRSeq());
}

std::string DisableUnsol(uint8_t seq, const opendnp3::ClassField& field)
{
    return ClassTask(FunctionCode::DISABLE_UNSOLICITED, seq, field);
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
    APDURequest request(buffer.as_wslice());
    build::ClearRestartIIN(request, seq);
    return HexConversions::to_hex(request.ToRSeq());
}

std::string MeasureDelay(uint8_t seq)
{
    Buffer buffer(DEFAULT_MAX_APDU_SIZE);
    APDURequest request(buffer.as_wslice());
    build::MeasureDelay(request, seq);
    return HexConversions::to_hex(request.ToRSeq());
}

std::string RecordCurrentTime(uint8_t seq)
{
    Buffer buffer(DEFAULT_MAX_APDU_SIZE);
    APDURequest request(buffer.as_wslice());
    build::RecordCurrentTime(request, seq);
    return HexConversions::to_hex(request.ToRSeq());
}

std::string Control(opendnp3::FunctionCode code,
                    uint8_t seq,
                    const opendnp3::ControlRelayOutputBlock& crob,
                    uint16_t index)
{
    Buffer buffer(DEFAULT_MAX_APDU_SIZE);
    APDURequest request(buffer.as_wslice());

    request.SetControl(AppControlField::Request(seq));
    request.SetFunction(code);

    auto writer = request.GetWriter();
    writer.WriteSingleIndexedValue<UInt16, ControlRelayOutputBlock>(QualifierCode::UINT16_CNT_UINT16_INDEX,
                                                                    Group12Var1::Inst(), crob, index);

    return HexConversions::to_hex(request.ToRSeq());
}

std::string EmptyResponse(uint8_t seq, const opendnp3::IINField& iin)
{
    Buffer buffer(DEFAULT_MAX_APDU_SIZE);
    APDUResponse response(buffer.as_wslice());
    response.SetFunction(FunctionCode::RESPONSE);
    response.SetControl(AppControlField(true, true, false, false, seq));
    response.SetIIN(iin);
    return HexConversions::to_hex(response.ToRSeq());
}

std::string EmptyAuthResponse(uint8_t seq, const opendnp3::IINField& iin)
{
    Buffer buffer(DEFAULT_MAX_APDU_SIZE);
    APDUResponse response(buffer.as_wslice());
    response.SetFunction(FunctionCode::AUTH_RESPONSE);
    response.SetControl(AppControlField(true, true, false, false, seq));
    response.SetIIN(iin);
    return HexConversions::to_hex(response.ToRSeq());
}

std::string NullUnsolicited(uint8_t seq, const IINField& iin)
{
    Buffer buffer(DEFAULT_MAX_APDU_SIZE);
    APDUResponse response(buffer.as_wslice());
    build::NullUnsolicited(response, seq, iin);
    return HexConversions::to_hex(response.ToRSeq());
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
    APDURequest apdu(buffer.as_wslice());
    apdu.SetControl(AppControlField(true, true, false, unsol, seq));
    apdu.SetFunction(FunctionCode::CONFIRM);
    return HexConversions::to_hex(apdu.ToRSeq());
}

// ----------- sec auth -------------

std::string RequestKeyStatus(uint8_t seq, uint16_t user)
{
    Buffer buffer(DEFAULT_MAX_APDU_SIZE);
    APDURequest apdu(buffer.as_wslice());
    apdu.SetControl(AppControlField(true, true, false, false, seq));
    apdu.SetFunction(FunctionCode::AUTH_REQUEST);
    Group120Var4 status;
    status.userNum = user;
    apdu.GetWriter().WriteSingleValue<UInt8>(QualifierCode::UINT8_CNT, status);
    return HexConversions::to_hex(apdu.ToRSeq());
}

std::string AuthErrorResponse(opendnp3::IINField iin,
                              uint8_t appSeq,
                              uint32_t challengeSeqNum,
                              uint16_t user,
                              uint16_t assocId,
                              opendnp3::AuthErrorCode code,
                              opendnp3::DNPTime timestamp,
                              const std::string& hexErrorText)
{
    Buffer buffer(DEFAULT_MAX_APDU_SIZE);
    APDUResponse apdu(buffer.as_wslice());

    apdu.SetControl(AppControlField(true, true, false, false, appSeq));
    apdu.SetFunction(FunctionCode::AUTH_RESPONSE);
    apdu.SetIIN(iin);

    HexSequence hexErrorTextBuff(hexErrorText);

    Group120Var7 error(challengeSeqNum, user, assocId, code, timestamp, hexErrorTextBuff.ToRSeq());

    apdu.GetWriter().WriteFreeFormat(error);

    return HexConversions::to_hex(apdu.ToRSeq());
}

std::string ChallengeResponse(opendnp3::IINField iin,
                              uint8_t seq,
                              uint32_t csq,
                              uint16_t user,
                              HMACType hmacType,
                              ChallengeReason reason,
                              const std::string& challengeDataHex)
{
    Buffer buffer(DEFAULT_MAX_APDU_SIZE);
    APDUResponse apdu(buffer.as_wslice());

    apdu.SetControl(AppControlField(true, true, false, false, seq));
    apdu.SetFunction(FunctionCode::AUTH_RESPONSE);
    apdu.SetIIN(iin);

    HexSequence challengeBuff(challengeDataHex);

    Group120Var1 rsp(csq, user, hmacType, reason, challengeBuff.ToRSeq());

    apdu.GetWriter().WriteFreeFormat(rsp);

    return HexConversions::to_hex(apdu.ToRSeq());
}

std::string ChallengeReply(uint8_t appSeq, uint32_t challengeSeqNum, uint16_t userNum, const std::string& hmacHex)
{
    Buffer buffer(DEFAULT_MAX_APDU_SIZE);
    APDURequest apdu(buffer.as_wslice());

    apdu.SetControl(AppControlField(true, true, false, false, appSeq));
    apdu.SetFunction(FunctionCode::AUTH_REQUEST);

    HexSequence hmacBuff(hmacHex);

    Group120Var2 rsp(challengeSeqNum, userNum, hmacBuff.ToRSeq());

    apdu.GetWriter().WriteFreeFormat(rsp);

    return HexConversions::to_hex(apdu.ToRSeq());
}

std::string KeyStatusResponse(IINField iin,
                              uint8_t seq,
                              uint32_t ksq,
                              uint16_t user,
                              KeyWrapAlgorithm keyWrap,
                              KeyStatus status,
                              HMACType hmacType,
                              const std::string& challenge,
                              const std::string& hmac)
{
    Buffer buffer(DEFAULT_MAX_APDU_SIZE);
    APDUResponse apdu(buffer.as_wslice());
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
    rsp.challengeData = challengeBuff.ToRSeq();
    rsp.hmacValue = hmacBuff.ToRSeq();

    apdu.GetWriter().WriteFreeFormat(rsp);

    return HexConversions::to_hex(apdu.ToRSeq());
}

std::string KeyChangeRequest(uint8_t seq, uint32_t ksq, uint16_t user, const std::string& keyWrapData)
{
    Buffer buffer(DEFAULT_MAX_APDU_SIZE);
    APDURequest apdu(buffer.as_wslice());
    apdu.SetControl(AppControlField(true, true, false, false, seq));
    apdu.SetFunction(FunctionCode::AUTH_REQUEST);

    HexSequence keyBuffer(keyWrapData);

    Group120Var6 rsp;
    rsp.keyChangeSeqNum = ksq;
    rsp.userNum = user;
    rsp.keyWrapData = keyBuffer.ToRSeq();

    apdu.GetWriter().WriteFreeFormat(rsp);

    return HexConversions::to_hex(apdu.ToRSeq());
}

std::string UserStatusChangeRequest(uint8_t seq,
                                    opendnp3::KeyChangeMethod keyChangeMethod,
                                    opendnp3::UserOperation userOperation,
                                    uint32_t statusChangeSeqNum,
                                    uint16_t userRole,
                                    uint16_t userRoleExpDays,
                                    const std::string& userName,
                                    const std::string& userPublicKeyHex,
                                    const std::string& certificationDataHex)
{
    Buffer buffer(DEFAULT_MAX_APDU_SIZE);
    APDURequest apdu(buffer.as_wslice());
    apdu.SetControl(AppControlField(true, true, false, false, seq));
    apdu.SetFunction(FunctionCode::AUTH_REQUEST);

    rseq_t name(reinterpret_cast<const uint8_t*>(userName.c_str()), static_cast<uint32_t>(userName.size()));
    HexSequence userPublicKeyBuffer(userPublicKeyHex);
    HexSequence certificationDataBuffer(certificationDataHex);

    Group120Var10 statusChange(keyChangeMethod, userOperation, statusChangeSeqNum, userRole, userRoleExpDays, name,
                               userPublicKeyBuffer.ToRSeq(), certificationDataBuffer.ToRSeq());

    apdu.GetWriter().WriteFreeFormat(statusChange);

    return HexConversions::to_hex(apdu.ToRSeq());
}

std::string BeginUpdateKeyChangeRequest(uint8_t seq,
                                        opendnp3::KeyChangeMethod method,
                                        const std::string& username,
                                        const std::string& masterChallenge)
{
    Buffer buffer(DEFAULT_MAX_APDU_SIZE);
    APDURequest apdu(buffer.as_wslice());
    apdu.SetControl(AppControlField(true, true, false, false, seq));
    apdu.SetFunction(FunctionCode::AUTH_REQUEST);

    rseq_t name(reinterpret_cast<const uint8_t*>(username.c_str()), static_cast<uint32_t>(username.size()));
    HexSequence challenge(masterChallenge);

    apdu.GetWriter().WriteFreeFormat(Group120Var11(method, name, challenge));

    return HexConversions::to_hex(apdu.ToRSeq());
}

std::string BeginUpdateKeyChangeResponse(uint8_t seq,
                                         uint32_t ksq,
                                         uint16_t user,
                                         const std::string& outstationChallenge)
{
    Buffer buffer(DEFAULT_MAX_APDU_SIZE);
    APDUResponse apdu(buffer.as_wslice());
    apdu.SetControl(AppControlField(true, true, false, false, seq));
    apdu.SetFunction(FunctionCode::AUTH_RESPONSE);
    apdu.SetIIN(IINBit::DEVICE_RESTART);

    HexSequence challenge(outstationChallenge);

    apdu.GetWriter().WriteFreeFormat(Group120Var12(ksq, user, challenge));

    return HexConversions::to_hex(apdu.ToRSeq());
}

std::string FinishUpdateKeyChangeRequest(
    uint8_t seq, uint32_t ksq, uint16_t user, const std::string& encryptedData, const std::string& hmac)
{
    Buffer buffer(DEFAULT_MAX_APDU_SIZE);
    APDURequest apdu(buffer.as_wslice());
    apdu.SetControl(AppControlField(true, true, false, false, seq));
    apdu.SetFunction(FunctionCode::AUTH_REQUEST);

    HexSequence encryptedBuffer(encryptedData);
    HexSequence hmacBuffer(hmac);

    auto writer = apdu.GetWriter();

    writer.WriteFreeFormat(Group120Var13(ksq, user, encryptedBuffer.ToRSeq()));
    writer.WriteFreeFormat(Group120Var15(hmacBuffer.ToRSeq()));

    return HexConversions::to_hex(apdu.ToRSeq());
}

std::string FinishUpdateKeyChangeResponse(uint8_t seq, const std::string& hmac)
{
    Buffer buffer(DEFAULT_MAX_APDU_SIZE);
    APDUResponse apdu(buffer.as_wslice());
    apdu.SetControl(AppControlField(true, true, false, false, seq));
    apdu.SetFunction(FunctionCode::AUTH_RESPONSE);
    apdu.SetIIN(IINBit::DEVICE_RESTART);

    HexSequence hmacBuffer(hmac);

    apdu.GetWriter().WriteFreeFormat(Group120Var15(hmacBuffer.ToRSeq()));

    return HexConversions::to_hex(apdu.ToRSeq());
}

std::string KeyWrapData(uint16_t keyLengthBytes, uint8_t keyRepeatValue, const std::string& keyStatusMsg)
{
    Buffer key(keyLengthBytes);
    key.as_wslice().set_all_to(keyRepeatValue);
    auto keyHex = HexConversions::to_hex(key.as_rslice());
    HexSequence statusBuffer(keyStatusMsg);

    Buffer lengthBuff(2);
    auto lenDest = lengthBuff.as_wslice();
    UInt16::write_to(lenDest, keyLengthBytes);
    auto lengthHex = HexConversions::to_hex(lengthBuff.as_rslice());

    return HexConversions::append_hex({lengthHex, keyHex, keyHex, keyStatusMsg});
}

} // namespace hex
