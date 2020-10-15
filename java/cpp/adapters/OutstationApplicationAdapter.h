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
#ifndef OPENDNP3_OUTSTATIONAPPLICATIONADAPTER_H
#define OPENDNP3_OUTSTATIONAPPLICATIONADAPTER_H


#include <opendnp3/outstation/IOutstationApplication.h>

#include "GlobalRef.h"
#include "../jni/JNIWrappers.h"

class OutstationApplicationAdapter : public opendnp3::IOutstationApplication
{
public:
    OutstationApplicationAdapter(jni::JOutstationApplication proxy) : proxy(proxy) {}

    bool SupportsWriteAbsoluteTime() override;

    bool WriteAbsoluteTime(const opendnp3::UTCTimestamp& timestamp) override;

    bool SupportsAssignClass() override;

    void RecordClassAssignment(opendnp3::AssignClassType type,
                               opendnp3::PointClass clazz,
                               uint16_t start,
                               uint16_t stop) override;

    opendnp3::ApplicationIIN GetApplicationIIN() const override;

    opendnp3::RestartMode ColdRestartSupport() const override;

    opendnp3::RestartMode WarmRestartSupport() const override;

    uint16_t ColdRestart() override;

    uint16_t WarmRestart() override;

    void OnConfirmProcessed(bool is_unsolicited, uint32_t num_class1, uint32_t num_class2, uint32_t num_class3) override;

    opendnp3::DNPTime Now() override;

private:
    GlobalRef<jni::JOutstationApplication> proxy;
};

#endif
