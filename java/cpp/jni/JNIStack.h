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
// Copyright 2013-2022 Step Function I/O, LLC
// 
// Licensed to Green Energy Corp (www.greenenergycorp.com) and Step Function I/O
// LLC (https://stepfunc.io) under one or more contributor license agreements.
// See the NOTICE file distributed with this work for additional information
// regarding copyright ownership. Green Energy Corp and Step Function I/O LLC license
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

#ifndef OPENDNP3JAVA_JNISTACK_H
#define OPENDNP3JAVA_JNISTACK_H

#include "../adapters/LocalRef.h"

#include "JNIWrappers.h"

namespace jni
{
    struct JCache;

    namespace cache
    {
        class Stack
        {
            friend struct jni::JCache;

            bool init(JNIEnv* env);
            void cleanup(JNIEnv* env);

            public:

            // methods
            void disable(JNIEnv* env, JStack instance);
            void enable(JNIEnv* env, JStack instance);
            LocalRef<JStackStatistics> getStatistics(JNIEnv* env, JStack instance);
            void setLogLevel(JNIEnv* env, JStack instance, jint arg0);
            void shutdown(JNIEnv* env, JStack instance);

            private:

            jclass clazz = nullptr;

            // method ids
            jmethodID method0 = nullptr;
            jmethodID method1 = nullptr;
            jmethodID method2 = nullptr;
            jmethodID method3 = nullptr;
            jmethodID method4 = nullptr;
        };
    }
}

#endif
