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
#include "com_automatak_dnp3_impl_MasterImpl.h"

#include "adapters/Conversions.h"
#include "adapters/GlobalRef.h"
#include "adapters/SOEHandlerAdapter.h"
#include "jni/JCache.h"

#include <opendnp3/master/CommandSet.h>
#include <opendnp3/master/IMaster.h>

#include <memory>

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_MasterImpl_set_1log_1level_1native(JNIEnv* /*env*/,
                                                                                       jobject /*unused*/,
                                                                                       jlong native,
                                                                                       jint levels)
{
    const auto master = (std::shared_ptr<opendnp3::IMaster>*)native;
    (*master)->SetLogFilters(opendnp3::LogLevel(levels));
}

JNIEXPORT jobject JNICALL Java_com_automatak_dnp3_impl_MasterImpl_get_1statistics_1native(JNIEnv* env,
                                                                                          jobject /*unused*/,
                                                                                          jlong native)
{
    const auto master = (std::shared_ptr<opendnp3::IMaster>*)native;
    auto stats = (*master)->GetStackStatistics();
    return env->NewGlobalRef(Conversions::ConvertStackStatistics(env, stats).get());
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_MasterImpl_enable_1native(JNIEnv* /*env*/,
                                                                              jobject /*unused*/,
                                                                              jlong native)
{
    const auto master = (std::shared_ptr<opendnp3::IMaster>*)native;
    (*master)->Enable();
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_MasterImpl_disable_1native(JNIEnv* /*env*/,
                                                                               jobject /*unused*/,
                                                                               jlong native)
{
    const auto master = (std::shared_ptr<opendnp3::IMaster>*)native;
    (*master)->Disable();
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_MasterImpl_shutdown_1native(JNIEnv* /*env*/,
                                                                                jobject /*unused*/,
                                                                                jlong native)
{
    const auto master = (std::shared_ptr<opendnp3::IMaster>*)native;
    (*master)->Shutdown();
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_MasterImpl_destroy_1native(JNIEnv* /*unused*/,
                                                                               jobject /*unused*/,
                                                                               jlong native)
{
    const auto master = (std::shared_ptr<opendnp3::IMaster>*)native;
    delete master;
}

template<class Fun>
void operate(JNIEnv* /*env*/, jlong native, jlong nativeCommandSet, jobject future, const Fun& operate)
{
    auto& set = *(opendnp3::CommandSet*)nativeCommandSet;
   
    auto callback = [jfuture = std::make_shared<GlobalRef<jni::JCompletableFuture>>(future)](const opendnp3::ICommandTaskResult& result) {
        const auto env = JNI::GetEnv();
        const auto jsummary = jni::JCache::TaskCompletion.fromType(env, static_cast<jint>(result.summary));
        const auto jlist = jni::JCache::ArrayList.construct(env, static_cast<jint>(result.Count()));

        auto addToJList = [&](const opendnp3::CommandPointResult& cpr) {
            const auto jstate = jni::JCache::CommandPointState.fromType(env, static_cast<jint>(cpr.state));
            const auto jstatus = jni::JCache::CommandStatus.fromType(env, static_cast<jint>(cpr.status));
            const auto jres = jni::JCache::CommandPointResult.construct(env, cpr.headerIndex, cpr.index, jstate, jstatus);
            jni::JCache::ArrayList.add(env, jlist, jres.as<jni::JObject>());
        };

        result.ForeachItem(addToJList);

        const auto jtaskresult = jni::JCache::CommandTaskResult.construct(env, jsummary, jlist.as<jni::JIterable>());

        jni::JCache::CompletableFuture.complete(env, *jfuture, jtaskresult.as<jni::JObject>()); // invoke the future
    };

    const auto master = (std::shared_ptr<opendnp3::IMaster>*)native;
    operate(**master, set, callback);
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_MasterImpl_select_1and_1operate_1native(
    JNIEnv* env, jobject /*unused*/, jlong native, jlong nativeCommandSet, jobject future)
{
    auto sbo = [](opendnp3::IMaster& master, opendnp3::CommandSet& commandset,
                  const opendnp3::CommandResultCallbackT& callback) -> void {
        master.SelectAndOperate(std::move(commandset), callback);
    };

    operate(env, native, nativeCommandSet, future, sbo);
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_MasterImpl_direct_1operate_1native(
    JNIEnv* env, jobject /*unused*/, jlong native, jlong nativeCommandSet, jobject future)
{
    auto directOp = [](opendnp3::IMaster& master, opendnp3::CommandSet& commandset,
                       const opendnp3::CommandResultCallbackT& callback) -> void {
        master.DirectOperate(std::move(commandset), callback);
    };

    operate(env, native, nativeCommandSet, future, directOp);
}

bool ConvertJHeader(JNIEnv* env, jni::JHeader jheader, opendnp3::Header& header)
{
    const auto group = jni::JCache::Header.getgroup(env, jheader);
    const auto var = jni::JCache::Header.getvariation(env, jheader);
    const auto qualifier = opendnp3::QualifierCodeSpec::from_type(
        static_cast<uint8_t>(jni::JCache::QualifierCode.toType(env, jni::JCache::Header.getqualifier(env, jheader))));
    const auto count = jni::JCache::Header.getcount(env, jheader);
    const auto start = jni::JCache::Header.getstart(env, jheader);
    const auto stop = jni::JCache::Header.getstop(env, jheader);

    switch (qualifier)
    {
    case (opendnp3::QualifierCode::ALL_OBJECTS):
        header = opendnp3::Header::AllObjects(group, var);
        return true;
    case (opendnp3::QualifierCode::UINT8_CNT):
        header = opendnp3::Header::Count8(group, var, static_cast<uint8_t>(count));
        return true;
    case (opendnp3::QualifierCode::UINT16_CNT):
        header = opendnp3::Header::Count16(group, var, static_cast<uint16_t>(count));
        return true;
    case (opendnp3::QualifierCode::UINT8_START_STOP):
        header = opendnp3::Header::Range8(group, var, static_cast<uint8_t>(start), static_cast<uint8_t>(stop));
        return true;
    case (opendnp3::QualifierCode::UINT16_START_STOP):
        header = opendnp3::Header::Range16(group, var, static_cast<uint16_t>(start), static_cast<uint16_t>(stop));
        return true;
    default:
        return false;
    }
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_MasterImpl_scan_1native(JNIEnv* env,
                                                                            jobject /*unused*/,
                                                                            jlong native,
                                                                            jobject jheaders,
                                                                            jobject jsoehandler)
{
    const auto master = (std::shared_ptr<opendnp3::IMaster>*)native;

    std::vector<opendnp3::Header> headers;

    auto process = [&](jni::JHeader jheader) {
        opendnp3::Header header;
        if (ConvertJHeader(env, jheader, header))
        {
            headers.push_back(header);
        }
    };

    JNI::Iterate<jni::JHeader>(env, jni::JIterable(jheaders), process);

    auto soeAdapter = std::make_shared<SOEHandlerAdapter>(jsoehandler);

    (*master)->Scan(headers, soeAdapter);
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_MasterImpl_add_1periodic_1scan_1native(
    JNIEnv* env, jobject /*unused*/, jlong native, jobject jduration, jobject jheaders, jobject jsoehandler)
{
    const auto master = (std::shared_ptr<opendnp3::IMaster>*)native;

    std::vector<opendnp3::Header> headers;

    auto process = [&](jni::JHeader jheader) {
        opendnp3::Header header;
        if (ConvertJHeader(env, jheader, header))
        {
            headers.push_back(header);
        }
    };

    JNI::Iterate<jni::JHeader>(env, jni::JIterable(jheaders), process);

    auto period = opendnp3::TimeDuration::Milliseconds(jni::JCache::Duration.toMillis(env, jduration));

    auto soeAdapter = std::make_shared<SOEHandlerAdapter>(jsoehandler);

    (*master)->AddScan(period, headers, soeAdapter);
}