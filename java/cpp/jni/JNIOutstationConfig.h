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

#ifndef OPENDNP3JAVA_JNIOUTSTATIONCONFIG_H
#define OPENDNP3JAVA_JNIOUTSTATIONCONFIG_H

#include "../adapters/LocalRef.h"

#include "JNIWrappers.h"

namespace jni
{
    struct JCache;

    namespace cache
    {
        class OutstationConfig
        {
            friend struct jni::JCache;

            bool init(JNIEnv* env);
            void cleanup(JNIEnv* env);

            public:

            // field getter methods
            jboolean getallowUnsolicited(JNIEnv* env, JOutstationConfig instance);
            jlong getmaxControlsPerRequest(JNIEnv* env, JOutstationConfig instance);
            jint getmaxRxFragSize(JNIEnv* env, JOutstationConfig instance);
            jint getmaxTxFragSize(JNIEnv* env, JOutstationConfig instance);
            jboolean getnoDefferedReadDuringUnsolicitedNullResponse(JNIEnv* env, JOutstationConfig instance);
            LocalRef<JNumRetries> getnumUnsolRetries(JNIEnv* env, JOutstationConfig instance);
            LocalRef<JDuration> getselectTimeout(JNIEnv* env, JOutstationConfig instance);
            LocalRef<JDuration> getsolConfirmTimeout(JNIEnv* env, JOutstationConfig instance);
            LocalRef<JStaticTypeBitField> gettypesAllowedInClass0(JNIEnv* env, JOutstationConfig instance);
            LocalRef<JDuration> getunsolConfirmTimeout(JNIEnv* env, JOutstationConfig instance);

            private:

            jclass clazz = nullptr;

            // field ids
            jfieldID maxControlsPerRequestField = nullptr;
            jfieldID selectTimeoutField = nullptr;
            jfieldID solConfirmTimeoutField = nullptr;
            jfieldID unsolConfirmTimeoutField = nullptr;
            jfieldID numUnsolRetriesField = nullptr;
            jfieldID maxTxFragSizeField = nullptr;
            jfieldID maxRxFragSizeField = nullptr;
            jfieldID allowUnsolicitedField = nullptr;
            jfieldID typesAllowedInClass0Field = nullptr;
            jfieldID noDefferedReadDuringUnsolicitedNullResponseField = nullptr;
        };
    }
}

#endif
