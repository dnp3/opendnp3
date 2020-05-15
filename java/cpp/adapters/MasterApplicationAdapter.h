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
#ifndef OPENDNP3_MASTERAPPLICATIONADAPTER_H
#define OPENDNP3_MASTERAPPLICATIONADAPTER_H

#include <opendnp3/gen/PointClass.h>
#include <opendnp3/master/IMasterApplication.h>

#include "../jni/JNIWrappers.h"
#include "GlobalRef.h"

class MasterApplicationAdapter final : public opendnp3::IMasterApplication
{
public:
    MasterApplicationAdapter(jni::JMasterApplication proxy) : proxy(proxy) {}

    virtual opendnp3::UTCTimestamp Now() override;

    void OnReceiveIIN(const opendnp3::IINField& iin) override;
    void OnTaskStart(opendnp3::MasterTaskType type, opendnp3::TaskId id) override;
    void OnTaskComplete(const opendnp3::TaskInfo& info) override;
    void OnOpen() override;
    void OnClose() override;
    bool AssignClassDuringStartup() override;
    void ConfigureAssignClassRequest(const opendnp3::WriteHeaderFunT& fun) override;

private:
    GlobalRef<jni::JMasterApplication> proxy;
};

#endif
