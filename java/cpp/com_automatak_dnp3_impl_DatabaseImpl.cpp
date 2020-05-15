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
#include "com_automatak_dnp3_impl_DatabaseImpl.h"

#include "opendnp3/outstation/UpdateBuilder.h"

#include "jni/JCache.h"

using namespace opendnp3;

DNPTime convertDnpTime(JNIEnv* env, jobject jtime)
{
    const auto time = jni::JCache::DNPTime.getmsSinceEpoch(env, jtime);
    const auto jquality = jni::JCache::DNPTime.getquality(env, jtime);
    const auto quality = static_cast<TimestampQuality>(jni::JCache::TimestampQuality.toType(env, jquality));
    return DNPTime(time, quality);
}

/*
 * Class:     com_automatak_dnp3_impl_DatabaseImpl
 * Method:    new_update_builder_native
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_DatabaseImpl_new_1update_1builder_1native(JNIEnv*, jclass)
{
    return (jlong) new UpdateBuilder();
}

/*
 * Class:     com_automatak_dnp3_impl_DatabaseImpl
 * Method:    delete_update_builder_native
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_DatabaseImpl_delete_1update_1builder_1native(JNIEnv*,
                                                                                                 jclass,
                                                                                                 jlong native)
{
    delete (UpdateBuilder*)native;
}

/*
 * Class:     com_automatak_dnp3_impl_DatabaseImpl
 * Method:    update_binary_native
 * Signature: (JZBLcom/automatak/dnp3/DNPTime;II)V
 */
JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_DatabaseImpl_update_1binary_1native(
    JNIEnv* env, jobject, jlong native, jboolean value, jbyte flags, jobject time, jint index, jint mode)
{
    ((IUpdateHandler*)native)->Update(Binary(value != 0, Flags(flags), convertDnpTime(env, time)), static_cast<uint16_t>(index), static_cast<EventMode>(mode));
}

/*
 * Class:     com_automatak_dnp3_impl_DatabaseImpl
 * Method:    update_double_binary_native
 * Signature: (JIBLcom/automatak/dnp3/DNPTime;II)V
 */
JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_DatabaseImpl_update_1double_1binary_1native(
    JNIEnv* env, jobject, jlong native, jint value, jbyte flags, jobject time, jint index, jint mode)
{
    ((IUpdateHandler*)native)->Update(DoubleBitBinary(static_cast<DoubleBit>(value), Flags(flags), convertDnpTime(env, time)), static_cast<uint16_t>(index), static_cast<EventMode>(mode));
}

/*
 * Class:     com_automatak_dnp3_impl_DatabaseImpl
 * Method:    update_analog_native
 * Signature: (JDBLcom/automatak/dnp3/DNPTime;II)V
 */
JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_DatabaseImpl_update_1analog_1native(
    JNIEnv* env, jobject, jlong native, jdouble value, jbyte flags, jobject time, jint index, jint mode)
{    
    ((IUpdateHandler*)native)->Update(Analog(value, Flags(flags), convertDnpTime(env, time)), static_cast<uint16_t>(index), static_cast<EventMode>(mode));
}

/*
 * Class:     com_automatak_dnp3_impl_DatabaseImpl
 * Method:    update_counter_native
 * Signature: (JJBLcom/automatak/dnp3/DNPTime;II)V
 */
JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_DatabaseImpl_update_1counter_1native(
    JNIEnv* env, jobject, jlong native, jlong value, jbyte flags, jobject time, jint index, jint mode)
{
    ((IUpdateHandler*)native)->Update(Counter(static_cast<uint32_t>(value), Flags(flags), convertDnpTime(env, time)), static_cast<uint16_t>(index), static_cast<EventMode>(mode));
}

/*
 * Class:     com_automatak_dnp3_impl_DatabaseImpl
 * Method:    freeze_counter_native
 * Signature: (JIZI)V
 */
JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_DatabaseImpl_freeze_1counter_1native(JNIEnv*, jobject, jlong native, jint index, jboolean clear, jint mode)
{        
    ((IUpdateHandler*)native)->FreezeCounter(static_cast<uint16_t>(index), clear, static_cast<EventMode>(mode));
}

/*
 * Class:     com_automatak_dnp3_impl_DatabaseImpl
 * Method:    update_bo_status_native
 * Signature: (JZBLcom/automatak/dnp3/DNPTime;II)V
 */
JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_DatabaseImpl_update_1bo_1status_1native(
    JNIEnv* env, jobject, jlong native, jboolean value, jbyte flags, jobject time, jint index, jint mode)
{    
    ((IUpdateHandler*)native)->Update(BinaryOutputStatus(!(value == 0u), Flags(flags), convertDnpTime(env, time)), static_cast<uint16_t>(index), static_cast<EventMode>(mode));
}

/*
 * Class:     com_automatak_dnp3_impl_DatabaseImpl
 * Method:    update_ao_status_native
 * Signature: (JDBLcom/automatak/dnp3/DNPTime;II)V
 */
JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_DatabaseImpl_update_1ao_1status_1native(
    JNIEnv* env, jobject, jlong native, jdouble value, jbyte flags, jobject time, jint index, jint mode)
{
    
    ((IUpdateHandler*)native)->Update(AnalogOutputStatus(value, Flags(flags), convertDnpTime(env, time)), static_cast<uint16_t>(index), static_cast<EventMode>(mode));
}
