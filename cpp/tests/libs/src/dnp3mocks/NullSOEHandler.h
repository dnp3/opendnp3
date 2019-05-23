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
#ifndef DNP3MOCKS_NULLSOEHANDLER_H
#define DNP3MOCKS_NULLSOEHANDLER_H

#include <opendnp3/master/ISOEHandler.h>

#include <memory>

namespace opendnp3
{

class NullSOEHandler : public ISOEHandler
{

public:
    NullSOEHandler() = default;

    static std::shared_ptr<ISOEHandler> Create()
    {
        return std::make_shared<NullSOEHandler>();
    }

    virtual void Process(const HeaderInfo& info, const ICollection<Indexed<Binary>>& values) override final {}
    virtual void Process(const HeaderInfo& info, const ICollection<Indexed<DoubleBitBinary>>& values) override final {}
    virtual void Process(const HeaderInfo& info, const ICollection<Indexed<Analog>>& values) override final {}
    virtual void Process(const HeaderInfo& info, const ICollection<Indexed<Counter>>& values) override final {}
    virtual void Process(const HeaderInfo& info, const ICollection<Indexed<FrozenCounter>>& values) override final {}
    virtual void Process(const HeaderInfo& info, const ICollection<Indexed<BinaryOutputStatus>>& values) override final
    {
    }
    virtual void Process(const HeaderInfo& info, const ICollection<Indexed<AnalogOutputStatus>>& values) override final
    {
    }
    virtual void Process(const HeaderInfo& info, const ICollection<Indexed<OctetString>>& values) override final {}
    virtual void Process(const HeaderInfo& info, const ICollection<Indexed<TimeAndInterval>>& values) override final {}
    virtual void Process(const HeaderInfo& info, const ICollection<Indexed<BinaryCommandEvent>>& values) override final
    {
    }
    virtual void Process(const HeaderInfo& info, const ICollection<Indexed<AnalogCommandEvent>>& values) override final
    {
    }
    virtual void Process(const HeaderInfo& info, const ICollection<Indexed<SecurityStat>>& values) override final {}
    virtual void Process(const HeaderInfo& info, const ICollection<DNPTime>& values) override final {}

protected:
    void Start() override final {}
    void End() override final {}

private:
    static NullSOEHandler instance;
};

} // namespace opendnp3

#endif
