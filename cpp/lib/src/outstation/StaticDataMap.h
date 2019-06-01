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

template<class T> class StaticSelection : private Uncopyable
{
    using map_iter = std::map<uint16_t, Cell<T>>::iterator;

public:

    class iterator
    {
        map_iter iter;
        Range& range;

    public:
        explicit iterator(map_iter iter, Range& range) : iter(iter),  range(range) {}

        using value_type = Cell<T>;
        using difference_type = std::ptrdiff_t;
        using pointer = Cell<T>*;
        using reference = Cell<T>&;
        using iterator_category = std::input_iterator_tag;
    };

private:

    std::map<uint16_t, Cell<T>> map;
    Range selected;
};

} // namespace opendnp3

#endif
