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

#include "EventWriting.h"

#include "EventCollection.h"
#include "IEventType.h"

namespace opendnp3
{

uint32_t EventWriting::Write(EventLists& lists, IEventWriteHandler& handler)
{
    uint32_t total_num_written = 0;

    auto iterator = lists.events.Iterate();

    while (true)
    {
        // continue calling WriteSome(..) until it fails to make progress
        auto num_written = WriteSome(iterator, lists, handler);

        if (num_written == 0)
        {
            return total_num_written;
        }

        total_num_written += num_written;
    }
}

EventRecord* EventWriting::FindNextSelected(event_iter_t& iter, EventType type)
{
    while (true)
    {
        auto current = iter.CurrentValue();
        if (!current)
            return nullptr;

        if (current->state == EventState::selected)
        {
            // we terminate here since the type has changed
            return current->type->IsEqual(type) ? current : nullptr;
        }

        iter.Next();
    }
}

uint16_t EventWriting::WriteSome(event_iter_t& iterator, EventLists& lists, IEventWriteHandler& handler)
{
    // don't bother searching
    if (lists.counters.selected == 0)
        return 0;

    const auto value = iterator.Find([](const EventRecord& record) { return record.state == EventState::selected; });

    if (!value)
        return 0; // no match

    return value->type->WriteSome(iterator, lists, handler);
}

} // namespace opendnp3
