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
#ifndef OPENDNP3_EVENTSELECTION_H
#define OPENDNP3_EVENTSELECTION_H

#include "EventLists.h"

#include "opendnp3/app/ClassField.h"

namespace opendnp3
{

struct EventSelection : private StaticOnly
{
    template<class T> static uint32_t SelectByType(EventLists& lists, uint32_t max)
    {
        return SelectByTypeGeneric<T>(lists, true, static_cast<typename T::event_variation_t>(0), max);
    }

    template<class T>
    static uint32_t SelectByType(EventLists& lists, typename T::event_variation_t variation, uint32_t max)
    {
        return SelectByTypeGeneric<T>(lists, false, variation, max);
    }

    static uint32_t SelectByClass(EventLists& lists, const ClassField& clazz, uint32_t max);

private:
    template<class T>
    static uint32_t SelectByTypeGeneric(EventLists& lists,
                                        bool useDefaultVariation,
                                        typename T::event_variation_t variation,
                                        uint32_t max);
};

template<class T>
uint32_t EventSelection::SelectByTypeGeneric(EventLists& lists,
                                             bool useDefaultVariation,
                                             typename T::event_variation_t variation,
                                             uint32_t max)
{
    auto& list = lists.GetList<T>();

    uint32_t num_selected = 0;

    auto select = [&](TypedEventRecord<T>& node) -> bool {
        if (num_selected == max)
            return false;

        if (node.record->value.state == EventState::unselected)
        {
            node.record->value.state = EventState::selected;
            node.selectedVariation = useDefaultVariation ? node.defaultVariation : variation;
            lists.counters.OnSelect();
            ++num_selected;
        }

        return true;
    };

    list.ForeachWhile(select);

    return num_selected;
}

} // namespace opendnp3

#endif
