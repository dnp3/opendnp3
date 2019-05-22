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

#ifndef OPENDNP3JAVA_JNICONTROLRELAYOUTPUTBLOCK_H
#define OPENDNP3JAVA_JNICONTROLRELAYOUTPUTBLOCK_H

#include <jni.h>

#include "../adapters/LocalRef.h"

namespace jni
{
    struct JCache;

    namespace cache
    {
        class ControlRelayOutputBlock
        {
            friend struct jni::JCache;

            bool init(JNIEnv* env);
            void cleanup(JNIEnv* env);

            public:

            // constructor methods
            LocalRef<jobject> init5(JNIEnv* env, jobject arg0, jshort arg1, jlong arg2, jlong arg3, jobject arg4);

            // field getter methods
            jshort getcount(JNIEnv* env, jobject instance);
            LocalRef<jobject> getfunction(JNIEnv* env, jobject instance);
            jlong getoffTimeMs(JNIEnv* env, jobject instance);
            jlong getonTimeMs(JNIEnv* env, jobject instance);
            LocalRef<jobject> getstatus(JNIEnv* env, jobject instance);

            private:

            jclass clazz = nullptr;

            // constructor method ids
            jmethodID init5Constructor = nullptr;

            // field ids
            jfieldID functionField = nullptr;
            jfieldID countField = nullptr;
            jfieldID onTimeMsField = nullptr;
            jfieldID offTimeMsField = nullptr;
            jfieldID statusField = nullptr;
        };
    }
}

#endif
