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

#include "JNIControlRelayOutputBlock.h"

namespace jni
{
    namespace cache
    {
        bool ControlRelayOutputBlock::init(JNIEnv* env)
        {
            auto clazzTemp = env->FindClass("Lcom/automatak/dnp3/ControlRelayOutputBlock;");
            this->clazz = (jclass) env->NewGlobalRef(clazzTemp);
            env->DeleteLocalRef(clazzTemp);

            this->init5Constructor = env->GetMethodID(this->clazz, "<init>", "(Lcom/automatak/dnp3/enums/ControlCode;SJJLcom/automatak/dnp3/enums/CommandStatus;)V");
            if(!this->init5Constructor) return false;

            this->functionField = env->GetFieldID(this->clazz, "function", "Lcom/automatak/dnp3/enums/ControlCode;");
            if(!this->functionField) return false;

            this->countField = env->GetFieldID(this->clazz, "count", "S");
            if(!this->countField) return false;

            this->onTimeMsField = env->GetFieldID(this->clazz, "onTimeMs", "J");
            if(!this->onTimeMsField) return false;

            this->offTimeMsField = env->GetFieldID(this->clazz, "offTimeMs", "J");
            if(!this->offTimeMsField) return false;

            this->statusField = env->GetFieldID(this->clazz, "status", "Lcom/automatak/dnp3/enums/CommandStatus;");
            if(!this->statusField) return false;

            return true;
        }

        void ControlRelayOutputBlock::cleanup(JNIEnv* env)
        {
            env->DeleteGlobalRef(this->clazz);
        }

        LocalRef<jobject> ControlRelayOutputBlock::init5(JNIEnv* env, jobject arg0, jshort arg1, jlong arg2, jlong arg3, jobject arg4)
        {
            return LocalRef<jobject>(env, env->NewObject(this->clazz, this->init5Constructor, arg0, arg1, arg2, arg3, arg4));
        }

        jshort ControlRelayOutputBlock::getcount(JNIEnv* env, jobject instance)
        {
            return env->GetShortField(instance, this->countField);
        }

        LocalRef<jobject> ControlRelayOutputBlock::getfunction(JNIEnv* env, jobject instance)
        {
            return LocalRef<jobject>(env, env->GetObjectField(instance, this->functionField));
        }

        jlong ControlRelayOutputBlock::getoffTimeMs(JNIEnv* env, jobject instance)
        {
            return env->GetLongField(instance, this->offTimeMsField);
        }

        jlong ControlRelayOutputBlock::getonTimeMs(JNIEnv* env, jobject instance)
        {
            return env->GetLongField(instance, this->onTimeMsField);
        }

        LocalRef<jobject> ControlRelayOutputBlock::getstatus(JNIEnv* env, jobject instance)
        {
            return LocalRef<jobject>(env, env->GetObjectField(instance, this->statusField));
        }
    }
}
