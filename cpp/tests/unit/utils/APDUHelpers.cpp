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
#include "utils/APDUHelpers.h"

uint8_t APDUHelpers::fixedBuffer[SIZE];

opendnp3::APDURequest APDUHelpers::Request(opendnp3::FunctionCode code, uint32_t size)
{
    assert(size <= SIZE);
    ser4cpp::wseq_t buffer(fixedBuffer, size);
    opendnp3::APDURequest request(buffer);
    request.SetFunction(code);
    request.SetControl(opendnp3::AppControlField(true, true, false, false, 0));
    return request;
}

opendnp3::APDUResponse APDUHelpers::Response(uint32_t size)
{
    assert(size <= SIZE);
    ser4cpp::wseq_t buffer(fixedBuffer, size);
    opendnp3::APDUResponse response(buffer);
    response.SetFunction(opendnp3::FunctionCode::RESPONSE);
    response.SetControl(opendnp3::AppControlField(true, true, false, false, 0));
    response.SetIIN(opendnp3::IINField::Empty());
    return response;
}
