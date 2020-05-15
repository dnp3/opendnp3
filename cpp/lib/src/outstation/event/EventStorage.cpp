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

#include "EventStorage.h"

#include "EventSelection.h"
#include "EventUpdate.h"
#include "EventWriting.h"

namespace opendnp3
{

EventStorage::EventStorage(const EventBufferConfig& config) : state(config) {}

bool EventStorage::IsAnyTypeFull() const
{
    return this->state.IsAnyTypeFull();
}

uint32_t EventStorage::NumSelected() const
{
    return this->state.counters.selected;
}

uint32_t EventStorage::NumUnwritten(EventClass clazz) const
{
    return this->state.counters.total.Get(clazz) - this->state.counters.written.Get(clazz);
}

bool EventStorage::Update(const Event<BinarySpec>& evt)
{
    return EventUpdate::Update(state, evt);
}

bool EventStorage::Update(const Event<DoubleBitBinarySpec>& evt)
{
    return EventUpdate::Update(state, evt);
}

bool EventStorage::Update(const Event<AnalogSpec>& evt)
{
    return EventUpdate::Update(state, evt);
}

bool EventStorage::Update(const Event<CounterSpec>& evt)
{
    return EventUpdate::Update(state, evt);
}

bool EventStorage::Update(const Event<FrozenCounterSpec>& evt)
{
    return EventUpdate::Update(state, evt);
}

bool EventStorage::Update(const Event<BinaryOutputStatusSpec>& evt)
{
    return EventUpdate::Update(state, evt);
}

bool EventStorage::Update(const Event<AnalogOutputStatusSpec>& evt)
{
    return EventUpdate::Update(state, evt);
}

bool EventStorage::Update(const Event<OctetStringSpec>& evt)
{
    return EventUpdate::Update(state, evt);
}

uint32_t EventStorage::SelectByType(EventBinaryVariation variation, uint32_t max)
{
    return EventSelection::SelectByType<BinarySpec>(this->state, variation, max);
}

uint32_t EventStorage::SelectByType(EventDoubleBinaryVariation variation, uint32_t max)
{
    return EventSelection::SelectByType<DoubleBitBinarySpec>(this->state, variation, max);
}

uint32_t EventStorage::SelectByType(EventAnalogVariation variation, uint32_t max)
{
    return EventSelection::SelectByType<AnalogSpec>(this->state, variation, max);
}

uint32_t EventStorage::SelectByType(EventCounterVariation variation, uint32_t max)
{
    return EventSelection::SelectByType<CounterSpec>(this->state, variation, max);
}

uint32_t EventStorage::SelectByType(EventFrozenCounterVariation variation, uint32_t max)
{
    return EventSelection::SelectByType<FrozenCounterSpec>(this->state, variation, max);
}

uint32_t EventStorage::SelectByType(EventBinaryOutputStatusVariation variation, uint32_t max)
{
    return EventSelection::SelectByType<BinaryOutputStatusSpec>(this->state, variation, max);
    return 0;
}

uint32_t EventStorage::SelectByType(EventAnalogOutputStatusVariation variation, uint32_t max)
{
    return EventSelection::SelectByType<AnalogOutputStatusSpec>(this->state, variation, max);
}

uint32_t EventStorage::SelectByType(EventOctetStringVariation variation, uint32_t max)
{
    return EventSelection::SelectByType<OctetStringSpec>(this->state, variation, max);
}

uint32_t EventStorage::SelectByType(EventType type, uint32_t max)
{
    switch (type)
    {
    case (EventType::Binary):
        return EventSelection::SelectByType<BinarySpec>(this->state, max);
    case (EventType::DoubleBitBinary):
        return EventSelection::SelectByType<DoubleBitBinarySpec>(this->state, max);
    case (EventType::Counter):
        return EventSelection::SelectByType<CounterSpec>(this->state, max);
    case (EventType::FrozenCounter):
        return EventSelection::SelectByType<FrozenCounterSpec>(this->state, max);
    case (EventType::Analog):
        return EventSelection::SelectByType<AnalogSpec>(this->state, max);
    case (EventType::BinaryOutputStatus):
        return EventSelection::SelectByType<BinaryOutputStatusSpec>(this->state, max);
    case (EventType::AnalogOutputStatus):
        return EventSelection::SelectByType<AnalogOutputStatusSpec>(this->state, max);
    case (EventType::OctetString):
        return EventSelection::SelectByType<OctetStringSpec>(this->state, max);
    default:
        return 0;
    }
}

uint32_t EventStorage::SelectByClass(const EventClass& clazz)
{
    return EventSelection::SelectByClass(this->state, ClassField(clazz), std::numeric_limits<uint32_t>::max());
}

uint32_t EventStorage::SelectByClass(const EventClass& clazz, uint32_t max)
{
    return EventSelection::SelectByClass(this->state, ClassField(clazz), max);
}

uint32_t EventStorage::SelectByClass(const ClassField& clazz)
{
    return EventSelection::SelectByClass(this->state, clazz, std::numeric_limits<uint32_t>::max());
}

uint32_t EventStorage::SelectByClass(const ClassField& clazz, uint32_t max)
{
    return EventSelection::SelectByClass(this->state, clazz, max);
}

uint32_t EventStorage::Write(IEventWriteHandler& handler)
{
    return EventWriting::Write(this->state, handler);
}

uint32_t EventStorage::ClearWritten()
{
    auto written = [this](EventRecord& record) -> bool {
        if (record.state == EventState::written)
        {
            record.type->RemoveTypeFromStorage(record, this->state);
            this->state.counters.OnRemove(record.clazz, record.state);
            return true;
        }

        return false;
    };

    return this->state.events.RemoveAll(written);
}

void EventStorage::Unselect()
{
    auto clear = [](EventRecord& record) -> void { record.state = EventState::unselected; };

    this->state.events.Foreach(clear);

    // keep the total, but clear the selected/written
    this->state.counters.ResetOnFail();
}

} // namespace opendnp3
