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

#include "JNIDatabaseConfig.h"

namespace jni
{
    namespace cache
    {
        bool DatabaseConfig::init(JNIEnv* env)
        {
            auto clazzTemp = env->FindClass("Lcom/automatak/dnp3/DatabaseConfig;");
            if(!clazzTemp) return false;
            this->clazz = (jclass) env->NewGlobalRef(clazzTemp);
            env->DeleteLocalRef(clazzTemp);

            this->binaryField = env->GetFieldID(this->clazz, "binary", "Ljava/util/Map;");
            if(!this->binaryField) return false;

            this->doubleBinaryField = env->GetFieldID(this->clazz, "doubleBinary", "Ljava/util/Map;");
            if(!this->doubleBinaryField) return false;

            this->analogField = env->GetFieldID(this->clazz, "analog", "Ljava/util/Map;");
            if(!this->analogField) return false;

            this->counterField = env->GetFieldID(this->clazz, "counter", "Ljava/util/Map;");
            if(!this->counterField) return false;

            this->frozenCounterField = env->GetFieldID(this->clazz, "frozenCounter", "Ljava/util/Map;");
            if(!this->frozenCounterField) return false;

            this->boStatusField = env->GetFieldID(this->clazz, "boStatus", "Ljava/util/Map;");
            if(!this->boStatusField) return false;

            this->aoStatusField = env->GetFieldID(this->clazz, "aoStatus", "Ljava/util/Map;");
            if(!this->aoStatusField) return false;

            return true;
        }

        void DatabaseConfig::cleanup(JNIEnv* env)
        {
            env->DeleteGlobalRef(this->clazz);
        }

        LocalRef<JMap> DatabaseConfig::getanalog(JNIEnv* env, JDatabaseConfig instance)
        {
            return LocalRef<JMap>(env, env->GetObjectField(instance, this->analogField));
        }

        LocalRef<JMap> DatabaseConfig::getaoStatus(JNIEnv* env, JDatabaseConfig instance)
        {
            return LocalRef<JMap>(env, env->GetObjectField(instance, this->aoStatusField));
        }

        LocalRef<JMap> DatabaseConfig::getbinary(JNIEnv* env, JDatabaseConfig instance)
        {
            return LocalRef<JMap>(env, env->GetObjectField(instance, this->binaryField));
        }

        LocalRef<JMap> DatabaseConfig::getboStatus(JNIEnv* env, JDatabaseConfig instance)
        {
            return LocalRef<JMap>(env, env->GetObjectField(instance, this->boStatusField));
        }

        LocalRef<JMap> DatabaseConfig::getcounter(JNIEnv* env, JDatabaseConfig instance)
        {
            return LocalRef<JMap>(env, env->GetObjectField(instance, this->counterField));
        }

        LocalRef<JMap> DatabaseConfig::getdoubleBinary(JNIEnv* env, JDatabaseConfig instance)
        {
            return LocalRef<JMap>(env, env->GetObjectField(instance, this->doubleBinaryField));
        }

        LocalRef<JMap> DatabaseConfig::getfrozenCounter(JNIEnv* env, JDatabaseConfig instance)
        {
            return LocalRef<JMap>(env, env->GetObjectField(instance, this->frozenCounterField));
        }
    }
}
