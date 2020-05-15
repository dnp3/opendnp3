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

#include "JNIApplicationIIN.h"

namespace jni
{
    namespace cache
    {
        bool ApplicationIIN::init(JNIEnv* env)
        {
            auto clazzTemp = env->FindClass("Lcom/automatak/dnp3/ApplicationIIN;");
            if(!clazzTemp) return false;
            this->clazz = (jclass) env->NewGlobalRef(clazzTemp);
            env->DeleteLocalRef(clazzTemp);

            this->needTimeField = env->GetFieldID(this->clazz, "needTime", "Z");
            if(!this->needTimeField) return false;

            this->localControlField = env->GetFieldID(this->clazz, "localControl", "Z");
            if(!this->localControlField) return false;

            this->deviceTroubleField = env->GetFieldID(this->clazz, "deviceTrouble", "Z");
            if(!this->deviceTroubleField) return false;

            this->configCorruptField = env->GetFieldID(this->clazz, "configCorrupt", "Z");
            if(!this->configCorruptField) return false;

            return true;
        }

        void ApplicationIIN::cleanup(JNIEnv* env)
        {
            env->DeleteGlobalRef(this->clazz);
        }

        jboolean ApplicationIIN::getconfigCorrupt(JNIEnv* env, JApplicationIIN instance)
        {
            return env->GetBooleanField(instance, this->configCorruptField);
        }

        jboolean ApplicationIIN::getdeviceTrouble(JNIEnv* env, JApplicationIIN instance)
        {
            return env->GetBooleanField(instance, this->deviceTroubleField);
        }

        jboolean ApplicationIIN::getlocalControl(JNIEnv* env, JApplicationIIN instance)
        {
            return env->GetBooleanField(instance, this->localControlField);
        }

        jboolean ApplicationIIN::getneedTime(JNIEnv* env, JApplicationIIN instance)
        {
            return env->GetBooleanField(instance, this->needTimeField);
        }
    }
}
