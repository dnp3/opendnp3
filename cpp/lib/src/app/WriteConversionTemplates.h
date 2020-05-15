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
#ifndef OPENDNP3_WRITECONVERSIONTEMPLATES_H
#define OPENDNP3_WRITECONVERSIONTEMPLATES_H

#include "app/DownSampling.h"

#include "opendnp3/gen/CommandStatus.h"
#include "opendnp3/util/StaticOnly.h"

#include <cstdint>

namespace opendnp3
{
template<class Target, class Source> struct ConvertQ : private StaticOnly
{
    static Target Apply(const Source& src)
    {
        Target t;
        t.flags = src.flags.value;
        return t;
    }
};

template<class Target, class Source> struct ConvertQV : private StaticOnly
{
    static Target Apply(const Source& src)
    {
        Target t;
        t.value = src.value;
        t.flags = src.flags.value;
        return t;
    }
};

template<class Target, class Source> struct ConvertV : private StaticOnly
{
    static Target Apply(const Source& src)
    {
        Target t;
        t.value = src.value;
        return t;
    }
};

template<class Target, class Source> struct ConvertVandTruncate : private StaticOnly
{
    static Target Apply(const Source& src)
    {
        Target t;
        t.value = static_cast<typename Target::ValueType>(src.value);
        return t;
    }
};

template<class Target, class Source> struct ConvertVRangeCheck : private StaticOnly
{
    static Target Apply(const Source& src)
    {
        Target t;
        DownSampling<typename Source::Type, typename Target::ValueType>::Apply(src.value, t.value);
        return t;
    }
};

template<class Target, class Source, uint8_t Overrange> struct ConvertQVRangeCheck : private StaticOnly
{
    static Target Apply(const Source& src)
    {
        Target t;
        auto overrange = DownSampling<typename Source::Type, typename Target::ValueType>::Apply(src.value, t.value);
        t.flags = overrange ? Overrange : 0;
        t.flags |= src.flags.value;
        return t;
    }
};

template<class Target, class Source> struct ConvertQVandTruncate : private StaticOnly
{
    static Target Apply(const Source& src)
    {
        Target t;
        t.flags = src.flags.value;
        t.value = static_cast<typename Target::ValueType>(src.value);
        return t;
    }
};

template<class Target, class Source> struct ConvertQVT : private StaticOnly
{
    static Target Apply(const Source& src)
    {
        Target t;
        t.flags = src.flags.value;
        t.value = src.value;
        t.time = src.time;
        return t;
    }
};

template<class Target, class Source> struct ConvertQVTandTruncate : private StaticOnly
{
    static Target Apply(const Source& src)
    {
        Target t;
        t.flags = src.flags.value;
        t.value = static_cast<typename Target::ValueType>(src.value);
        t.time = src.time;
        return t;
    }
};

template<class Target, class Source, uint8_t Overrange> struct ConvertQVTRangeCheck : private StaticOnly
{
    static Target Apply(const Source& src)
    {
        Target t;
        auto overrange = DownSampling<typename Source::Type, typename Target::ValueType>::Apply(src.value, t.value);
        t.flags = overrange ? Overrange : 0;
        t.flags |= src.flags.value;
        t.time = src.time;
        return t;
    }
};

template<class Target, class Source> struct ConvertQT : private StaticOnly
{
    static Target Apply(const Source& src)
    {
        Target t;
        t.flags = src.flags.value;
        t.time = src.time;
        return t;
    }
};

template<class Target, class Source, class Downcast> struct ConvertQTDowncast : private StaticOnly
{
    static Target Apply(const Source& src)
    {
        Target t;
        t.flags = src.flags.value;
        t.time = static_cast<Downcast>(src.time.value);
        return t;
    }
};

template<class Target, class Source> struct ConvertQS : private StaticOnly
{
    static Target Apply(const Source& src)
    {
        Target t;
        t.value = src.value;
        t.status = CommandStatusSpec::to_type(src.status);
        return t;
    }
};

} // namespace opendnp3

#endif
