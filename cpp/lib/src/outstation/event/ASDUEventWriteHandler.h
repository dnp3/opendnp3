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
#ifndef OPENDNP3_ASDUEVENTWRITEHANDLER_H
#define OPENDNP3_ASDUEVENTWRITEHANDLER_H

#include "IEventWriteHandler.h"
#include "app/DNP3Serializer.h"
#include "app/HeaderWriter.h"

#include "opendnp3/util/Uncopyable.h"

namespace opendnp3
{

class ASDUEventWriteHandler final : public IEventWriteHandler, private Uncopyable
{

public:
    explicit ASDUEventWriteHandler(const HeaderWriter& writer) : writer(writer) {}

    virtual uint16_t Write(EventBinaryVariation variation,
                           const Binary& first,
                           IEventCollection<Binary>& items) override;
    virtual uint16_t Write(EventDoubleBinaryVariation variation,
                           const DoubleBitBinary& first,
                           IEventCollection<DoubleBitBinary>& items) override;
    virtual uint16_t Write(EventCounterVariation variation,
                           const Counter& first,
                           IEventCollection<Counter>& items) override;
    virtual uint16_t Write(EventFrozenCounterVariation variation,
                           const FrozenCounter& first,
                           IEventCollection<FrozenCounter>& items) override;
    virtual uint16_t Write(EventAnalogVariation variation,
                           const Analog& first,
                           IEventCollection<Analog>& items) override;
    virtual uint16_t Write(EventBinaryOutputStatusVariation variation,
                           const BinaryOutputStatus& first,
                           IEventCollection<BinaryOutputStatus>& items) override;
    virtual uint16_t Write(EventAnalogOutputStatusVariation variation,
                           const AnalogOutputStatus& first,
                           IEventCollection<AnalogOutputStatus>& items) override;
    virtual uint16_t Write(EventOctetStringVariation variation,
                           const OctetString& first,
                           IEventCollection<OctetString>& items) override;

private:
    HeaderWriter writer;
};

} // namespace opendnp3

#endif
