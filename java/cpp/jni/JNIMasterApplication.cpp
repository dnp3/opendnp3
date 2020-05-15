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

#include "JNIMasterApplication.h"

namespace jni
{
    namespace cache
    {
        bool MasterApplication::init(JNIEnv* env)
        {
            auto clazzTemp = env->FindClass("Lcom/automatak/dnp3/MasterApplication;");
            if(!clazzTemp) return false;
            this->clazz = (jclass) env->NewGlobalRef(clazzTemp);
            env->DeleteLocalRef(clazzTemp);

            this->method0 = env->GetMethodID(this->clazz, "assignClassDuringStartup", "()Z");
            if(!this->method0) return false;

            this->method1 = env->GetMethodID(this->clazz, "getClassAssignments", "()Ljava/lang/Iterable;");
            if(!this->method1) return false;

            this->method2 = env->GetMethodID(this->clazz, "getMillisecondsSinceEpoch", "()J");
            if(!this->method2) return false;

            this->method3 = env->GetMethodID(this->clazz, "onClose", "()V");
            if(!this->method3) return false;

            this->method4 = env->GetMethodID(this->clazz, "onOpen", "()V");
            if(!this->method4) return false;

            this->method5 = env->GetMethodID(this->clazz, "onReceiveIIN", "(Lcom/automatak/dnp3/IINField;)V");
            if(!this->method5) return false;

            this->method6 = env->GetMethodID(this->clazz, "onTaskComplete", "(Lcom/automatak/dnp3/TaskInfo;)V");
            if(!this->method6) return false;

            this->method7 = env->GetMethodID(this->clazz, "onTaskStart", "(Lcom/automatak/dnp3/enums/MasterTaskType;Lcom/automatak/dnp3/TaskId;)V");
            if(!this->method7) return false;

            return true;
        }

        void MasterApplication::cleanup(JNIEnv* env)
        {
            env->DeleteGlobalRef(this->clazz);
        }

        jboolean MasterApplication::assignClassDuringStartup(JNIEnv* env, JMasterApplication instance)
        {
            return env->CallBooleanMethod(instance, this->method0);
        }

        LocalRef<JIterable> MasterApplication::getClassAssignments(JNIEnv* env, JMasterApplication instance)
        {
            return LocalRef<JIterable>(env, env->CallObjectMethod(instance, this->method1));
        }

        jlong MasterApplication::getMillisecondsSinceEpoch(JNIEnv* env, JMasterApplication instance)
        {
            return env->CallLongMethod(instance, this->method2);
        }

        void MasterApplication::onClose(JNIEnv* env, JMasterApplication instance)
        {
            env->CallVoidMethod(instance, this->method3);
        }

        void MasterApplication::onOpen(JNIEnv* env, JMasterApplication instance)
        {
            env->CallVoidMethod(instance, this->method4);
        }

        void MasterApplication::onReceiveIIN(JNIEnv* env, JMasterApplication instance, JIINField arg0)
        {
            env->CallVoidMethod(instance, this->method5, arg0);
        }

        void MasterApplication::onTaskComplete(JNIEnv* env, JMasterApplication instance, JTaskInfo arg0)
        {
            env->CallVoidMethod(instance, this->method6, arg0);
        }

        void MasterApplication::onTaskStart(JNIEnv* env, JMasterApplication instance, JMasterTaskType arg0, JTaskId arg1)
        {
            env->CallVoidMethod(instance, this->method7, arg0, arg1);
        }
    }
}
