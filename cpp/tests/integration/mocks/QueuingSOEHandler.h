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

#ifndef OPENDNP3_INTEGRATIONTESTS_QUEUINGSOEHANDLER_H
#define OPENDNP3_INTEGRATIONTESTS_QUEUINGSOEHANDLER_H

#include "ExpectedValue.h"
#include "mocks/SynchronizedQueue.h"

#include <opendnp3/master/ISOEHandler.h>

#include <mutex>
#include <vector>

class QueuingSOEHandler final : public opendnp3::ISOEHandler
{
    std::mutex mutex;
    std::vector<ExpectedValue> temp;

    template<class T> void ProcessAny(const opendnp3::ICollection<opendnp3::Indexed<T>>& values)
    {
        auto add = [this](const opendnp3::Indexed<T>& item) { temp.push_back(ExpectedValue(item.value, item.index)); };

        values.ForeachItem(add);
    }

public:
    SynchronizedQueue<ExpectedValue> values;

    void Start() final
    {
        mutex.lock();
    }

    void End() final
    {
        values.AddMany(temp);
        temp.clear();
        mutex.unlock();
    }

    void Process(const opendnp3::HeaderInfo& info, const opendnp3::ICollection<opendnp3::Indexed<opendnp3::Binary>>& values) final
    {
        this->ProcessAny(values);
    }

    void Process(const opendnp3::HeaderInfo& info, const opendnp3::ICollection<opendnp3::Indexed<opendnp3::DoubleBitBinary>>& values) final
    {
        this->ProcessAny(values);
    }

    void Process(const opendnp3::HeaderInfo& info, const opendnp3::ICollection<opendnp3::Indexed<opendnp3::Analog>>& values) final
    {
        this->ProcessAny(values);
    }

    void Process(const opendnp3::HeaderInfo& info, const opendnp3::ICollection<opendnp3::Indexed<opendnp3::Counter>>& values) final
    {
        this->ProcessAny(values);
    }

    void Process(const opendnp3::HeaderInfo& info, const opendnp3::ICollection<opendnp3::Indexed<opendnp3::FrozenCounter>>& values) final
    {
        this->ProcessAny(values);
    }

    void Process(const opendnp3::HeaderInfo& info, const opendnp3::ICollection<opendnp3::Indexed<opendnp3::BinaryOutputStatus>>& values) final
    {
        this->ProcessAny(values);
    }

    void Process(const opendnp3::HeaderInfo& info, const opendnp3::ICollection<opendnp3::Indexed<opendnp3::AnalogOutputStatus>>& values) final
    {
        this->ProcessAny(values);
    }

    void Process(const opendnp3::HeaderInfo& info,
                         const opendnp3::ICollection<opendnp3::DNPTime>& values) final
    {
    }

    void Process(const opendnp3::HeaderInfo& info, const opendnp3::ICollection<opendnp3::Indexed<opendnp3::OctetString>>& values) final {}
    void Process(const opendnp3::HeaderInfo& info, const opendnp3::ICollection<opendnp3::Indexed<opendnp3::BinaryCommandEvent>>& values) final {}
    void Process(const opendnp3::HeaderInfo& info, const opendnp3::ICollection<opendnp3::Indexed<opendnp3::AnalogCommandEvent>>& values) final {}
    void Process(const opendnp3::HeaderInfo& info, const opendnp3::ICollection<opendnp3::Indexed<opendnp3::TimeAndInterval>>& values) final {}
    void Process(const opendnp3::HeaderInfo& info, const opendnp3::ICollection<opendnp3::Indexed<opendnp3::SecurityStat>>& values) final {}
};

#endif
