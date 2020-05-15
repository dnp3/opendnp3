/*
 * Copyright 2013-2020 Automatak, LLC
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

#include "utils/BufferHelpers.h"

#include <opendnp3/app/AppConstants.h>

#include <ser4cpp/container/Buffer.h>
#include <ser4cpp/util/HexConversions.h>

#include <app/APDUBuilders.h>
#include <app/APDURequest.h>
#include <app/APDUResponse.h>
#include <gen/objects/Group12.h>

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

} // namespace hex
