/*
 * Copyright 2013-2022 Step Function I/O, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Step Function I/O
 * LLC (https://stepfunc.io) under one or more contributor license agreements.
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Step Function I/O LLC license
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
#ifndef OPENDNP3_INTEGRATIONTESTS_NULLSOEHANDLER_H
#define OPENDNP3_INTEGRATIONTESTS_NULLSOEHANDLER_H

#include <opendnp3/master/ISOEHandler.h>

#include <memory>

class NullSOEHandler final : public opendnp3::ISOEHandler
{

public:
    NullSOEHandler() = default;

    static std::shared_ptr<ISOEHandler> Create()
    {
        return std::make_shared<NullSOEHandler>();
    }

    void BeginFragment(const opendnp3::ResponseInfo& info) override {}

    void EndFragment(const opendnp3::ResponseInfo& info) override {}

    void Process(const opendnp3::HeaderInfo& info,
                 const opendnp3::ICollection<opendnp3::Indexed<opendnp3::Binary>>& values) override
    {
    }
    void Process(const opendnp3::HeaderInfo& info,
                 const opendnp3::ICollection<opendnp3::Indexed<opendnp3::DoubleBitBinary>>& values) override
    {
    }
    void Process(const opendnp3::HeaderInfo& info,
                 const opendnp3::ICollection<opendnp3::Indexed<opendnp3::Analog>>& values) override
    {
    }
    void Process(const opendnp3::HeaderInfo& info,
                 const opendnp3::ICollection<opendnp3::Indexed<opendnp3::Counter>>& values) override
    {
    }
    void Process(const opendnp3::HeaderInfo& info,
                 const opendnp3::ICollection<opendnp3::Indexed<opendnp3::FrozenCounter>>& values) override
    {
    }
    void Process(const opendnp3::HeaderInfo& info,
                 const opendnp3::ICollection<opendnp3::Indexed<opendnp3::BinaryOutputStatus>>& values) override
    {
    }
    void Process(const opendnp3::HeaderInfo& info,
                 const opendnp3::ICollection<opendnp3::Indexed<opendnp3::AnalogOutputStatus>>& values) override
    {
    }
    void Process(const opendnp3::HeaderInfo& info,
                 const opendnp3::ICollection<opendnp3::Indexed<opendnp3::OctetString>>& values) override
    {
    }
    void Process(const opendnp3::HeaderInfo& info,
                 const opendnp3::ICollection<opendnp3::Indexed<opendnp3::TimeAndInterval>>& values) override
    {
    }
    void Process(const opendnp3::HeaderInfo& info,
                 const opendnp3::ICollection<opendnp3::Indexed<opendnp3::BinaryCommandEvent>>& values) override
    {
    }
    void Process(const opendnp3::HeaderInfo& info,
                 const opendnp3::ICollection<opendnp3::Indexed<opendnp3::AnalogCommandEvent>>& values) override
    {
    }

    void Process(const opendnp3::HeaderInfo& info, const opendnp3::ICollection<opendnp3::DNPTime>& values) override {}

private:
    static NullSOEHandler instance;
};

#endif
