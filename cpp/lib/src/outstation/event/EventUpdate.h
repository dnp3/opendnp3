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
#ifndef OPENDNP3_EVENTUPDATE_H
#define OPENDNP3_EVENTUPDATE_H

#include "EventLists.h"
#include "EventTypeImpl.h"
#include "outstation/Event.h"

namespace opendnp3
{

struct EventUpdate : private StaticOnly
{
    template<class T> static bool Update(EventLists& lists, const Event<T>& event);
};

template<class T> bool EventUpdate::Update(EventLists& lists, const Event<T>& event)
{
    auto& list = lists.GetList<T>();

    // lists with no capacity don't cause "buffer overflow"
    if (list.Capacity() == 0)
        return false;

    bool overflow = false;

    if (list.IsFullAndCapacityNotZero())
    {
        // we must make space

        overflow = true;
        const auto first = list.Head();
        const auto record_node = first->value.record;

        // remove the generic record
        lists.counters.OnRemove(record_node->value.clazz, record_node->value.state);
        lists.events.Remove(first->value.record);

        // remove the type specific record
        list.Remove(first);
    }

    // now that we know that space exists, create the generic record
    const auto record_node = lists.events.Add(EventRecord(event.index, event.clazz));

    // followed by the typed record
    const auto typed_node = list.Add(TypedEventRecord<T>(event.value, event.variation, record_node));

    // configure the typed storage
    record_node->value.type = EventTypeImpl<T>::Instance();
    record_node->value.storage_node = typed_node;

    lists.counters.OnAdd(event.clazz);

    return overflow;
}

} // namespace opendnp3

#endif
