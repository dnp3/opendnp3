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

#include "JNIOutstationConfig.h"

namespace jni
{
    namespace cache
    {
        bool OutstationConfig::init(JNIEnv* env)
        {
            auto clazzTemp = env->FindClass("Lcom/automatak/dnp3/OutstationConfig;");
            this->clazz = (jclass) env->NewGlobalRef(clazzTemp);
            env->DeleteLocalRef(clazzTemp);

            this->indexModeField = env->GetFieldID(this->clazz, "indexMode", "Lcom/automatak/dnp3/enums/IndexMode;");
            if(!this->indexModeField) return false;

            this->maxControlsPerRequestField = env->GetFieldID(this->clazz, "maxControlsPerRequest", "S");
            if(!this->maxControlsPerRequestField) return false;

            this->selectTimeoutField = env->GetFieldID(this->clazz, "selectTimeout", "Ljava/time/Duration;");
            if(!this->selectTimeoutField) return false;

            this->solConfirmTimeoutField = env->GetFieldID(this->clazz, "solConfirmTimeout", "Ljava/time/Duration;");
            if(!this->solConfirmTimeoutField) return false;

            this->unsolRetryTimeoutField = env->GetFieldID(this->clazz, "unsolRetryTimeout", "Ljava/time/Duration;");
            if(!this->unsolRetryTimeoutField) return false;

            this->maxTxFragSizeField = env->GetFieldID(this->clazz, "maxTxFragSize", "I");
            if(!this->maxTxFragSizeField) return false;

            this->maxRxFragSizeField = env->GetFieldID(this->clazz, "maxRxFragSize", "I");
            if(!this->maxRxFragSizeField) return false;

            this->allowUnsolicitedField = env->GetFieldID(this->clazz, "allowUnsolicited", "Z");
            if(!this->allowUnsolicitedField) return false;

            return true;
        }

        void OutstationConfig::cleanup(JNIEnv* env)
        {
            env->DeleteGlobalRef(this->clazz);
        }

        jboolean OutstationConfig::getallowUnsolicited(JNIEnv* env, jobject instance)
        {
            return env->GetBooleanField(instance, this->allowUnsolicitedField);
        }

        LocalRef<jobject> OutstationConfig::getindexMode(JNIEnv* env, jobject instance)
        {
            return LocalRef<jobject>(env, env->GetObjectField(instance, this->indexModeField));
        }

        jshort OutstationConfig::getmaxControlsPerRequest(JNIEnv* env, jobject instance)
        {
            return env->GetShortField(instance, this->maxControlsPerRequestField);
        }

        jint OutstationConfig::getmaxRxFragSize(JNIEnv* env, jobject instance)
        {
            return env->GetIntField(instance, this->maxRxFragSizeField);
        }

        jint OutstationConfig::getmaxTxFragSize(JNIEnv* env, jobject instance)
        {
            return env->GetIntField(instance, this->maxTxFragSizeField);
        }

        LocalRef<jobject> OutstationConfig::getselectTimeout(JNIEnv* env, jobject instance)
        {
            return LocalRef<jobject>(env, env->GetObjectField(instance, this->selectTimeoutField));
        }

        LocalRef<jobject> OutstationConfig::getsolConfirmTimeout(JNIEnv* env, jobject instance)
        {
            return LocalRef<jobject>(env, env->GetObjectField(instance, this->solConfirmTimeoutField));
        }

        LocalRef<jobject> OutstationConfig::getunsolRetryTimeout(JNIEnv* env, jobject instance)
        {
            return LocalRef<jobject>(env, env->GetObjectField(instance, this->unsolRetryTimeoutField));
        }
    }
}
