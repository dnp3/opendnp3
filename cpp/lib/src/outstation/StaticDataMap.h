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
#include "app/MeasurementTypeSpecs.h"
#include "outstation/StaticDataCell.h"

#include "opendnp3/Uncopyable.h"

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
    using map_t = std::map<uint16_t, StaticDataCell<Spec>>;
    using map_iter_t = typename map_t::iterator;

public:

    StaticDataMap() = default;
    StaticDataMap(const std::map<uint16_t, typename Spec::config_t>& config);

    class iterator
    {
        map_iter_t iter;
        map_iter_t end;
        Range& range;

    public:
        explicit iterator(map_iter_t begin, map_iter_t end, Range& range) : iter(begin), end(end), range(range) {}

        using value_type = std::pair<uint16_t, SelectedValue<Spec>>;
        using difference_type = typename map_iter_t::difference_type;
        using pointer = typename map_iter_t::pointer;
        using reference = std::pair<uint16_t, SelectedValue<Spec>&>;
        using iterator_category = std::input_iterator_tag;

        bool operator==(const iterator& rhs)
        {
            return this->iter == rhs.iter;
        }
        bool operator!=(const iterator& rhs)
        {
            return this->iter != rhs.iter;
        }

        void operator++()
        {
            // unselect the point
            this->iter->second.selection.selected = false;            


            while (true)
            {
                iter++;

                if (iter == this->end)
                {
                    this->range = Range::Invalid();
                    return;
                }

				// shorten the range
                this->range.start = iter->first; 

                if (iter->second.selection.selected)
                {
                    return;
                }
            }
        }

        reference operator*()
        {
            return reference(iter->first, iter->second.selection);
        }
    };

    bool add(const typename Spec::meas_t& value, uint16_t index, typename Spec::config_t config);

    UpdateResult update(const typename Spec::meas_t& value, uint16_t index);	

    void clear_selection();

    bool has_any_selection() const
    {
        return this->selected.IsValid();
	}

    Range get_selected_range() const
    {
        return this->selected;
    }

    size_t select_all()
    {
        return this->select_all([](auto var) { return var; }); // use the default
    }

    size_t select_all(typename Spec::static_variation_t variation)
    {
        return this->select_all([variation](auto var) { return variation; }); // override default
    }

    size_t select(Range range)
    {
        return this->select(range, [](auto var) { return var; }); // use the default
    }

    size_t select(Range range, typename Spec::static_variation_t variation)
    {
        return this->select_all([variation](auto var) { return variation; }); // override default
    }

    iterator begin();

    iterator end();

private:
    std::map<uint16_t, StaticDataCell<Spec>> map;
    Range selected;

    // generic implementation of select_all that accepts a function
    // that can use or override the default variation
    template<class F> size_t select_all(F get_variation);

    // generic implementation of select that accepts a function
    // that can use or override the default variation
    template<class F> size_t select(Range range, F get_variation);
};


template<class Spec> StaticDataMap<Spec>::StaticDataMap(const std::map<uint16_t, typename Spec::config_t>& config)
{
    for (const auto& item : config)
    {
        this->map[item.first] = StaticDataCell<Spec>{item.second};
    }
}

template<class Spec>
bool StaticDataMap<Spec>::add(const typename Spec::meas_t& value, uint16_t index, typename Spec::config_t config)
{
    if (this->map.find(index) != this->map.end())
    {
        return false;
    }

    this->map[index] = StaticDataCell<Spec>{value, config};

    return true;
}

template<> UpdateResult StaticDataMap<TimeAndIntervalSpec>::update(const TimeAndInterval& value, uint16_t index);

template<class Spec> UpdateResult StaticDataMap<Spec>::update(const typename Spec::meas_t& value, uint16_t index)
{
    const auto iter = this->map.find(index);
    if (iter == this->map.end())
    {
        return UpdateResult::point_not_defined;
    }

    const auto is_event = Spec::IsEvent(iter->second.event.lastEvent, value, iter->second.config);

    iter->second.value = value;

    if (is_event)
    {
        iter->second.event.lastEvent = value;
    }

    return is_event ? UpdateResult::event : UpdateResult::no_change;
}

template<class Spec> void StaticDataMap<Spec>::clear_selection()
{
    // the act of iterating clears the selection
    for (auto value : *this)
    {
    }
}

template<class Spec> template<class F> size_t StaticDataMap<Spec>::select_all(F get_variation)
{
    if (map.empty())
    {
        return 0;
    }
    else
    {
        this->selected = Range::From(map.begin()->first, map.rbegin()->first);

        for (auto& iter : this->map)
        {
            iter.second.selection
                = SelectedValue<Spec>{true, iter.second.value, get_variation(iter.second.config.svariation)};
        }

        return this->map.size();
    }
}

template<class Spec> template<class F> size_t StaticDataMap<Spec>::select(Range range, F get_variation)
{
    if (!range.IsValid())
    {
        return 0;
    }

    const auto start = this->map.lower_bound(range.start);

    if (start == this->map.end())
    {
        return 0;
    }

    if (!range.Contains(start->first))
    {
        return 0;
    }

    uint16_t stop = 0;
    size_t count = 0;

    for (auto iter = start; iter != this->map.end(); ++iter)
    {
        if (!range.Contains(iter->first))
        {
            break;
        }

        stop = iter->first;
        iter->second.selection
            = SelectedValue<Spec>{true, iter->second.value, get_variation(iter->second.config.svariation)};
        ++count;
    }

    this->selected = this->selected.Union(Range::From(start->first, stop));

    return count;
}

template<class Spec> typename StaticDataMap<Spec>::iterator StaticDataMap<Spec>::begin()
{
    if (!this->selected.IsValid())
    {
        return iterator(this->map.end(), this->map.end(), this->selected);
    }

    const auto begin = this->map.lower_bound(this->selected.start);

    return iterator(begin, this->map.end(), this->selected);
}

template<class Spec> typename StaticDataMap<Spec>::iterator StaticDataMap<Spec>::end()
{
    return iterator(this->map.end(), this->map.end(), this->selected);
}

} // namespace opendnp3

#endif
