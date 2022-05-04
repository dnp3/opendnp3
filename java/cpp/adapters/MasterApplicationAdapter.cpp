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
#include "MasterApplicationAdapter.h"

#include "../jni/JCache.h"

using namespace opendnp3;

void MasterApplicationAdapter::OnStateChange(LinkStatus value)
{
    const auto env = JNI::GetEnv();
    jni::JCache::LinkStatusListener.onStateChange(env, jni::JLinkStatusListener(proxy.get()), jni::JCache::LinkStatus.fromType(env, static_cast<jint>(value)));
}

void MasterApplicationAdapter::OnUnknownDestinationAddress(uint16_t destination)
{
    const auto env = JNI::GetEnv();
    jni::JCache::LinkStatusListener.onUnknownDestinationAddress(env, jni::JLinkStatusListener(proxy.get()), destination);
}

void MasterApplicationAdapter::OnUnknownSourceAddress(uint16_t source)
{
    const auto env = JNI::GetEnv();
    jni::JCache::LinkStatusListener.onUnknownSourceAddress(env, jni::JLinkStatusListener(proxy.get()), source);
}

void MasterApplicationAdapter::OnKeepAliveInitiated()
{
    const auto env = JNI::GetEnv();
    jni::JCache::LinkStatusListener.onKeepAliveInitiated(env, jni::JLinkStatusListener(proxy.get()));
}

void MasterApplicationAdapter::OnKeepAliveFailure()
{
    const auto env = JNI::GetEnv();
    jni::JCache::LinkStatusListener.onKeepAliveFailure(env, jni::JLinkStatusListener(proxy.get()));
}

void MasterApplicationAdapter::OnKeepAliveSuccess()
{
    const auto env = JNI::GetEnv();
    jni::JCache::LinkStatusListener.onKeepAliveSuccess(env, jni::JLinkStatusListener(proxy.get()));
}

opendnp3::UTCTimestamp MasterApplicationAdapter::Now()
{
    const auto env = JNI::GetEnv();
    const auto ms = jni::JCache::MasterApplication.getMillisecondsSinceEpoch(env, proxy);
    return opendnp3::UTCTimestamp(ms);
}

void MasterApplicationAdapter::OnReceiveIIN(const IINField& iin)
{
    const auto env = JNI::GetEnv();
    auto jiin = jni::JCache::IINField.construct(env, iin.LSB, iin.MSB);
    jni::JCache::MasterApplication.onReceiveIIN(env, proxy, jiin);
}

void MasterApplicationAdapter::OnTaskStart(MasterTaskType type, TaskId id)
{
    const auto env = JNI::GetEnv();
    auto jtasktype = jni::JCache::MasterTaskType.fromType(env, static_cast<jint>(type));
    auto jtaskid = jni::JCache::TaskId.construct(env, id.GetId(), static_cast<jboolean>(id.IsDefined()));
    jni::JCache::MasterApplication.onTaskStart(env, proxy, jtasktype, jtaskid);
}

void MasterApplicationAdapter::OnTaskComplete(const TaskInfo& info)
{
    const auto env = JNI::GetEnv();
    auto jtype = jni::JCache::MasterTaskType.fromType(env, static_cast<jint>(info.type));
    auto jresult = jni::JCache::TaskCompletion.fromType(env, static_cast<jint>(info.result));
    auto jtaskid = jni::JCache::TaskId.construct(env, info.id.GetId(), static_cast<jboolean>(info.id.IsDefined()));

    auto jinfo = jni::JCache::TaskInfo.construct(env, jtype, jresult, jtaskid);
    jni::JCache::MasterApplication.onTaskComplete(env, proxy, jinfo);
}

void MasterApplicationAdapter::OnOpen()
{
    const auto env = JNI::GetEnv();
    jni::JCache::MasterApplication.onOpen(env, proxy);
}

void MasterApplicationAdapter::OnClose()
{
    const auto env = JNI::GetEnv();
    jni::JCache::MasterApplication.onClose(env, proxy);
}

bool MasterApplicationAdapter::AssignClassDuringStartup()
{
    const auto env = JNI::GetEnv();
    return !(jni::JCache::MasterApplication.assignClassDuringStartup(env, proxy) == 0u);
}

void MasterApplicationAdapter::ConfigureAssignClassRequest(const WriteHeaderFunT& fun)
{
    auto env = JNI::GetEnv();

    auto jiterable = jni::JCache::MasterApplication.getClassAssignments(env, proxy);

    auto write = [&](jni::JClassAssignment assigment) {
        const auto clazz = static_cast<PointClass>(
            jni::JCache::PointClass.toType(env, jni::JCache::ClassAssignment.getclazz(env, assigment)));
        const auto jgroup = jni::JCache::ClassAssignment.getgroup(env, assigment);
        const auto jvariation = jni::JCache::ClassAssignment.getvariation(env, assigment);
        const auto jrange = jni::JCache::ClassAssignment.getrange(env, assigment);

        // write the group 60 header
        fun(Header::From(clazz));

        // write the header for the assigned type
        if (jni::JCache::Range.isDefined(env, jrange) != 0u)
        {
            const auto jstart = jni::JCache::Range.getstart(env, jrange);
            const auto jstop = jni::JCache::Range.getstop(env, jrange);

            fun(Header::Range16(jgroup, jvariation, static_cast<uint16_t>(jstart), static_cast<uint16_t>(jstop)));
        }
        else
        {
            fun(Header::AllObjects(jgroup, jvariation));
        }
    };

    JNI::Iterate<jni::JClassAssignment>(env, jiterable.get(), write);
}
