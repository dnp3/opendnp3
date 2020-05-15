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

#include "EventSelection.h"

namespace opendnp3
{

uint32_t EventSelection::SelectByClass(EventLists& lists, const ClassField& clazz, uint32_t max)
{
    uint32_t num_selected = 0;
    auto iter = lists.events.Iterate();

    while (iter.HasNext() && num_selected < max)
    {
        auto node = iter.Next();
        if (node->value.state == EventState::unselected && clazz.HasEventType(node->value.clazz))
        {
            // if not previously selected
            node->value.state = EventState::selected;
            // TODO - set the storage to use the default variation
            // node->value.selectedVariation = useDefaultVariation ? node->value.defaultVariation : variation;
            ++num_selected;
            lists.counters.OnSelect();
        }
    }

    return num_selected;
}

} // namespace opendnp3
