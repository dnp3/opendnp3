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
// Copyright 2013-2020 Automatak, LLC
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

#ifndef OPENDNP3JAVA_JNIOUTSTATIONAPPLICATION_H
#define OPENDNP3JAVA_JNIOUTSTATIONAPPLICATION_H

#include "../adapters/LocalRef.h"

#include "JNIWrappers.h"

namespace jni
{
    struct JCache;

    namespace cache
    {
        class OutstationApplication
        {
            friend struct jni::JCache;

            bool init(JNIEnv* env);
            void cleanup(JNIEnv* env);

            public:

            // methods
            jint coldRestart(JNIEnv* env, JOutstationApplication instance);
            LocalRef<JRestartMode> coldRestartSupport(JNIEnv* env, JOutstationApplication instance);
            LocalRef<JApplicationIIN> getApplicationIIN(JNIEnv* env, JOutstationApplication instance);
            LocalRef<JDNPTime> now(JNIEnv* env, JOutstationApplication instance);
            void onConfirmProcessed(JNIEnv* env, JOutstationApplication instance, jboolean arg0, jlong arg1, jlong arg2, jlong arg3);
            void recordClassAssignment(JNIEnv* env, JOutstationApplication instance, JAssignClassType arg0, JPointClass arg1, jint arg2, jint arg3);
            jboolean supportsAssignClass(JNIEnv* env, JOutstationApplication instance);
            jboolean supportsWriteAbsoluteTime(JNIEnv* env, JOutstationApplication instance);
            jint warmRestart(JNIEnv* env, JOutstationApplication instance);
            LocalRef<JRestartMode> warmRestartSupport(JNIEnv* env, JOutstationApplication instance);
            jboolean writeAbsoluteTime(JNIEnv* env, JOutstationApplication instance, jlong arg0);

            private:

            jclass clazz = nullptr;

            // method ids
            jmethodID method0 = nullptr;
            jmethodID method1 = nullptr;
            jmethodID method2 = nullptr;
            jmethodID method3 = nullptr;
            jmethodID method4 = nullptr;
            jmethodID method5 = nullptr;
            jmethodID method6 = nullptr;
            jmethodID method7 = nullptr;
            jmethodID method8 = nullptr;
            jmethodID method9 = nullptr;
            jmethodID method10 = nullptr;
        };
    }
}

#endif
