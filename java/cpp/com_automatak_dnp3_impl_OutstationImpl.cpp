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
#include "com_automatak_dnp3_impl_OutstationImpl.h"

#include "adapters/Conversions.h"

#include <opendnp3/outstation/IOutstation.h>
#include <opendnp3/outstation/UpdateBuilder.h>

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_OutstationImpl_set_1log_1level_1native(JNIEnv* /*env*/,
                                                                                           jobject /*unused*/,
                                                                                           jlong native,
                                                                                           jint levels)
{
    auto outstation = (std::shared_ptr<opendnp3::IOutstation>*)native;
    (*outstation)->SetLogFilters(opendnp3::LogLevel(levels));
}

JNIEXPORT jobject JNICALL Java_com_automatak_dnp3_impl_OutstationImpl_get_1statistics_1native(JNIEnv* env,
                                                                                              jobject /*unused*/,
                                                                                              jlong native)
{
    auto outstation = (std::shared_ptr<opendnp3::IOutstation>*)native;
    auto stats = (*outstation)->GetStackStatistics();
    return env->NewGlobalRef(Conversions::ConvertStackStatistics(env, stats).get());
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_OutstationImpl_enable_1native(JNIEnv* /*env*/,
                                                                                  jobject /*unused*/,
                                                                                  jlong native)
{
    auto outstation = (std::shared_ptr<opendnp3::IOutstation>*)native;
    (*outstation)->Enable();
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_OutstationImpl_disable_1native(JNIEnv* /*env*/,
                                                                                   jobject /*unused*/,
                                                                                   jlong native)
{
    auto outstation = (std::shared_ptr<opendnp3::IOutstation>*)native;
    (*outstation)->Disable();
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_OutstationImpl_shutdown_1native(JNIEnv* /*env*/,
                                                                                    jobject /*unused*/,
                                                                                    jlong native)
{
    auto outstation = (std::shared_ptr<opendnp3::IOutstation>*)native;
    (*outstation)->Shutdown();
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_OutstationImpl_destroy_1native(JNIEnv* /*unused*/,
                                                                                   jobject /*unused*/,
                                                                                   jlong native)
{
    auto outstation = (std::shared_ptr<opendnp3::IOutstation>*)native;
    delete outstation;
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_OutstationImpl_apply_1native(JNIEnv* /*env*/,
                                                                                 jobject /*unused*/,
                                                                                 jlong native,
                                                                                 jlong nativeChangeSet)
{
    auto outstation = (std::shared_ptr<opendnp3::IOutstation>*)native;
    auto builder = (opendnp3::UpdateBuilder*)nativeChangeSet;
    (*outstation)->Apply(builder->Build());
}
