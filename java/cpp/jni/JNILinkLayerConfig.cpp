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

#include "JNILinkLayerConfig.h"

namespace jni
{
    namespace cache
    {
        bool LinkLayerConfig::init(JNIEnv* env)
        {
            auto clazzTemp = env->FindClass("Lcom/automatak/dnp3/LinkLayerConfig;");
            if(!clazzTemp) return false;
            this->clazz = (jclass) env->NewGlobalRef(clazzTemp);
            env->DeleteLocalRef(clazzTemp);

            this->isMasterField = env->GetFieldID(this->clazz, "isMaster", "Z");
            if(!this->isMasterField) return false;

            this->localAddrField = env->GetFieldID(this->clazz, "localAddr", "I");
            if(!this->localAddrField) return false;

            this->remoteAddrField = env->GetFieldID(this->clazz, "remoteAddr", "I");
            if(!this->remoteAddrField) return false;

            this->responseTimeoutField = env->GetFieldID(this->clazz, "responseTimeout", "Ljava/time/Duration;");
            if(!this->responseTimeoutField) return false;

            this->keepAliveTimeoutField = env->GetFieldID(this->clazz, "keepAliveTimeout", "Ljava/time/Duration;");
            if(!this->keepAliveTimeoutField) return false;

            return true;
        }

        void LinkLayerConfig::cleanup(JNIEnv* env)
        {
            env->DeleteGlobalRef(this->clazz);
        }

        jboolean LinkLayerConfig::getisMaster(JNIEnv* env, JLinkLayerConfig instance)
        {
            return env->GetBooleanField(instance, this->isMasterField);
        }

        LocalRef<JDuration> LinkLayerConfig::getkeepAliveTimeout(JNIEnv* env, JLinkLayerConfig instance)
        {
            return LocalRef<JDuration>(env, env->GetObjectField(instance, this->keepAliveTimeoutField));
        }

        jint LinkLayerConfig::getlocalAddr(JNIEnv* env, JLinkLayerConfig instance)
        {
            return env->GetIntField(instance, this->localAddrField);
        }

        jint LinkLayerConfig::getremoteAddr(JNIEnv* env, JLinkLayerConfig instance)
        {
            return env->GetIntField(instance, this->remoteAddrField);
        }

        LocalRef<JDuration> LinkLayerConfig::getresponseTimeout(JNIEnv* env, JLinkLayerConfig instance)
        {
            return LocalRef<JDuration>(env, env->GetObjectField(instance, this->responseTimeoutField));
        }
    }
}
