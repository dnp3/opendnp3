//
//  _   _         ______    _ _ _   _             _ _ _
// | \ | |       |  ____|  | (_) | (_)           | | | |
// |  \| | ___   | |__   __| |_| |_ _ _ __   __ _| | | |
// | . ` |/ _ \  |  __| / _` | | __| | '_ \ / _` | | | |
// | |\  | (_) | | |___| (_| | | |_| | | | | (_| |_|_|_|
// |_| \_|\___/  |______\__,_|_|\__|_|_| |_|\__, (_|_|_)
//                                           __/ |
//                                          |___/
// 
// This file is auto-generated. Do not edit manually
// 
// Copyright 2013-2019 Automatak, LLC
// 
// Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
// LLC (www.automatak.com) under one or more contributor license agreements.
// See the NOTICE file distributed with this work for additional information
// regarding copyright ownership. Green Energy Corp and Automatak LLC license
// this file to you under the Apache License, Version 2.0 (the "License"); you
// may not use this file except in compliance with the License. You may obtain
// a copy of the License at:
// 
//   http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef OPENDNP3JAVA_JNICOMMANDPROCESSOR_H
#define OPENDNP3JAVA_JNICOMMANDPROCESSOR_H

#include <jni.h>

#include "../adapters/LocalRef.h"

namespace jni
{
    struct JCache;

    namespace cache
    {
        class CommandProcessor
        {
            friend struct jni::JCache;

            bool init(JNIEnv* env);
            void cleanup(JNIEnv* env);

            public:

            // methods
            LocalRef<jobject> directOperate(JNIEnv* env, jobject instance, jobject arg0);
            LocalRef<jobject> directOperateAODouble64(JNIEnv* env, jobject instance, jobject arg0, jint arg1);
            LocalRef<jobject> directOperateAOFloat32(JNIEnv* env, jobject instance, jobject arg0, jint arg1);
            LocalRef<jobject> directOperateAOInt16(JNIEnv* env, jobject instance, jobject arg0, jint arg1);
            LocalRef<jobject> directOperateAOInt32(JNIEnv* env, jobject instance, jobject arg0, jint arg1);
            LocalRef<jobject> directOperateCROB(JNIEnv* env, jobject instance, jobject arg0, jint arg1);
            LocalRef<jobject> selectAndOperate(JNIEnv* env, jobject instance, jobject arg0);
            LocalRef<jobject> selectAndOperateAODouble64(JNIEnv* env, jobject instance, jobject arg0, jint arg1);
            LocalRef<jobject> selectAndOperateAOFloat32(JNIEnv* env, jobject instance, jobject arg0, jint arg1);
            LocalRef<jobject> selectAndOperateAOInt16(JNIEnv* env, jobject instance, jobject arg0, jint arg1);
            LocalRef<jobject> selectAndOperateAOInt32(JNIEnv* env, jobject instance, jobject arg0, jint arg1);
            LocalRef<jobject> selectAndOperateCROB(JNIEnv* env, jobject instance, jobject arg0, jint arg1);

            private:

            jclass clazz = nullptr;

            // method ids
            jmethodID directOperateMethod = nullptr;
            jmethodID directOperateAODouble64Method = nullptr;
            jmethodID directOperateAOFloat32Method = nullptr;
            jmethodID directOperateAOInt16Method = nullptr;
            jmethodID directOperateAOInt32Method = nullptr;
            jmethodID directOperateCROBMethod = nullptr;
            jmethodID selectAndOperateMethod = nullptr;
            jmethodID selectAndOperateAODouble64Method = nullptr;
            jmethodID selectAndOperateAOFloat32Method = nullptr;
            jmethodID selectAndOperateAOInt16Method = nullptr;
            jmethodID selectAndOperateAOInt32Method = nullptr;
            jmethodID selectAndOperateCROBMethod = nullptr;
        };
    }
}

#endif
