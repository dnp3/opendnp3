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

#include "JNIMasterConfig.h"

namespace jni
{
    namespace cache
    {
        bool MasterConfig::init(JNIEnv* env)
        {
            auto clazzTemp = env->FindClass("Lcom/automatak/dnp3/MasterConfig;");
            if(!clazzTemp) return false;
            this->clazz = (jclass) env->NewGlobalRef(clazzTemp);
            env->DeleteLocalRef(clazzTemp);

            this->responseTimeoutField = env->GetFieldID(this->clazz, "responseTimeout", "Ljava/time/Duration;");
            if(!this->responseTimeoutField) return false;

            this->timeSyncModeField = env->GetFieldID(this->clazz, "timeSyncMode", "Lcom/automatak/dnp3/enums/TimeSyncMode;");
            if(!this->timeSyncModeField) return false;

            this->disableUnsolOnStartupField = env->GetFieldID(this->clazz, "disableUnsolOnStartup", "Z");
            if(!this->disableUnsolOnStartupField) return false;

            this->ignoreRestartIINField = env->GetFieldID(this->clazz, "ignoreRestartIIN", "Z");
            if(!this->ignoreRestartIINField) return false;

            this->unsolClassMaskField = env->GetFieldID(this->clazz, "unsolClassMask", "Lcom/automatak/dnp3/ClassField;");
            if(!this->unsolClassMaskField) return false;

            this->startupIntegrityClassMaskField = env->GetFieldID(this->clazz, "startupIntegrityClassMask", "Lcom/automatak/dnp3/ClassField;");
            if(!this->startupIntegrityClassMaskField) return false;

            this->integrityOnEventOverflowIINField = env->GetFieldID(this->clazz, "integrityOnEventOverflowIIN", "Z");
            if(!this->integrityOnEventOverflowIINField) return false;

            this->eventScanOnEventsAvailableClassMaskField = env->GetFieldID(this->clazz, "eventScanOnEventsAvailableClassMask", "Lcom/automatak/dnp3/ClassField;");
            if(!this->eventScanOnEventsAvailableClassMaskField) return false;

            this->taskRetryPeriodField = env->GetFieldID(this->clazz, "taskRetryPeriod", "Ljava/time/Duration;");
            if(!this->taskRetryPeriodField) return false;

            this->taskStartTimeoutField = env->GetFieldID(this->clazz, "taskStartTimeout", "Ljava/time/Duration;");
            if(!this->taskStartTimeoutField) return false;

            this->maxTxFragSizeField = env->GetFieldID(this->clazz, "maxTxFragSize", "I");
            if(!this->maxTxFragSizeField) return false;

            this->maxRxFragSizeField = env->GetFieldID(this->clazz, "maxRxFragSize", "I");
            if(!this->maxRxFragSizeField) return false;

            this->controlQualifierModeField = env->GetFieldID(this->clazz, "controlQualifierMode", "Lcom/automatak/dnp3/enums/IndexQualifierMode;");
            if(!this->controlQualifierModeField) return false;

            return true;
        }

        void MasterConfig::cleanup(JNIEnv* env)
        {
            env->DeleteGlobalRef(this->clazz);
        }

        LocalRef<JIndexQualifierMode> MasterConfig::getcontrolQualifierMode(JNIEnv* env, JMasterConfig instance)
        {
            return LocalRef<JIndexQualifierMode>(env, env->GetObjectField(instance, this->controlQualifierModeField));
        }

        jboolean MasterConfig::getdisableUnsolOnStartup(JNIEnv* env, JMasterConfig instance)
        {
            return env->GetBooleanField(instance, this->disableUnsolOnStartupField);
        }

        LocalRef<JClassField> MasterConfig::geteventScanOnEventsAvailableClassMask(JNIEnv* env, JMasterConfig instance)
        {
            return LocalRef<JClassField>(env, env->GetObjectField(instance, this->eventScanOnEventsAvailableClassMaskField));
        }

        jboolean MasterConfig::getignoreRestartIIN(JNIEnv* env, JMasterConfig instance)
        {
            return env->GetBooleanField(instance, this->ignoreRestartIINField);
        }

        jboolean MasterConfig::getintegrityOnEventOverflowIIN(JNIEnv* env, JMasterConfig instance)
        {
            return env->GetBooleanField(instance, this->integrityOnEventOverflowIINField);
        }

        jint MasterConfig::getmaxRxFragSize(JNIEnv* env, JMasterConfig instance)
        {
            return env->GetIntField(instance, this->maxRxFragSizeField);
        }

        jint MasterConfig::getmaxTxFragSize(JNIEnv* env, JMasterConfig instance)
        {
            return env->GetIntField(instance, this->maxTxFragSizeField);
        }

        LocalRef<JDuration> MasterConfig::getresponseTimeout(JNIEnv* env, JMasterConfig instance)
        {
            return LocalRef<JDuration>(env, env->GetObjectField(instance, this->responseTimeoutField));
        }

        LocalRef<JClassField> MasterConfig::getstartupIntegrityClassMask(JNIEnv* env, JMasterConfig instance)
        {
            return LocalRef<JClassField>(env, env->GetObjectField(instance, this->startupIntegrityClassMaskField));
        }

        LocalRef<JDuration> MasterConfig::gettaskRetryPeriod(JNIEnv* env, JMasterConfig instance)
        {
            return LocalRef<JDuration>(env, env->GetObjectField(instance, this->taskRetryPeriodField));
        }

        LocalRef<JDuration> MasterConfig::gettaskStartTimeout(JNIEnv* env, JMasterConfig instance)
        {
            return LocalRef<JDuration>(env, env->GetObjectField(instance, this->taskStartTimeoutField));
        }

        LocalRef<JTimeSyncMode> MasterConfig::gettimeSyncMode(JNIEnv* env, JMasterConfig instance)
        {
            return LocalRef<JTimeSyncMode>(env, env->GetObjectField(instance, this->timeSyncModeField));
        }

        LocalRef<JClassField> MasterConfig::getunsolClassMask(JNIEnv* env, JMasterConfig instance)
        {
            return LocalRef<JClassField>(env, env->GetObjectField(instance, this->unsolClassMaskField));
        }
    }
}
