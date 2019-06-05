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
#include "Database.h"

#include <log4cpp/LogMacros.h>

#include <cassert>

namespace opendnp3
{

Database::Database(const DatabaseConfig& config,
                   IEventReceiver& event_receiver,                   
                   StaticTypeBitField allowed_class_zero_types)
    : event_receiver(event_receiver), 
	  data_maps(config, allowed_class_zero_types)
{
}

bool Database::Update(const Binary& value, uint16_t index, EventMode mode)
{
    return this->UpdateEvent<BinarySpec>(value, index, mode);
}

bool Database::Update(const DoubleBitBinary& value, uint16_t index, EventMode mode)
{
    return this->UpdateEvent<DoubleBitBinarySpec>(value, index, mode);
}

bool Database::Update(const Analog& value, uint16_t index, EventMode mode)
{
    return this->UpdateEvent<AnalogSpec>(value, index, mode);
}

bool Database::Update(const Counter& value, uint16_t index, EventMode mode)
{
    return this->UpdateEvent<CounterSpec>(value, index, mode);
}

bool Database::Update(const FrozenCounter& value, uint16_t index, EventMode mode)
{
    return this->UpdateEvent<FrozenCounterSpec>(value, index, mode);
}

bool Database::Update(const BinaryOutputStatus& value, uint16_t index, EventMode mode)
{
    return this->UpdateEvent<BinaryOutputStatusSpec>(value, index, mode);
}

bool Database::Update(const AnalogOutputStatus& value, uint16_t index, EventMode mode)
{
    return this->UpdateEvent<AnalogOutputStatusSpec>(value, index, mode);
}

bool Database::Update(const OctetString& value, uint16_t index, EventMode mode)
{
    return this->UpdateEvent<OctetStringSpec>(value, index, mode);
}

bool Database::Update(const TimeAndInterval& value, uint16_t index)
{
    this->data_maps.Update(value, index);
    return false;
}

bool Database::Modify(FlagsType type, uint16_t start, uint16_t stop, uint8_t flags)
{
    // TODO
    return false;
}

bool Database::ConvertToEventClass(PointClass pc, EventClass& ec)
{
    switch (pc)
    {
    case (PointClass::Class1):
        ec = EventClass::EC1;
        return true;
    case (PointClass::Class2):
        ec = EventClass::EC2;
        return true;
    case (PointClass::Class3):
        ec = EventClass::EC3;
        return true;
    default:
        return false;
    }
}

template<class Spec> bool Database::UpdateEvent(const typename Spec::meas_t& value, uint16_t index, EventMode mode)
{     
    switch (mode)
    {
    case (EventMode::Force):
    case (EventMode::EventOnly):
        // TODO  - this->TryCreateEvent(cell, value);
        break;
    case (EventMode::Detect):
        if (this->data_maps.Update(value, index))
        {
            // TODO - this->TryCreateEvent(cell, value);
        }
        break;
    default:
        break;
    }

    

    return true;
}

template<class Spec> void Database::TryCreateEvent(Cell<Spec>& cell, const typename Spec::meas_t& value)
{
    EventClass ec;
    // don't create an event if point is assigned to Class 0
    if (ConvertToEventClass(cell.config.clazz, ec))
    {
        cell.event.lastEvent = value;
        this->eventReceiver->Update(Event<Spec>(value, cell.config.vIndex, ec, cell.config.evariation));
    }
}

} // namespace opendnp3
