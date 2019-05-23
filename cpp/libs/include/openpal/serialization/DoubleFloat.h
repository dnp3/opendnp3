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
#ifndef OPENPAL_DOUBLE_FLOAT_H
#define OPENPAL_DOUBLE_FLOAT_H

#include "openpal/container/RSlice.h"
#include "openpal/container/WSlice.h"
#include "openpal/util/Uncopyable.h"

#include <cstddef>

namespace openpal
{

class DoubleFloat : private StaticOnly
{
public:
    static_assert(sizeof(double) == 8, "Unexpected size of double float");

    typedef double Type;

    static double ReadBuffer(RSlice& buffer);
    static void WriteBuffer(WSlice& buffer, double value);

    static double Read(const uint8_t* data);
    static void Write(uint8_t* dest, double value);

    const static std::size_t SIZE = sizeof(double);
    const static double Max;
    const static double Min;
};

} // namespace openpal

#endif
