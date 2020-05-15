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

#ifndef OPENDNP3_IEVENTTYPE_H
#define OPENDNP3_IEVENTTYPE_H

#include "List.h"

#include "opendnp3/app/EventType.h"

namespace opendnp3
{

class EventLists;
class IEventWriteHandler;
class EventRecord;

class IEventType
{

public:
    const EventType value;

    inline bool IsEqual(EventType type) const
    {
        return type == value;
    }

    inline bool IsNotEqual(EventType type) const
    {
        return type != value;
    }

protected:
    IEventType(EventType value) : value(value) {}

public:
    virtual void SelectDefaultVariation(EventRecord& record) const = 0;

    virtual uint16_t WriteSome(List<EventRecord>::Iterator& iterator,
                               EventLists& lists,
                               IEventWriteHandler& handler) const = 0;

    virtual void RemoveTypeFromStorage(EventRecord& record, EventLists& lists) const = 0;
};

} // namespace opendnp3

#endif
