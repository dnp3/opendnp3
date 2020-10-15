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

#include "OutstationApplicationAdapter.h"

#include "../jni/JCache.h"

using namespace jni;
using namespace opendnp3;

bool OutstationApplicationAdapter::SupportsWriteAbsoluteTime()
{
    const auto env = JNI::GetEnv();
    return !(JCache::OutstationApplication.supportsWriteAbsoluteTime(env, proxy) == 0u);
}

bool OutstationApplicationAdapter::WriteAbsoluteTime(const opendnp3::UTCTimestamp& timestamp)
{
    const auto env = JNI::GetEnv();
    return !(JCache::OutstationApplication.writeAbsoluteTime(env, proxy, timestamp.msSinceEpoch) == 0u);
}

bool OutstationApplicationAdapter::SupportsAssignClass()
{
    const auto env = JNI::GetEnv();
    return !(JCache::OutstationApplication.supportsAssignClass(env, proxy) == 0u);
}

void OutstationApplicationAdapter::RecordClassAssignment(AssignClassType type,
                                                         PointClass clazz,
                                                         uint16_t start,
                                                         uint16_t stop)
{
    const auto env = JNI::GetEnv();

    auto jtype = JCache::AssignClassType.fromType(env, static_cast<jint>(type));
    auto jclazz = JCache::PointClass.fromType(env, static_cast<jint>(clazz));

    JCache::OutstationApplication.recordClassAssignment(env, proxy, jtype, jclazz, start, stop);
}

ApplicationIIN OutstationApplicationAdapter::GetApplicationIIN() const
{
    const auto env = JNI::GetEnv();
    auto jiin = JCache::OutstationApplication.getApplicationIIN(env, proxy);
    ApplicationIIN iin;
    iin.configCorrupt = !(JCache::ApplicationIIN.getconfigCorrupt(env, jiin) == 0u);
    iin.deviceTrouble = !(JCache::ApplicationIIN.getdeviceTrouble(env, jiin) == 0u);
    iin.localControl = !(JCache::ApplicationIIN.getlocalControl(env, jiin) == 0u);
    iin.needTime = !(JCache::ApplicationIIN.getneedTime(env, jiin) == 0u);
    return iin;
}

RestartMode OutstationApplicationAdapter::ColdRestartSupport() const
{
    const auto env = JNI::GetEnv();
    const auto jmode = JCache::OutstationApplication.coldRestartSupport(env, proxy);
    const auto jvalue = JCache::RestartMode.toType(env, jmode);
    return static_cast<RestartMode>(jvalue);
}

RestartMode OutstationApplicationAdapter::WarmRestartSupport() const
{
    const auto env = JNI::GetEnv();
    const auto jmode = JCache::OutstationApplication.warmRestartSupport(env, proxy);
    const auto jvalue = JCache::RestartMode.toType(env, jmode);
    return static_cast<RestartMode>(jvalue);
}

uint16_t OutstationApplicationAdapter::ColdRestart()
{
    const auto env = JNI::GetEnv();
    return static_cast<uint16_t>(JCache::OutstationApplication.coldRestart(env, proxy));
}

uint16_t OutstationApplicationAdapter::WarmRestart()
{
    const auto env = JNI::GetEnv();
    return static_cast<uint16_t>(JCache::OutstationApplication.warmRestart(env, proxy));
}

void OutstationApplicationAdapter::OnConfirmProcessed(bool is_unsolicited, uint32_t num_class1, uint32_t num_class2, uint32_t num_class3)
{
    const auto env = JNI::GetEnv();
    JCache::OutstationApplication.onConfirmProcessed(env, proxy, is_unsolicited, num_class1, num_class2, num_class3);
}

DNPTime OutstationApplicationAdapter::Now()
{
    const auto env = JNI::GetEnv();
    const auto dnp_time = JCache::OutstationApplication.now(env, proxy);
    const auto time = JCache::DNPTime.getmsSinceEpoch(env, dnp_time);
    const auto quality = JCache::DNPTime.getquality(env, dnp_time);
    return DNPTime(time, static_cast<TimestampQuality>(JCache::TimestampQuality.toType(env, quality)));
}
