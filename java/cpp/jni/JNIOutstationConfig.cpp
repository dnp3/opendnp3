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

#include "JNIOutstationConfig.h"

namespace jni
{
    namespace cache
    {
        bool OutstationConfig::init(JNIEnv* env)
        {
            auto clazzTemp = env->FindClass("Lcom/automatak/dnp3/OutstationConfig;");
            if(!clazzTemp) return false;
            this->clazz = (jclass) env->NewGlobalRef(clazzTemp);
            env->DeleteLocalRef(clazzTemp);

            this->maxControlsPerRequestField = env->GetFieldID(this->clazz, "maxControlsPerRequest", "J");
            if(!this->maxControlsPerRequestField) return false;

            this->selectTimeoutField = env->GetFieldID(this->clazz, "selectTimeout", "Ljava/time/Duration;");
            if(!this->selectTimeoutField) return false;

            this->solConfirmTimeoutField = env->GetFieldID(this->clazz, "solConfirmTimeout", "Ljava/time/Duration;");
            if(!this->solConfirmTimeoutField) return false;

            this->unsolConfirmTimeoutField = env->GetFieldID(this->clazz, "unsolConfirmTimeout", "Ljava/time/Duration;");
            if(!this->unsolConfirmTimeoutField) return false;

            this->numUnsolRetriesField = env->GetFieldID(this->clazz, "numUnsolRetries", "Lcom/automatak/dnp3/NumRetries;");
            if(!this->numUnsolRetriesField) return false;

            this->maxTxFragSizeField = env->GetFieldID(this->clazz, "maxTxFragSize", "I");
            if(!this->maxTxFragSizeField) return false;

            this->maxRxFragSizeField = env->GetFieldID(this->clazz, "maxRxFragSize", "I");
            if(!this->maxRxFragSizeField) return false;

            this->allowUnsolicitedField = env->GetFieldID(this->clazz, "allowUnsolicited", "Z");
            if(!this->allowUnsolicitedField) return false;

            this->typesAllowedInClass0Field = env->GetFieldID(this->clazz, "typesAllowedInClass0", "Lcom/automatak/dnp3/StaticTypeBitField;");
            if(!this->typesAllowedInClass0Field) return false;

            this->noDefferedReadDuringUnsolicitedNullResponseField = env->GetFieldID(this->clazz, "noDefferedReadDuringUnsolicitedNullResponse", "Z");
            if(!this->noDefferedReadDuringUnsolicitedNullResponseField) return false;

            return true;
        }

        void OutstationConfig::cleanup(JNIEnv* env)
        {
            env->DeleteGlobalRef(this->clazz);
        }

        jboolean OutstationConfig::getallowUnsolicited(JNIEnv* env, JOutstationConfig instance)
        {
            return env->GetBooleanField(instance, this->allowUnsolicitedField);
        }

        jlong OutstationConfig::getmaxControlsPerRequest(JNIEnv* env, JOutstationConfig instance)
        {
            return env->GetLongField(instance, this->maxControlsPerRequestField);
        }

        jint OutstationConfig::getmaxRxFragSize(JNIEnv* env, JOutstationConfig instance)
        {
            return env->GetIntField(instance, this->maxRxFragSizeField);
        }

        jint OutstationConfig::getmaxTxFragSize(JNIEnv* env, JOutstationConfig instance)
        {
            return env->GetIntField(instance, this->maxTxFragSizeField);
        }

        jboolean OutstationConfig::getnoDefferedReadDuringUnsolicitedNullResponse(JNIEnv* env, JOutstationConfig instance)
        {
            return env->GetBooleanField(instance, this->noDefferedReadDuringUnsolicitedNullResponseField);
        }

        LocalRef<JNumRetries> OutstationConfig::getnumUnsolRetries(JNIEnv* env, JOutstationConfig instance)
        {
            return LocalRef<JNumRetries>(env, env->GetObjectField(instance, this->numUnsolRetriesField));
        }

        LocalRef<JDuration> OutstationConfig::getselectTimeout(JNIEnv* env, JOutstationConfig instance)
        {
            return LocalRef<JDuration>(env, env->GetObjectField(instance, this->selectTimeoutField));
        }

        LocalRef<JDuration> OutstationConfig::getsolConfirmTimeout(JNIEnv* env, JOutstationConfig instance)
        {
            return LocalRef<JDuration>(env, env->GetObjectField(instance, this->solConfirmTimeoutField));
        }

        LocalRef<JStaticTypeBitField> OutstationConfig::gettypesAllowedInClass0(JNIEnv* env, JOutstationConfig instance)
        {
            return LocalRef<JStaticTypeBitField>(env, env->GetObjectField(instance, this->typesAllowedInClass0Field));
        }

        LocalRef<JDuration> OutstationConfig::getunsolConfirmTimeout(JNIEnv* env, JOutstationConfig instance)
        {
            return LocalRef<JDuration>(env, env->GetObjectField(instance, this->unsolConfirmTimeoutField));
        }
    }
}
