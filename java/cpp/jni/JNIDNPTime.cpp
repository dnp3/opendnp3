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

#include "JNIDNPTime.h"

namespace jni
{
    namespace cache
    {
        bool DNPTime::init(JNIEnv* env)
        {
            auto clazzTemp = env->FindClass("Lcom/automatak/dnp3/DNPTime;");
            if(!clazzTemp) return false;
            this->clazz = (jclass) env->NewGlobalRef(clazzTemp);
            env->DeleteLocalRef(clazzTemp);

            this->constructor0 = env->GetMethodID(this->clazz, "<init>", "(J)V");
            if(!this->constructor0) return false;

            this->constructor1 = env->GetMethodID(this->clazz, "<init>", "(JLcom/automatak/dnp3/enums/TimestampQuality;)V");
            if(!this->constructor1) return false;

            this->msSinceEpochField = env->GetFieldID(this->clazz, "msSinceEpoch", "J");
            if(!this->msSinceEpochField) return false;

            this->qualityField = env->GetFieldID(this->clazz, "quality", "Lcom/automatak/dnp3/enums/TimestampQuality;");
            if(!this->qualityField) return false;

            return true;
        }

        void DNPTime::cleanup(JNIEnv* env)
        {
            env->DeleteGlobalRef(this->clazz);
        }

        LocalRef<JDNPTime> DNPTime::construct(JNIEnv* env, jlong arg0)
        {
            return LocalRef<JDNPTime>(env, JDNPTime(env->NewObject(this->clazz, this->constructor0, arg0)));
        }

        LocalRef<JDNPTime> DNPTime::construct(JNIEnv* env, jlong arg0, JTimestampQuality arg1)
        {
            return LocalRef<JDNPTime>(env, JDNPTime(env->NewObject(this->clazz, this->constructor1, arg0, arg1)));
        }

        jlong DNPTime::getmsSinceEpoch(JNIEnv* env, JDNPTime instance)
        {
            return env->GetLongField(instance, this->msSinceEpochField);
        }

        LocalRef<JTimestampQuality> DNPTime::getquality(JNIEnv* env, JDNPTime instance)
        {
            return LocalRef<JTimestampQuality>(env, env->GetObjectField(instance, this->qualityField));
        }
    }
}
