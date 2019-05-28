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
#ifndef OPENDNP3_INDEXSEARCH_H
#define OPENDNP3_INDEXSEARCH_H

#include <ser4cpp/container/ArrayView.h>
#include <ser4cpp/util/Uncopyable.h>

#include "opendnp3/app/MeasurementTypes.h"
#include "app/Range.h"
#include "opendnp3/outstation/Cell.h"

namespace opendnp3
{

/**
 * Implements a binary search for virtual indices in a discontiguous database
 */
class IndexSearch : private ser4cpp::StaticOnly
{

public:
    class Result
    {
    public:
        Result(bool match_, uint16_t index_) : match(match_), index(index_) {}

        const bool match;
        const uint16_t index;

    private:
        Result() = delete;
    };

    template<class T> static Range FindRawRange(const ser4cpp::ArrayView<Cell<T>, uint16_t>& view, const Range& range);

    template<class T>
    static Result FindClosestRawIndex(const ser4cpp::ArrayView<Cell<T>, uint16_t>& view, uint16_t vIndex);

private:
    static uint16_t GetMidpoint(uint16_t lower, uint16_t upper)
    {
        return ((upper - lower) / 2) + lower;
    }
};

template<class T> Range IndexSearch::FindRawRange(const ser4cpp::ArrayView<Cell<T>, uint16_t>& view, const Range& range)
{
    if (range.IsValid() && view.is_not_empty())
    {
        uint16_t start = FindClosestRawIndex(view, range.start).index;
        uint16_t stop = FindClosestRawIndex(view, range.stop).index;

        if (view[start].config.vIndex < range.start)
        {
            if (start < std::numeric_limits<uint16_t>::max())
            {
                ++start;
            }
            else
            {
                return Range::Invalid();
            }
        }

        if (view[stop].config.vIndex > range.stop)
        {
            if (stop > 0)
            {
                --stop;
            }
            else
            {
                return Range::Invalid();
            }
        }

        return (view.contains(start) && view.contains(stop)) ? Range::From(start, stop) : Range::Invalid();
    }
    else
    {
        return Range::Invalid();
    }
}

template<class T>
IndexSearch::Result IndexSearch::FindClosestRawIndex(const ser4cpp::ArrayView<Cell<T>, uint16_t>& view, uint16_t vIndex)
{
    if (view.is_empty())
    {
        return Result(false, 0);
    }

    uint16_t lower = 0;
    uint16_t upper = view.length() - 1;

    uint16_t midpoint = 0;

    while (lower <= upper)
    {
        midpoint = GetMidpoint(lower, upper);

        const auto index = view[midpoint].config.vIndex;

        if (index == vIndex)
        {
            // exact match
            return Result(true, midpoint);
        }

        if (index < vIndex) // search the upper array
        {
            if (lower < std::numeric_limits<uint16_t>::max())
            {
                lower = midpoint + 1;
            }
            else
            {
                // we're at the upper limit
                return Result(false, midpoint);
            }
        }
        else // search the lower array
        {
            if (upper > 0 && midpoint > 0)
            {
                upper = midpoint - 1;
            }
            else
            {
                // we're at the lower limit
                return Result(false, midpoint);
            }
        }
    }

    return Result(false, midpoint);
}

} // namespace opendnp3

#endif
