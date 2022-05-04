/*
 * Copyright 2013-2022 Step Function I/O, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Step Function I/O
 * LLC (www.automatak.com) under one or more contributor license agreements.
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
#ifndef OPENDNP3_SOEHANDLERADAPTER_H
#define OPENDNP3_SOEHANDLERADAPTER_H


#include <opendnp3/master/ISOEHandler.h>

#include "GlobalRef.h"
#include "LocalRef.h"

#include "../jni/JNIWrappers.h"

class SOEHandlerAdapter final : public opendnp3::ISOEHandler
{
public:
    SOEHandlerAdapter(jni::JSOEHandler proxy) : proxy(proxy) {}

    void BeginFragment(const opendnp3::ResponseInfo& info) override;

    void EndFragment(const opendnp3::ResponseInfo& info) override;

    void Process(const opendnp3::HeaderInfo& info,
                         const opendnp3::ICollection<opendnp3::Indexed<opendnp3::Binary>>& values) override;
    void Process(const opendnp3::HeaderInfo& info,
                         const opendnp3::ICollection<opendnp3::Indexed<opendnp3::DoubleBitBinary>>& values) override;
    void Process(const opendnp3::HeaderInfo& info,
                         const opendnp3::ICollection<opendnp3::Indexed<opendnp3::Analog>>& values) override;
    void Process(const opendnp3::HeaderInfo& info,
                         const opendnp3::ICollection<opendnp3::Indexed<opendnp3::Counter>>& values) override;
    void Process(const opendnp3::HeaderInfo& info,
                         const opendnp3::ICollection<opendnp3::Indexed<opendnp3::FrozenCounter>>& values) override;
    void Process(const opendnp3::HeaderInfo& info,
                         const opendnp3::ICollection<opendnp3::Indexed<opendnp3::BinaryOutputStatus>>& values) override;
    void Process(const opendnp3::HeaderInfo& info,
                         const opendnp3::ICollection<opendnp3::Indexed<opendnp3::AnalogOutputStatus>>& values) override;
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
    
    void Process(const opendnp3::HeaderInfo& info,
                         const opendnp3::ICollection<opendnp3::DNPTime>& values) override;

private:
    template<class T, class CreateMeas, class CallProxy>
    void Process(const opendnp3::HeaderInfo& info,
                 const opendnp3::ICollection<opendnp3::Indexed<T>>& values,
                 const CreateMeas& createMeas,
                 const CallProxy& callProxy);

    static LocalRef<jni::JHeaderInfo> Convert(JNIEnv* env, const opendnp3::HeaderInfo& info);
    static LocalRef<jni::JDNPTime> Convert(JNIEnv* env, const opendnp3::DNPTime& time);

    GlobalRef<jni::JSOEHandler> proxy;
};

#endif
