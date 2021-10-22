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
#include "EventBuffer.h"

#include "ASDUEventWriteHandler.h"

namespace opendnp3
{

EventBuffer::EventBuffer(const EventBufferConfig& config) : storage(config) {}

void EventBuffer::Update(const Event<BinarySpec>& evt)
{
    this->UpdateAny(evt);
}

void EventBuffer::Update(const Event<DoubleBitBinarySpec>& evt)
{
    this->UpdateAny(evt);
}

void EventBuffer::Update(const Event<AnalogSpec>& evt)
{
    this->UpdateAny(evt);
}

void EventBuffer::Update(const Event<CounterSpec>& evt)
{
    this->UpdateAny(evt);
}

void EventBuffer::Update(const Event<FrozenCounterSpec>& evt)
{
    this->UpdateAny(evt);
}

void EventBuffer::Update(const Event<BinaryOutputStatusSpec>& evt)
{
    this->UpdateAny(evt);
}

void EventBuffer::Update(const Event<AnalogOutputStatusSpec>& evt)
{
    this->UpdateAny(evt);
}

void EventBuffer::Update(const Event<OctetStringSpec>& evt)
{
    this->UpdateAny(evt);
}

void EventBuffer::Unselect()
{
    this->storage.Unselect();
}

IINField EventBuffer::SelectAll(GroupVariation gv)
{
    return SelectMaxCount(gv, std::numeric_limits<uint32_t>::max());
}

IINField EventBuffer::SelectCount(GroupVariation gv, uint16_t count)
{
    return SelectMaxCount(gv, count);
}

IINField EventBuffer::SelectMaxCount(GroupVariation gv, uint32_t maximum)
{
    switch (gv)
    {
    case (GroupVariation::Group2Var0):
        return this->SelectByType(maximum, EventType::Binary);
    case (GroupVariation::Group2Var1):
        return this->SelectByType(maximum, EventBinaryVariation::Group2Var1);
    case (GroupVariation::Group2Var2):
        return this->SelectByType(maximum, EventBinaryVariation::Group2Var2);
    case (GroupVariation::Group2Var3):
        return this->SelectByType(maximum, EventBinaryVariation::Group2Var3);

    case (GroupVariation::Group4Var0):
        return this->SelectByType(maximum, EventType::DoubleBitBinary);
    case (GroupVariation::Group4Var1):
        return this->SelectByType(maximum, EventDoubleBinaryVariation::Group4Var1);
    case (GroupVariation::Group4Var2):
        return this->SelectByType(maximum, EventDoubleBinaryVariation::Group4Var2);
    case (GroupVariation::Group4Var3):
        return this->SelectByType(maximum, EventDoubleBinaryVariation::Group4Var3);

    case (GroupVariation::Group11Var0):
        return this->SelectByType(maximum, EventType::BinaryOutputStatus);
    case (GroupVariation::Group11Var1):
        return this->SelectByType(maximum, EventBinaryOutputStatusVariation::Group11Var1);
    case (GroupVariation::Group11Var2):
        return this->SelectByType(maximum, EventBinaryOutputStatusVariation::Group11Var2);

    case (GroupVariation::Group22Var0):
        return this->SelectByType(maximum, EventType::Counter);
    case (GroupVariation::Group22Var1):
        return this->SelectByType(maximum, EventCounterVariation::Group22Var1);
    case (GroupVariation::Group22Var2):
        return this->SelectByType(maximum, EventCounterVariation::Group22Var2);
    case (GroupVariation::Group22Var5):
        return this->SelectByType(maximum, EventCounterVariation::Group22Var5);
    case (GroupVariation::Group22Var6):
        return this->SelectByType(maximum, EventCounterVariation::Group22Var6);

    case (GroupVariation::Group23Var0):
        return this->SelectByType(maximum, EventType::FrozenCounter);
    case (GroupVariation::Group23Var1):
        return this->SelectByType(maximum, EventFrozenCounterVariation::Group23Var1);
    case (GroupVariation::Group23Var2):
        return this->SelectByType(maximum, EventFrozenCounterVariation::Group23Var2);
    case (GroupVariation::Group23Var5):
        return this->SelectByType(maximum, EventFrozenCounterVariation::Group23Var5);
    case (GroupVariation::Group23Var6):
        return this->SelectByType(maximum, EventFrozenCounterVariation::Group23Var6);

    case (GroupVariation::Group32Var0):
        return this->SelectByType(maximum, EventType::Analog);
    case (GroupVariation::Group32Var1):
        return this->SelectByType(maximum, EventAnalogVariation::Group32Var1);
    case (GroupVariation::Group32Var2):
        return this->SelectByType(maximum, EventAnalogVariation::Group32Var2);
    case (GroupVariation::Group32Var3):
        return this->SelectByType(maximum, EventAnalogVariation::Group32Var3);
    case (GroupVariation::Group32Var4):
        return this->SelectByType(maximum, EventAnalogVariation::Group32Var4);
    case (GroupVariation::Group32Var5):
        return this->SelectByType(maximum, EventAnalogVariation::Group32Var5);
    case (GroupVariation::Group32Var6):
        return this->SelectByType(maximum, EventAnalogVariation::Group32Var6);
    case (GroupVariation::Group32Var7):
        return this->SelectByType(maximum, EventAnalogVariation::Group32Var7);
    case (GroupVariation::Group32Var8):
        return this->SelectByType(maximum, EventAnalogVariation::Group32Var8);

    case (GroupVariation::Group42Var0):
        return this->SelectByType(maximum, EventType::AnalogOutputStatus);
    case (GroupVariation::Group42Var1):
        return this->SelectByType(maximum, EventAnalogOutputStatusVariation::Group42Var1);
    case (GroupVariation::Group42Var2):
        return this->SelectByType(maximum, EventAnalogOutputStatusVariation::Group42Var2);
    case (GroupVariation::Group42Var3):
        return this->SelectByType(maximum, EventAnalogOutputStatusVariation::Group42Var3);
    case (GroupVariation::Group42Var4):
        return this->SelectByType(maximum, EventAnalogOutputStatusVariation::Group42Var4);
    case (GroupVariation::Group42Var5):
        return this->SelectByType(maximum, EventAnalogOutputStatusVariation::Group42Var5);
    case (GroupVariation::Group42Var6):
        return this->SelectByType(maximum, EventAnalogOutputStatusVariation::Group42Var6);
    case (GroupVariation::Group42Var7):
        return this->SelectByType(maximum, EventAnalogOutputStatusVariation::Group42Var7);
    case (GroupVariation::Group42Var8):
        return this->SelectByType(maximum, EventAnalogOutputStatusVariation::Group42Var8);

    case (GroupVariation::Group60Var2):
        return this->SelectByClass(maximum, EventClass::EC1);
    case (GroupVariation::Group60Var3):
        return this->SelectByClass(maximum, EventClass::EC2);
    case (GroupVariation::Group60Var4):
        return this->SelectByClass(maximum, EventClass::EC3);

    case (GroupVariation::Group111Var0):
        return this->SelectByType(maximum, EventOctetStringVariation::Group111Var0);

    default:
        return IINBit::FUNC_NOT_SUPPORTED;
    }
}

bool EventBuffer::HasAnySelection() const
{
    // are there any selected, but unwritten, events
    return storage.NumSelected() > 0;
}

bool EventBuffer::Load(HeaderWriter& writer)
{
    ASDUEventWriteHandler handler(writer);
    this->storage.Write(handler);
    // all selected events were written?
    return this->storage.NumSelected() == 0;
}

ClassField EventBuffer::UnwrittenClassField() const
{
    return ClassField(false, storage.NumUnwritten(EventClass::EC1) > 0, storage.NumUnwritten(EventClass::EC2) > 0,
                      storage.NumUnwritten(EventClass::EC3) > 0);
}

bool EventBuffer::IsOverflown()
{
    if (overflow && !this->storage.IsAnyTypeFull())
    {
        overflow = false;
    }

    return overflow;
}

void EventBuffer::SelectAllByClass(const ClassField& clazz)
{
    this->storage.SelectByClass(clazz);
}

void EventBuffer::ClearWritten()
{
    this->storage.ClearWritten();
}

uint32_t EventBuffer::NumEvents(EventClass ec) const
{
    return this->storage.NumUnwritten(ec);
}

} // namespace opendnp3
