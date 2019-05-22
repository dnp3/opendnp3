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

#include "JNIStack.h"

namespace jni
{
    namespace cache
    {
        bool Stack::init(JNIEnv* env)
        {
            auto clazzTemp = env->FindClass("Lcom/automatak/dnp3/Stack;");
            this->clazz = (jclass) env->NewGlobalRef(clazzTemp);
            env->DeleteLocalRef(clazzTemp);

            this->disableMethod = env->GetMethodID(this->clazz, "disable", "()V");
            if(!this->disableMethod) return false;

            this->enableMethod = env->GetMethodID(this->clazz, "enable", "()V");
            if(!this->enableMethod) return false;

            this->getStatisticsMethod = env->GetMethodID(this->clazz, "getStatistics", "()Lcom/automatak/dnp3/StackStatistics;");
            if(!this->getStatisticsMethod) return false;

            this->setLogLevelMethod = env->GetMethodID(this->clazz, "setLogLevel", "(I)V");
            if(!this->setLogLevelMethod) return false;

            this->shutdownMethod = env->GetMethodID(this->clazz, "shutdown", "()V");
            if(!this->shutdownMethod) return false;

            return true;
        }

        void Stack::cleanup(JNIEnv* env)
        {
            env->DeleteGlobalRef(this->clazz);
        }

        void Stack::disable(JNIEnv* env, jobject instance)
        {
            env->CallVoidMethod(instance, this->disableMethod);
        }

        void Stack::enable(JNIEnv* env, jobject instance)
        {
            env->CallVoidMethod(instance, this->enableMethod);
        }

        LocalRef<jobject> Stack::getStatistics(JNIEnv* env, jobject instance)
        {
            return LocalRef<jobject>(env, env->CallObjectMethod(instance, this->getStatisticsMethod));
        }

        void Stack::setLogLevel(JNIEnv* env, jobject instance, jint arg0)
        {
            env->CallVoidMethod(instance, this->setLogLevelMethod, arg0);
        }

        void Stack::shutdown(JNIEnv* env, jobject instance)
        {
            env->CallVoidMethod(instance, this->shutdownMethod);
        }
    }
}
