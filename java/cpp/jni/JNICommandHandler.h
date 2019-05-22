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

#ifndef OPENDNP3JAVA_JNICOMMANDHANDLER_H
#define OPENDNP3JAVA_JNICOMMANDHANDLER_H

#include <jni.h>

#include "../adapters/LocalRef.h"

namespace jni
{
    struct JCache;

    namespace cache
    {
        class CommandHandler
        {
            friend struct jni::JCache;

            bool init(JNIEnv* env);
            void cleanup(JNIEnv* env);

            public:

            // methods
            void end(JNIEnv* env, jobject instance);
            LocalRef<jobject> operateAOD64(JNIEnv* env, jobject instance, jobject arg0, jint arg1, jobject arg2);
            LocalRef<jobject> operateAOF32(JNIEnv* env, jobject instance, jobject arg0, jint arg1, jobject arg2);
            LocalRef<jobject> operateAOI16(JNIEnv* env, jobject instance, jobject arg0, jint arg1, jobject arg2);
            LocalRef<jobject> operateAOI32(JNIEnv* env, jobject instance, jobject arg0, jint arg1, jobject arg2);
            LocalRef<jobject> operateCROB(JNIEnv* env, jobject instance, jobject arg0, jint arg1, jobject arg2);
            LocalRef<jobject> selectAOD64(JNIEnv* env, jobject instance, jobject arg0, jint arg1);
            LocalRef<jobject> selectAOF32(JNIEnv* env, jobject instance, jobject arg0, jint arg1);
            LocalRef<jobject> selectAOI16(JNIEnv* env, jobject instance, jobject arg0, jint arg1);
            LocalRef<jobject> selectAOI32(JNIEnv* env, jobject instance, jobject arg0, jint arg1);
            LocalRef<jobject> selectCROB(JNIEnv* env, jobject instance, jobject arg0, jint arg1);
            void start(JNIEnv* env, jobject instance);

            private:

            jclass clazz = nullptr;

            // method ids
            jmethodID endMethod = nullptr;
            jmethodID operateAOD64Method = nullptr;
            jmethodID operateAOF32Method = nullptr;
            jmethodID operateAOI16Method = nullptr;
            jmethodID operateAOI32Method = nullptr;
            jmethodID operateCROBMethod = nullptr;
            jmethodID selectAOD64Method = nullptr;
            jmethodID selectAOF32Method = nullptr;
            jmethodID selectAOI16Method = nullptr;
            jmethodID selectAOI32Method = nullptr;
            jmethodID selectCROBMethod = nullptr;
            jmethodID startMethod = nullptr;
        };
    }
}

#endif
