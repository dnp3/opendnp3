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

#include "JNIOutstationStackConfig.h"

namespace jni
{
    namespace cache
    {
        bool OutstationStackConfig::init(JNIEnv* env)
        {
            auto clazzTemp = env->FindClass("Lcom/automatak/dnp3/OutstationStackConfig;");
            if(!clazzTemp) return false;
            this->clazz = (jclass) env->NewGlobalRef(clazzTemp);
            env->DeleteLocalRef(clazzTemp);

            this->linkConfigField = env->GetFieldID(this->clazz, "linkConfig", "Lcom/automatak/dnp3/LinkLayerConfig;");
            if(!this->linkConfigField) return false;

            this->outstationConfigField = env->GetFieldID(this->clazz, "outstationConfig", "Lcom/automatak/dnp3/OutstationConfig;");
            if(!this->outstationConfigField) return false;

            this->databaseConfigField = env->GetFieldID(this->clazz, "databaseConfig", "Lcom/automatak/dnp3/DatabaseConfig;");
            if(!this->databaseConfigField) return false;

            this->eventBufferConfigField = env->GetFieldID(this->clazz, "eventBufferConfig", "Lcom/automatak/dnp3/EventBufferConfig;");
            if(!this->eventBufferConfigField) return false;

            return true;
        }

        void OutstationStackConfig::cleanup(JNIEnv* env)
        {
            env->DeleteGlobalRef(this->clazz);
        }

        LocalRef<JDatabaseConfig> OutstationStackConfig::getdatabaseConfig(JNIEnv* env, JOutstationStackConfig instance)
        {
            return LocalRef<JDatabaseConfig>(env, env->GetObjectField(instance, this->databaseConfigField));
        }

        LocalRef<JEventBufferConfig> OutstationStackConfig::geteventBufferConfig(JNIEnv* env, JOutstationStackConfig instance)
        {
            return LocalRef<JEventBufferConfig>(env, env->GetObjectField(instance, this->eventBufferConfigField));
        }

        LocalRef<JLinkLayerConfig> OutstationStackConfig::getlinkConfig(JNIEnv* env, JOutstationStackConfig instance)
        {
            return LocalRef<JLinkLayerConfig>(env, env->GetObjectField(instance, this->linkConfigField));
        }

        LocalRef<JOutstationConfig> OutstationStackConfig::getoutstationConfig(JNIEnv* env, JOutstationStackConfig instance)
        {
            return LocalRef<JOutstationConfig>(env, env->GetObjectField(instance, this->outstationConfigField));
        }
    }
}
