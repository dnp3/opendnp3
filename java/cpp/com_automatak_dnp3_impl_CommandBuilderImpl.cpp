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
#include "com_automatak_dnp3_impl_CommandBuilderImpl.h"

#include "adapters/JNI.h"
#include "jni/JCache.h"

#include "opendnp3/master/CommandSet.h"

using namespace opendnp3;

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_CommandBuilderImpl_create_1native(JNIEnv* /*env*/,
                                                                                       jobject /*unused*/)
{
    return (jlong) new CommandSet();
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_CommandBuilderImpl_destroy_1native(JNIEnv* /*env*/,
                                                                                       jobject /*unused*/,
                                                                                       jlong native)
{
    delete (CommandSet*)native;
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_CommandBuilderImpl_add_1crob_1native(JNIEnv* env,
                                                                                         jobject /*unused*/,
                                                                                         jlong native,
                                                                                         jobject jcommands)
{
    const auto set = (CommandSet*)native;

    std::vector<Indexed<ControlRelayOutputBlock>> commands;
    auto process = [&](jni::JIndexedValue indexed) {
        const auto jindex = jni::JCache::IndexedValue.getindex(env, indexed);
        const auto jcommand = jni::JCache::IndexedValue.getvalue(env, indexed);
        const auto crob = jcommand.as<jni::JControlRelayOutputBlock>();

        auto& ref = jni::JCache::ControlRelayOutputBlock;
        const auto opType = OperationTypeSpec::from_type(static_cast<uint8_t>(jni::JCache::OperationType.toType(env, ref.getopType(env, crob))));
        const auto tcc = TripCloseCodeSpec::from_type(static_cast<uint8_t>(jni::JCache::TripCloseCode.toType(env, ref.gettcc(env, crob))));
        const auto clear = ref.getclear(env, crob);
        const auto count = ref.getcount(env, crob);
        const auto onTime = ref.getonTimeMs(env, crob);
        const auto offTime = ref.getoffTimeMs(env, crob);
        const auto status = jni::JCache::CommandStatus.toType(env, ref.getstatus(env, crob));

        Indexed<ControlRelayOutputBlock> value(
            ControlRelayOutputBlock(opType, tcc, clear, static_cast<uint8_t>(count),
                                    static_cast<uint32_t>(onTime), static_cast<uint32_t>(offTime),
                                    CommandStatusSpec::from_type(static_cast<uint8_t>(status))),
            static_cast<uint16_t>(jindex));

        commands.push_back(value);
    };

    JNI::Iterate<jni::JIndexedValue>(env, jni::JIterable(jcommands), process);
    set->Add(commands);
}

template<class T, class J, class Cache> void process_analogs(JNIEnv* env, jlong native, jobject jcommands, Cache& cache)
{
    const auto set = (CommandSet*)native;

    std::vector<Indexed<T>> commands;
    auto process = [&](jni::JIndexedValue indexed) {
        const auto jindex = jni::JCache::IndexedValue.getindex(env, indexed);
        const auto jcommand = jni::JCache::IndexedValue.getvalue(env, indexed);

        const auto avalue = cache.getvalue(env, jcommand.as<J>());
        const auto status = jni::JCache::CommandStatus.toType(env, cache.getstatus(env, jcommand.as<J>()));

        Indexed<T> value(T(avalue, CommandStatusSpec::from_type(static_cast<uint8_t>(status))), static_cast<uint16_t>(jindex));

        commands.push_back(value);
    };

    JNI::Iterate<jni::JIndexedValue>(env, jni::JIterable(jcommands), process);
    set->Add(commands);
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_CommandBuilderImpl_add_1aoI16_1native(JNIEnv* env,
                                                                                          jobject /*unused*/,
                                                                                          jlong native,
                                                                                          jobject jcommands)
{
    process_analogs<AnalogOutputInt16, jni::JAnalogOutputInt16>(env, native, jcommands, jni::JCache::AnalogOutputInt16);
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_CommandBuilderImpl_add_1aoI32_1native(JNIEnv* env,
                                                                                          jobject /*unused*/,
                                                                                          jlong native,
                                                                                          jobject jcommands)
{
    process_analogs<AnalogOutputInt32, jni::JAnalogOutputInt32>(env, native, jcommands, jni::JCache::AnalogOutputInt32);
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_CommandBuilderImpl_add_1aoF32_1native(JNIEnv* env,
                                                                                          jobject /*unused*/,
                                                                                          jlong native,
                                                                                          jobject jcommands)
{
    process_analogs<AnalogOutputFloat32, jni::JAnalogOutputFloat32>(env, native, jcommands, jni::JCache::AnalogOutputFloat32);
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_CommandBuilderImpl_add_1aoD64_1native(JNIEnv* env,
                                                                                          jobject /*unused*/,
                                                                                          jlong native,
                                                                                          jobject jcommands)
{
    process_analogs<AnalogOutputDouble64, jni::JAnalogOutputDouble64>(env, native, jcommands, jni::JCache::AnalogOutputDouble64);
}
