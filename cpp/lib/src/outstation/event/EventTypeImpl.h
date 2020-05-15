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

#ifndef OPENDNP3_EVENTTYPEIMPL_H
#define OPENDNP3_EVENTTYPEIMPL_H

#include "EventCollection.h"
#include "EventWriting.h"
#include "IEventType.h"
#include "TypedEventRecord.h"

namespace opendnp3
{

template<class T> class EventTypeImpl final : public IEventType
{

private:
    EventTypeImpl() : IEventType(T::EventTypeEnum) {}

    static EventTypeImpl instance;

public:
    static IEventType* Instance()
    {
        return &instance;
    }

    virtual void SelectDefaultVariation(EventRecord& record) const override
    {
        auto node = TypedStorage<T>::Retrieve(record);
        node->value.selectedVariation = node->value.defaultVariation;
    }

    virtual uint16_t WriteSome(List<EventRecord>::Iterator& iterator,
                               EventLists& lists,
                               IEventWriteHandler& handler) const override
    {
        const auto pos = iterator.CurrentValue();
        const auto type = TypedStorage<T>::Retrieve(*pos);

        EventCollection<T> collection(iterator, lists.counters, type->value.selectedVariation);

        return handler.Write(type->value.selectedVariation, type->value.value, collection);
    }

    virtual void RemoveTypeFromStorage(EventRecord& record, EventLists& lists) const override
    {
        auto node = TypedStorage<T>::Retrieve(record);
        lists.GetList<T>().Remove(node);
    }
};

template<class T> EventTypeImpl<T> EventTypeImpl<T>::instance;

} // namespace opendnp3

#endif
