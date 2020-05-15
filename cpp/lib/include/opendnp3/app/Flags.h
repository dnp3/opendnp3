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
#ifndef OPENDNP3_FLAGS_H
#define OPENDNP3_FLAGS_H

#include "opendnp3/gen/AnalogOutputStatusQuality.h"
#include "opendnp3/gen/AnalogQuality.h"
#include "opendnp3/gen/BinaryOutputStatusQuality.h"
#include "opendnp3/gen/BinaryQuality.h"
#include "opendnp3/gen/CounterQuality.h"
#include "opendnp3/gen/DoubleBitBinaryQuality.h"
#include "opendnp3/gen/FrozenCounterQuality.h"

namespace opendnp3
{

/**
  Measurement Flags
*/
class Flags
{
public:
    Flags() : value(0) {}

    explicit Flags(uint8_t value) : value(value) {}

    inline bool IsSet(BinaryQuality flag) const
    {
        return IsSetAny(flag);
    }
    inline bool IsSet(DoubleBitBinaryQuality flag) const
    {
        return IsSetAny(flag);
    }
    inline bool IsSet(AnalogQuality flag) const
    {
        return IsSetAny(flag);
    }
    inline bool IsSet(CounterQuality flag) const
    {
        return IsSetAny(flag);
    }
    inline bool IsSet(FrozenCounterQuality flag) const
    {
        return IsSetAny(flag);
    }
    inline bool IsSet(BinaryOutputStatusQuality flag) const
    {
        return IsSetAny(flag);
    }
    inline bool IsSet(AnalogOutputStatusQuality flag) const
    {
        return IsSetAny(flag);
    }

    inline void Set(BinaryQuality flag)
    {
        SetAny(flag);
    }
    inline void Set(DoubleBitBinaryQuality flag)
    {
        SetAny(flag);
    }
    inline void Set(AnalogQuality flag)
    {
        SetAny(flag);
    }
    inline void Set(CounterQuality flag)
    {
        SetAny(flag);
    }
    inline void Set(FrozenCounterQuality flag)
    {
        SetAny(flag);
    }
    inline void Set(BinaryOutputStatusQuality flag)
    {
        SetAny(flag);
    }
    inline void Set(AnalogOutputStatusQuality flag)
    {
        SetAny(flag);
    }

    uint8_t value;

protected:
    template<class T> bool IsSetAny(T flag) const
    {
        return (value & static_cast<uint8_t>(flag)) != 0;
    }

    template<class T> void SetAny(T flag)
    {
        value |= static_cast<uint8_t>(flag);
    }
};

} // namespace opendnp3

#endif
