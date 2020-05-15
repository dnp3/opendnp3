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

#ifndef OPENDNP3JAVA_JNICONTROLRELAYOUTPUTBLOCK_H
#define OPENDNP3JAVA_JNICONTROLRELAYOUTPUTBLOCK_H

#include "../adapters/LocalRef.h"

#include "JNIWrappers.h"

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
            LocalRef<JControlRelayOutputBlock> construct(JNIEnv* env, JOperationType arg0, JTripCloseCode arg1, jboolean arg2, jshort arg3, jlong arg4, jlong arg5, JCommandStatus arg6);

            // field getter methods
            jboolean getclear(JNIEnv* env, JControlRelayOutputBlock instance);
            jshort getcount(JNIEnv* env, JControlRelayOutputBlock instance);
            jlong getoffTimeMs(JNIEnv* env, JControlRelayOutputBlock instance);
            jlong getonTimeMs(JNIEnv* env, JControlRelayOutputBlock instance);
            LocalRef<JOperationType> getopType(JNIEnv* env, JControlRelayOutputBlock instance);
            LocalRef<JCommandStatus> getstatus(JNIEnv* env, JControlRelayOutputBlock instance);
            LocalRef<JTripCloseCode> gettcc(JNIEnv* env, JControlRelayOutputBlock instance);

            private:

            jclass clazz = nullptr;

            // constructor method ids
            jmethodID constructor0 = nullptr;

            // field ids
            jfieldID opTypeField = nullptr;
            jfieldID tccField = nullptr;
            jfieldID clearField = nullptr;
            jfieldID countField = nullptr;
            jfieldID onTimeMsField = nullptr;
            jfieldID offTimeMsField = nullptr;
            jfieldID statusField = nullptr;
        };
    }
}

#endif
