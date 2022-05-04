/*
 * Copyright 2013-2022 Step Function I/O, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Step Function I/O
 * LLC (https://stepfunc.io) under one or more contributor license agreements.
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Step Function I/O LLC license
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
#ifndef OPENDNP3_TXBUFFER_H
#define OPENDNP3_TXBUFFER_H

#include "app/APDUResponse.h"

#include <ser4cpp/container/Buffer.h>

namespace opendnp3
{

class TxBuffer
{
public:
    TxBuffer(uint32_t maxTxSize) : buffer(maxTxSize) {}

    APDUResponse Start()
    {
        APDUResponse response(buffer.as_wslice());
        return response;
    }

    void Record(const AppControlField& control, const ser4cpp::rseq_t& view)
    {
        this->control = control;
        this->lastResponse = view;
    }

    const ser4cpp::rseq_t& GetLastResponse() const
    {
        return lastResponse;
    }

    const AppControlField& GetLastControl() const
    {
        return control;
    }

private:
    ser4cpp::rseq_t lastResponse;
    AppControlField control;

    ser4cpp::Buffer buffer;
};

} // namespace opendnp3

#endif
