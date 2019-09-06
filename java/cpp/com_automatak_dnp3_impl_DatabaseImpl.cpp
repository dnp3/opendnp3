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
#include "com_automatak_dnp3_impl_DatabaseImpl.h"

#include "opendnp3/outstation/UpdateBuilder.h"

using namespace opendnp3;

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
 * Signature: (JZBJII)V
 */
JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_DatabaseImpl_update_1binary_1native(
    JNIEnv*, jobject, jlong native, jboolean value, jbyte flags, jlong time, jint index, jint mode)
{
    ((IUpdateHandler*)native)->Update(Binary(value != 0, Flags(flags), DNPTime(time)), static_cast<uint16_t>(index));
}

/*
 * Class:     com_automatak_dnp3_impl_DatabaseImpl
 * Method:    update_double_binary_native
 * Signature: (JIBJII)V
 */
JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_DatabaseImpl_update_1double_1binary_1native(
    JNIEnv*, jobject, jlong native, jint value, jbyte flags, jlong time, jint index, jint mode)
{
    ((IUpdateHandler*)native)->Update(DoubleBitBinary(static_cast<DoubleBit>(value), Flags(flags), DNPTime(time)), static_cast<uint16_t>(index), static_cast<EventMode>(mode));
}

/*
 * Class:     com_automatak_dnp3_impl_DatabaseImpl
 * Method:    update_analog_native
 * Signature: (JDBJII)V
 */
JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_DatabaseImpl_update_1analog_1native(
    JNIEnv*, jobject, jlong native, jdouble value, jbyte flags, jlong time, jint index, jint mode)
{    
    ((IUpdateHandler*)native) ->Update(Analog(value, Flags(flags), DNPTime(time)), static_cast<uint16_t>(index), static_cast<EventMode>(mode));
}

/*
 * Class:     com_automatak_dnp3_impl_DatabaseImpl
 * Method:    update_counter_native
 * Signature: (JJBJII)V
 */
JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_DatabaseImpl_update_1counter_1native(
    JNIEnv*, jobject, jlong native, jlong value, jbyte flags, jlong time, jint index, jint mode)
{
    ((IUpdateHandler*)native)->Update(Counter(static_cast<uint32_t>(value), Flags(flags), DNPTime(time)), static_cast<uint16_t>(index), static_cast<EventMode>(mode));
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
 * Signature: (JZBJII)V
 */
JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_DatabaseImpl_update_1bo_1status_1native(
    JNIEnv*, jobject, jlong native, jboolean value, jbyte flags, jlong time, jint index, jint mode)
{    
    ((IUpdateHandler*)native)->Update(BinaryOutputStatus(!(value == 0u), Flags(flags), DNPTime(time)), static_cast<uint16_t>(index), static_cast<EventMode>(mode));
}

/*
 * Class:     com_automatak_dnp3_impl_DatabaseImpl
 * Method:    update_ao_status_native
 * Signature: (JDBJII)V
 */
JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_DatabaseImpl_update_1ao_1status_1native(
    JNIEnv*, jobject, jlong native, jdouble value, jbyte flags, jlong time, jint index, jint mode)
{    
    ((IUpdateHandler*)native)->Update(AnalogOutputStatus(value, Flags(flags), DNPTime(time)), static_cast<uint16_t>(index), static_cast<EventMode>(mode));
}
