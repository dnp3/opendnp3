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
#ifndef OPENDNP3_STATICDATAMAP_H
#define OPENDNP3_STATICDATAMAP_H

#include "app/Range.h"

#include "opendnp3/Uncopyable.h"
#include "opendnp3/outstation/Cell.h"

#include <iterator>
#include <map>

namespace opendnp3
{

enum class UpdateResult
{
    point_not_defined,
    no_change,
    event
};

template<class Spec> class StaticDataMap : private Uncopyable
{
    using map_iter = typename std::map<uint16_t, Cell<Spec>>::iterator;

public:
    class iterator
    {
        map_iter iter;
        Range& range;

    public:
        explicit iterator(map_iter iter, Range& range) : iter(iter), range(range) {}

        using value_type = Cell<Spec>;
        using difference_type = std::ptrdiff_t;
        using pointer = Cell<Spec>*;
        using reference = Cell<Spec>&;
        using iterator_category = std::input_iterator_tag;
    };

    bool add(const typename Spec::meas_t& value, uint16_t index, typename Spec::config_t config);

    UpdateResult update(const typename Spec::meas_t& value, uint16_t index);

private:

    std::map<uint16_t, Cell<Spec>> map;
    Range selected;
};

template<class Spec>
bool StaticDataMap<Spec>::add(const typename Spec::meas_t& value, uint16_t index, typename Spec::config_t config)
{
    if (this->map.find(index) != this->map.end())
    {
        return false;
    }

    this->map[index] = Cell<Spec>{value, config, {}, {}};

    return true;
}

template<class Spec> UpdateResult StaticDataMap<Spec>::update(const typename Spec::meas_t& value, uint16_t index)
{
    const auto iter = this->map.find(index);
    if (iter == this->map.end())
    {
        return UpdateResult::point_not_defined;
    }

    const auto is_event = Spec::IsEvent(iter->second.event.lastEvent, value);

    iter->second.value = value;

    if (is_event)
    {
        iter->second.event.lastEvent = value;
    }

    return is_event ? UpdateResult::event : UpdateResult::no_change;
}

} // namespace opendnp3

#endif
