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
#ifndef OPENDNP3_INDEXED_H
#define OPENDNP3_INDEXED_H

#include <cstdint>

namespace opendnp3
{

/**
 * A simple tuple for pairing Values with an index
 */
template<class T> class Indexed
{
public:
    Indexed(const T& value_, uint16_t index_) : value(value_), index(index_) {}

    Indexed() : value(), index(0) {}

    T value;
    uint16_t index;
};

template<class T> Indexed<T> WithIndex(const T& value, uint16_t index)
{
    return Indexed<T>(value, index);
}

} // namespace opendnp3

#endif
