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
#ifndef OPENDNP3_CLAZZCOUNT_H
#define OPENDNP3_CLAZZCOUNT_H

#include "EventState.h"

#include "opendnp3/app/EventType.h"

namespace opendnp3
{

struct ClazzCount
{

public:
    void Reset()
    {
        this->num_class_1 = 0;
        this->num_class_2 = 0;
        this->num_class_3 = 0;
    }

    uint32_t Get(EventClass clazz) const
    {
        switch (clazz)
        {
        case (EventClass::EC1):
            return num_class_1;
        case (EventClass::EC2):
            return num_class_2;
            break;
        default:
            return num_class_3;
        }
    }

    void Increment(EventClass clazz)
    {
        switch (clazz)
        {
        case (EventClass::EC1):
            ++num_class_1;
            break;
        case (EventClass::EC2):
            ++num_class_2;
            break;
        default:
            ++num_class_3;
            break;
        }
    }

    void Decrement(EventClass clazz)
    {
        switch (clazz)
        {
        case (EventClass::EC1):
            --num_class_1;
            break;
        case (EventClass::EC2):
            --num_class_2;
            break;
        default:
            --num_class_3;
            break;
        }
    }

    bool Any() const
    {
        return (num_class_1 > 0) || (num_class_2 > 0) || (num_class_3 > 0);
    }

private:
    uint32_t num_class_1 = 0;
    uint32_t num_class_2 = 0;
    uint32_t num_class_3 = 0;
};

class EventClassCounters
{

public:
    ClazzCount total;
    ClazzCount written;
    uint32_t selected = 0;

    void OnAdd(EventClass clazz)
    {
        this->total.Increment(clazz);
    }

    void OnSelect()
    {
        ++selected;
    }

    void OnWrite(EventClass clazz)
    {
        // only selected events are written
        --selected;
        this->written.Increment(clazz);
    }

    void ResetOnFail()
    {
        this->selected = 0;
        this->written.Reset();
    }

    void OnRemove(EventClass clazz, EventState state)
    {
        switch (state)
        {
        case (EventState::selected):
            --selected;
            break;
        case (EventState::written):
            this->written.Decrement(clazz);
            break;
        default:
            break;
        }

        this->total.Decrement(clazz);
    }
};
} // namespace opendnp3

#endif
